#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
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
            std::cout << "- " << item << std::endl;
    }
};

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
    void attackEnemy(Monster& enemy) {
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks " << enemy.getHealth() << " HP enemy for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks, but it has no effect!" << std::endl;
        }
    }
    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }
    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            std::cout << name << " leveled up to level " << level << "!" << std::endl;
        }
    }
    void addToInventory(const std::string& item) {
        inventory.addItem(item);
    }
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << std::endl;
        inventory.showInventory();
    }
    std::string getName() const {
        return name;
    }
    int getHealth() const {
        return health;
    }
    int getDefense() const {
        return defense;
    }
    int getAttack() const {
        return attack;
    }
    void save(std::ofstream& file) const {
        file << name << "\n" << health << " " << attack << " "
            << defense << " " << level << " " << experience << "\n";
    }
    void load(std::ifstream& file) {
        file >> name >> health >> attack >> defense >> level >> experience;
    }
    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
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
            std::cout << name << " attacks for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks, but it has no effect!" << std::endl;
        }
    }
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << ", ATK: " << attack << ", Defense: " << defense << std::endl;
    }
    bool isAlive() const { return health > 0; }
    int getDefense() const { return defense; }
    int getHealth() const { return health; }
    void takeDamage(int damage) { health -= damage; }
    virtual ~Monster() = default;
};
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
        std::cout << "Enter Name on English: ";
        std::cin >> name;
        player = std::make_unique<Character>(name, 100, 15, 5);

        generateMonsters();
        std::cout << "The Game has started!\n";
    }
    void saveGame(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) { std::cout << "Game save error" << std::endl; }
        player->save(file);
        file.close();
    }
    void loadGame(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Error loading game" << std::endl;
        }
        player = std::make_unique<Character>("", 0, 0, 0);
        player->load(file);
        file.close();
    }
    void battle() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, monsters.size() - 1);

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
                std::cout << "The player is defeated!\n";
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
            std::cout << "\n1. Start\n2. Show status\n3. Save\n4. Load\n5. Exit\n";
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
        std::cerr << "Unknown error!" << std::endl;
    }
    return 0;
}