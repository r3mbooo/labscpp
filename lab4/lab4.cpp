#include <memory>
#include <string>
#include <iostream>
#include <vector>
class Entity {
protected:
    std::string name; // Защищенное поле: имя
    int health;      // Защищенное поле: здоровье

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
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Type: " << type << std::endl;
    }
};

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items;
public:
    Inventory() {}
    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item));
        std::cout << item << " has been added!" << std::endl;

    }
    void removeItem(const std::string& item) {
        auto it = std::find(items.begin(), items.end(), item);
        std::cout << item << " has been deleted!" << std::endl;
        if (it != items.end()) {
            items.erase(it);
        }
        else {
            std::cerr << "Item not found!";
        }
    }
    void displayInventory() const {
        std::cout << "Inventory:" << std::endl;
        if (items.empty()) {
            std::cout << "Inventory is Empty!" << std::endl;
        }
        else {
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << i + 1 << ". " << items[i] << std::endl;
            }
        }
    }
};
int main() {
    // Массив умных указателей на базовый класс
    std::unique_ptr<Entity> entities[] = {
        std::make_unique<Player>("Hero", 100, 0),
        std::make_unique<Enemy>("Goblin", 50, "Goblin")
    };

    // Полиморфизм и автоматическое управление памятью
    for (const auto& entity : entities) {
        entity->displayInfo();
    }

    Inventory backpack;
    backpack.addItem("Bow");
    backpack.addItem("Apple");
    backpack.addItem("Potion");
    backpack.displayInventory();
    backpack.removeItem("Apple");
    backpack.displayInventory();

    return 0;
}