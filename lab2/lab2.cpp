#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    int weight;

public:
    // Конструктор для инициализации полей оружия
    Weapon(const std::string& n, int dmg, int w)
        : name(n), damage(dmg), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор, выводящий сообщение об уничтожении объекта
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Метод для вывода информации об оружии
    void displayInfo() const {
        std::cout << "Weapon: " << name << ", Damage: " << damage << ", Weight: " << weight << std::endl;
    }
};

int main() {
    // Создаем несколько объектов Weapon и демонстрируем их работу
    Weapon sword("Iron Sword", 10, 2);
    Weapon bow("Long Bow", 7, 1);
    Weapon staff("Wooden Staff", 5, 1);

    //Информация об оружии
    std::cout << std::endl;
    sword.displayInfo();
    bow.displayInfo();
    staff.displayInfo();

    //Создание персонажей и монстров:
    std::cout << std::endl;
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);

    //Конец программы, вызов деструкторов

    return 0;
}