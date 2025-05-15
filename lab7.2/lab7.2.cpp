#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

using namespace std;

class Entity {
protected:
    string name;
    int health;
    int attack;
    int defense;

public:
    Entity(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    virtual void displayInfo() const {
        cout << name << " | HP: " << health
            << ", ATK: " << attack
            << ", DEF: " << defense << endl;
    }

    int getHealth() const { return health; }
    void setHealth(int h) { health = h; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    string getName() const { return name; }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }
};

class Monster : public Entity {
public:
    Monster(const string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }
};

// Общие объекты и мьютекс
Character hero("Hero", 100, 20, 10);
Monster monster("Goblin", 50, 15, 5);
mutex dataMutex;

void generateMonster() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(5));
        lock_guard<mutex> lock(dataMutex);
        monster = Monster("Goblin", 50, 15, 5);
        cout << "[System] New monster generated!" << endl;
    }
}

void fight() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(2));
        lock_guard<mutex> lock(dataMutex);

        if (monster.getHealth() <= 0 || hero.getHealth() <= 0)
            continue;

        int damageToMonster = max(0, hero.getAttack() - monster.getDefense());
        monster.setHealth(monster.getHealth() - damageToMonster);
        cout << hero.getName() << " attacks " << monster.getName()
            << " for " << damageToMonster << " damage." << endl;

        if (monster.getHealth() > 0) {
            int damageToHero = max(0, monster.getAttack() - hero.getDefense());
            hero.setHealth(hero.getHealth() - damageToHero);
            cout << monster.getName() << " attacks " << hero.getName()
                << " for " << damageToHero << " damage." << endl;
        }
        else {
            cout << monster.getName() << " is defeated!" << endl;
        }

        if (hero.getHealth() <= 0) {
            cout << hero.getName() << " has been defeated. Game Over." << endl;
            exit(0);
        }
    }
}

int main() {
    thread t1(generateMonster);
    thread t2(fight);

    t1.detach();
    t2.detach();

    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        lock_guard<mutex> lock(dataMutex);
        cout << "\n[Status Update]" << endl;
        hero.displayInfo();
        monster.displayInfo();
    }

    return 0;
}