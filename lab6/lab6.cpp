#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <deque>
using namespace std;

template <typename T>
class Queue {
    deque<T> elements;
public:
    void push(const T& item) {
        elements.push_back(item);
        cout << "Element added: " << item << endl;
    }
    void pop() {
        if (elements.empty()) {
            throw runtime_error("Queue is empty!");
        }
        cout << "Removed element: " << elements.front() << endl;
        elements.pop_front();
    }
    bool empty() const { return elements.empty(); }
};

int main() {
    try {
        Queue<int> intQueue;
        intQueue.push(321);
        intQueue.push(123);
        intQueue.pop();
        intQueue.pop();
        intQueue.pop();
    }
    catch (const runtime_error& e) {
        cerr << "Queue error: " << e.what() << endl;
    }

    return 0;
}