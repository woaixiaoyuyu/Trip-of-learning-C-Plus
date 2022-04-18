//
//  ObserverPattern_new.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <list>
#include <map>

class Observer_new {
public:
    Observer_new() = default;
    ~Observer_new() = default;
    Observer_new(const Observer_new&) = delete;
    Observer_new& operator=(const Observer_new&) = delete;
};

template <typename Func>
class Events : public Observer_new {
private:
    int id; // 观察者对应id
    std::map<int, Func> connections;    // 观察者列表
    
    int Assgin(Func&& f) {
        int k = id++;
        connections.emplace(k, std::forward<Func>(f));
        return k;
    }

public:
    Events() {}
    ~Events() {}
    
    int Connect(Func&& f) {
        return Assgin(std::forward<Func>(f));
    }
    
    int Connect(const Func& f) {
        return Assign(f);
    }
    
    void Disconnect(int key) {
        connections.erase(key);
    }
    
    template <typename... Args>
    void Notify(Args&&... args) {
        for (auto& it : connections) {
            it.second(std::forward<Args>(args)...);
        }
    }
};

struct stA {
    int a = 0, b = 0;
    void print() {
        std::cout  << "a + b = " << a + b << std::endl;
    }
};

void print(int a, int b) {
    std::cout << a << ", " << b << std::endl;
}

//int main() {
//    Events<std::function<void(int, int)>> event;
//    auto key = event.Connect(print);
//    stA t;
//    t.print();
//    event.Connect([&t](int a, int b) -> void {
//        t.a = a;
//        t.b = b;
//    });
//    int a = 1, b = 2;
//    event.Notify(a, b);
//    t.print();
//    event.Disconnect(key);
//    event.Notify(3, 4);
//    t.print();
//}
