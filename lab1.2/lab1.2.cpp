#include <iostream>
#include <string>

class Entity {
protected:
    std::string name; // Защищенное поле: имя
    int health;     // Защищенное поле: здоровье

public:
    // Конструктор базового класса
    Entity(const std::string& n, int h) : name(n), health(h) {}

    // Метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience; // Приватное поле: опыт

public:
    // Конструктор производного класса
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Experience: " << experience << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string type; // Приватное поле: тип врага

public:
    // Конструктор производного класса
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    // Переопределение метода displayInfo
    virtual void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Type: " << type << std::endl;
    }
};

class Boss : public Enemy {
private:
    std::string specialAbility; // Уникальное свойство Boss

public:
    // Конструктор Boss с specialAbility
    Boss(const std::string& n, int h, const std::string& t, const std::string& abilityDescription)
        : Enemy(n, h, t), specialAbility(abilityDescription) {}

    // Переопределение метода 
    virtual void displayInfo() const override {
        Enemy::displayInfo(); 
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }

    void useSpecialAbility() const {
        std::cout << name << " uses their special ability: " << specialAbility << "!" << std::endl;
        // Какая либо логика работы похищения жизни...
        std::cout << "(Drains life from the target and heals the user.)" << std::endl;
    }
}; 

int main() {
    // Создаем объекты игрока и врага
    Player hero("Hero", 100, 0);
    Enemy monster("Goblin", 50, "Goblin");
    Boss vampireLord("Lord Vorlag", 200, "Undead", "Drains the life force of enemies to heal itself");

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();
    vampireLord.displayInfo(); 

    //Проверяем работу метода
    vampireLord.useSpecialAbility(); 

    return 0;
}