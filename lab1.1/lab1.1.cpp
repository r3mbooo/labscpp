#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }
    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    void heal(int amount) {
        if (health == 100)
        {
            std::cout << name << "already full hp!" << std::endl;
            return;
        }
        health += amount;
        if (health > 100) {
            health = 100;
        }
        std::cout << name << " healed! Current hp:" << health << std::endl;

    }

    void takeDamage(int amount) {
        if (health - amount <= 0) {
            health = 0;
            std::cout << "Name: " << name << " died! Game over!" << std::endl;
            return;
        };
        health -= amount;
        std::cout << "Name: " << name << " gets damage: " << amount << ". Current hp: " << health << std::endl;

    }
    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    };


    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }
};

int main()
{
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    //Монстр использует лечение
    monster.heal(30);
    monster.displayInfo();

    //Монстр атакует
    hero.takeDamage(20);
    hero.displayInfo();

    return 0;
}