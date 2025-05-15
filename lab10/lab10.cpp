#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Класс ресурса
class Resource {
    string name;
    int requiredAccessLevel;

public:
    Resource(const string& name, int level) {
        if (level < 0) throw invalid_argument("Invalid access level!");
        this->name = name;
        this->requiredAccessLevel = level;
    }

    string getName() const { return name; }
    int getAccessLevel() const { return requiredAccessLevel; }

    bool checkAccess(int userAccessLevel) const {
        return userAccessLevel >= requiredAccessLevel;
    }
};

// Базовый пользователь
class User {
protected:
    string name;
    int id;
    int accessLevel;

public:
    User(const string& name, int id, int accessLevel) {
        if (name.empty() && id < 0 && accessLevel < 0)
            throw invalid_argument("Invalid user data.");
        this->name = name;
        this->id = id;
        this->accessLevel = accessLevel;
    }

    virtual void displayInfo() const {
        cout << "Name: " << name << ", ID: " << id << ", Access Level: " << accessLevel << endl;
    }

    virtual ~User() = default;

    string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }
};

// Дочерние классы 
class Student : public User {
    string group;
public:
    Student(const string& name, int id, int accessLevel, const string& group)
        : User(name, id, accessLevel), group(group) {
    }

    void displayInfo() const override {
        cout << "[Student] " << name << ", Group: " << group << ", Access Level: " << accessLevel << endl;
    }
};

class Teacher : public User {
    string department;
public:
    Teacher(const string& name, int id, int accessLevel, const string& dept)
        : User(name, id, accessLevel), department(dept) {
    }

    void displayInfo() const override {
        cout << "[Teacher] " << name << ", Department: " << department << ", Access Level: " << accessLevel << endl;
    }
};

class Administrator : public User {
    string position;
public:
    Administrator(const string& name, int id, int accessLevel, const string& position)
        : User(name, id, accessLevel), position(position) {
    }

    void displayInfo() const override {
        cout << "[Admin] " << name << ", Position: " << position << ", Access Level: " << accessLevel << endl;
    }
};

// Шаблон системы контроля доступа
template<typename TUser, typename TResource>
class AccessControlSystem {
    vector<shared_ptr<TUser>> users;
    vector<TResource> resources;

public:
    void addUser(shared_ptr<TUser> user) {
        users.push_back(user);
    }

    void addResource(const TResource& res) {
        resources.push_back(res);
    }

    void checkAllAccess() {
        for (const auto& user : users) {
            user->displayInfo();
            for (const auto& res : resources) {
                cout << " - Resource: " << res.getName()
                    << (res.checkAccess(user->getAccessLevel()) ? " — ACCESS GRANTED" : " — ACCESS DENIED") << endl;
            }
            cout << endl;
        }
    }

    void saveUsers(const string& filename) {
        ofstream file(filename);
        for (const auto& user : users)
            file << user->getName() << " " << user->getId() << " " << user->getAccessLevel() << endl;
        file.close();
    }

    void loadUsers(const string& filename) {
        ifstream file(filename);
        string name;
        int id, level;
        while (file >> name >> id >> level)
            users.push_back(make_shared<User>(name, id, level));
        file.close();
    }

    void searchUserByName(const string& query) {
        for (const auto& user : users) {
            if (user->getName() == query)
                user->displayInfo();
        }
    }

    void sortUsersByAccess() {
        sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
            return a->getAccessLevel() > b->getAccessLevel();
            });
    }
};

// Пример
int main() {
        try {
        AccessControlSystem<User, Resource> system;

        system.addUser(make_shared<Student>("John", 1, 1, "CS-101"));
        system.addUser(make_shared<Teacher>("DrSmith", 2, 3, "Mathematics"));
        system.addUser(make_shared<Administrator>("Alice", 3, 5, "Dean"));

        system.addResource(Resource("Library", 1));
        system.addResource(Resource("Lab", 3));
        system.addResource(Resource("Server Room", 5));

        system.sortUsersByAccess();
        system.checkAllAccess();

        // Поиск
        system.searchUserByName("John");

        // Сохранение файла
        system.saveUsers("users.txt");

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}