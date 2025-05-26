#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <random>
#include <algorithm>

template<typename T>
class Inventory {
    std::vector<T> items;
public:
    void addItem(const T& item) {
        items.push_back(item);
    }
    void removeItem(const T& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) items.erase(it);
    }
    void showInventory() const {
        std::cout << "Inventory:\n";
        for (const auto& item : items)
            std::cout << "- " << item << "\n";
    }
};

class Monster;


class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory<std::string> inventory;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0) {
    }

    void attackEnemy(Monster& enemy);
    void heal(int amount);
    void gainExperience(int exp);
    void addToInventory(const std::string& item);
    void displayInfo() const;
    bool isAlive() const { return health > 0; }
    void takeDamage(int damage);
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }

    void save(std::ofstream& file) const;
    void load(std::ifstream& file);
};


class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    virtual void attackEnemy(Character& enemy) {
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks, but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "\n";
    }

    bool isAlive() const { return health > 0; }
    int getDefense() const { return defense; }
    int getHealth() const { return health; }
    void takeDamage(int damage) { health -= damage; }

    virtual ~Monster() = default;
};

// Реализация методов Character
void Character::attackEnemy(Monster& enemy) {
    int damage = attack - enemy.getDefense();
    if (damage > 0) {
        enemy.takeDamage(damage);
        std::cout << name << " attacks " << enemy.getHealth() << " HP enemy for " << damage << " damage!\n";
    }
    else {
        std::cout << name << " attacks, but it has no effect!\n";
    }
}

void Character::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;
    std::cout << name << " heals for " << amount << " HP!\n";
}

void Character::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        level++;
        experience -= 100;
        std::cout << name << " leveled up to level " << level << "!\n";
    }
}

void Character::addToInventory(const std::string& item) {
    inventory.addItem(item);
}

void Character::displayInfo() const {
    std::cout << "Name: " << name << ", HP: " << health
        << ", Attack: " << attack << ", Defense: " << defense
        << ", Level: " << level << ", Experience: " << experience << "\n";
    inventory.showInventory();
}

void Character::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Character::save(std::ofstream& file) const {
    file << name << "\n" << health << " " << attack << " "
        << defense << " " << level << " " << experience << "\n";
}

void Character::load(std::ifstream& file) {
    file >> name >> health >> attack >> defense >> level >> experience;
}

// Конкретные монстры
class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 8, 2) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 25, 10, 3) {}
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 100, 20, 10) {}
};


class GameManager {
    std::unique_ptr<Character> player;
    std::vector<std::unique_ptr<Monster>> monsters;

    void generateMonsters() {
        monsters.push_back(std::make_unique<Goblin>());
        monsters.push_back(std::make_unique<Skeleton>());
        monsters.push_back(std::make_unique<Dragon>());
    }

public:
    void startGame() {
        std::string name;
        std::cout << "Enter your name in English: ";
        std::cin >> name;
        player = std::make_unique<Character>(name, 100, 15, 5);

        generateMonsters();
        std::cout << "The game has started!\n";
    }

    void saveGame(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cout << "Error saving game\n";
            return;
        }
        player->save(file);
        file.close();
        std::cout << "Game saved.\n";
    }

    void loadGame(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Error loading game\n";
            return;
        }
        player = std::make_unique<Character>("", 0, 0, 0);
        player->load(file);
        file.close();
        std::cout << "Game loaded.\n";
    }

    void battle() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, static_cast<int>(monsters.size() - 1));

        auto& monster = monsters[dis(gen)];
        std::cout << "\nYou were attacked by: ";
        monster->displayInfo();

        while (player->isAlive() && monster->isAlive()) {
            player->attackEnemy(*monster);
            if (!monster->isAlive()) {
                std::cout << "Monster defeated!\n";
                player->gainExperience(30);
                player->addToInventory("Gold");
                return;
            }

            monster->attackEnemy(*player);
            if (!player->isAlive()) {
                std::cout << "You were defeated!\n";
                return;
            }
        }
    }

    void showPlayerStatus() const {
        player->displayInfo();
    }
};

int main() {
    try {
        GameManager game;
        game.startGame();

        while (true) {
            std::cout << "\n1. Battle\n2. Show status\n3. Save\n4. Load\n5. Exit\n";
            int choice;
            std::cin >> choice;

            if (choice == 1) game.battle();
            else if (choice == 2) game.showPlayerStatus();
            else if (choice == 3) game.saveGame("save.txt");
            else if (choice == 4) game.loadGame("save.txt");
            else if (choice == 5) break;
        }
    }
    catch (...) {
        std::cerr << "Unknown error occurred!\n";
    }

    return 0;
}