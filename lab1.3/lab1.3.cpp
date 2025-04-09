#include <iostream>
#include <string>
#include <cstdlib> // Для rand() и srand()
#include <ctime>    // Для time()

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

    std::string getName() const {
        return name;
    }

    int getHealth() const { return health; }
    int getDefense() const { return defense; }
    void reduceHealth(int amount) { health -= amount; }

    // Виртуальный метод для атаки
    virtual void attack(Entity& target) {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    virtual void heal(int amount) {
        if (amount > 0) {
            health += amount;
            std::cout << name << " restores " << amount << " HP!\n";
        }
    }

    // Виртуальный деструктор
    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            // Шанс на критический удар
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "  Character: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
    // Переопределение метода heal с ограничением максимального здоровья
    void heal(int amount) override {
        if (health == 100)
        {
            std::cout << name << "already full hp!" << std::endl;
            return;
        }
        health += amount;
        if (health > 100) {
            health = 100;
        }
        std::cout << name << " healed by using a flask! Current hp: " << health << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            // Шанс на ядовитую атаку (30%)
            if (rand() % 100 < 30) {
                damage += 5; // Дополнительный урон от яда
                std::cout << "Poisonous attack! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "  Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }
    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            // Шанс на ледяное дыхание (40%)
            if (rand() % 100 < 40) {
                damage += 10; // Дополнительный урон от холода
                std::cout << "Coldbreeze! ";
                target.reduceHealth(damage);
                std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
                return;
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
            return;
        }
    }
    void displayInfo() const override {
        std::cout << "  Great dragon " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};


int main() {
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    // Создание объектов
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);

    Boss dragon("Wyvern", 150, 25, 20);

    // Массив указателей на базовый класс
    Entity* entities[] = { &hero, &goblin, &dragon };

    // Полиморфное поведение
    for (auto* entity : entities) {
        entity->displayInfo(); // Вывод информации о сущности
    }

    // Бой между персонажем и монстрами
    hero.attack(goblin);
    goblin.attack(hero);
    dragon.attack(hero);

    // Изменение статуса после битвы
    std::cout << std::endl;
    hero.displayInfo();
    goblin.displayInfo();
    dragon.displayInfo();

    hero.heal(30);


    return 0;
}