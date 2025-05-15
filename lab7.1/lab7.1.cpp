#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <new>
#include <ostream>
using namespace std;

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;
public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {
    }
    virtual void displayInfo() const {
        cout << "Name: " << name << ", HP: " << health << ", ATK: " << attackPower << ", DEF: " << defense << endl;
    }
    virtual ~Entity() = default;

    std::string getName() const {
        return name;
    }

    int getHealth() const { return health; }
    virtual void save(std::ofstream& out) const = 0;
    virtual void load(std::ifstream& in) = 0;
};

class Player : public Entity {
    int experience;
public:
    Player(const string& n, int h, int a, int d, int exp) : Entity(n, h, a, d), experience(exp) {}
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "Experience: " << experience << endl;
    }
    void save(std::ofstream& out) const override {
        out << "Player\n" << name << "\n" << health << "\n" << attackPower << "\n" << defense << "\n" << experience << "\n";
    }

    void load(std::ifstream& in) override {
        std::getline(in, name);
        in >> health >> attackPower >> defense >> experience;
        in.ignore();
    }
    Player() : Entity("", 0, 0, 0), experience(0) {}//////////
};

class Enemy : public Entity {
    string type;
public:
    Enemy(const string& n, int h, int a, int d, const string& t) : Entity(n, h, a, d), type(t) {}
    void displayInfo() const override {
        Entity::displayInfo();
        cout << "Type: " << type << endl;
    }
    void save(std::ofstream& out) const override {
        out << "Enemy\n" << name << "\n" << health << "\n" << attackPower << "\n" << defense << "\n" << type << "\n";
    }

    void load(std::ifstream& in) override {
        std::getline(in, name);
        in >> health >> attackPower >> defense >> type;
        in.ignore();
    }
};

class Boss : public Enemy {
    string specialAbility;
public:
    Boss(const string& n, int h, int a, int d, const string& t, const string& b)
        : Enemy(n, h, a, d, t), specialAbility(b) {
    }
    void displayInfo() const override {
        Enemy::displayInfo();
        cout << "Special Ability: " << specialAbility << endl;
    }
    void save(std::ofstream& out) const override {
        out << "Boss\n" << name << "\n" << health << "\n" << attackPower << "\n" << defense << "\n" << specialAbility << "\n";
    }

    void load(std::ifstream& in) override {
        std::getline(in, name);
        in >> health >> attackPower >> defense >> specialAbility;
        in.ignore();
    }
};

template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        if (entity->getHealth() <= 0) {
            throw std::invalid_argument("Entity has invalid health");
        }
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    std::vector<T> getEntities() const {
        return entities;
    }

};

void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        const auto& entities = manager.getEntities();
        file << entities.size() << "\n";
        for (const auto& entity : entities) {
            entity->save(file);
        }
    }

    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

}

void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; ++i) {
        std::string type;
        std::getline(file, type);

        Entity* entity = nullptr;

        if (type == "Player") {
            entity = new Player();
        }
        else if (type == "Enemy") {
            entity = new Enemy("", 0, 0, 0, "");
        }
        else if (type == "Boss") {
            entity = new Boss("", 0, 0, 0, "", "");
        }
        else {
            throw std::runtime_error("Unknown type: " + type);
        }

        entity->load(file);
        manager.addEntity(entity);

    }

}

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 10, 15, 35));
    manager.addEntity(new Enemy("Goblin", 50, 15, 10, "Forest"));
    manager.addEntity(new Enemy("Mermaid", 75, 25, 5, "Sea"));
    saveToFile(manager, "game_save.txt");

    GameManager<Entity*> loadedManager;
    loadFromFile(loadedManager, "game_save.txt");
    cout << "Loaded entities:\n";
    loadedManager.displayAll();

    return 0;
}