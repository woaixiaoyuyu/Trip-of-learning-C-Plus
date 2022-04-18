//
//  SingletonPattern_new.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 对单例模式的改进

#include <stdio.h>
#include <stdexcept>


template <typename T>
class Singleton_new {
public:
    Singleton_new() = delete;
    Singleton_new(const Singleton_new&) = delete;
    Singleton_new& operator= (const Singleton_new&) = delete;
    virtual ~Singleton_new() = delete;
public:
    // 可变参数模版
    template <typename... Args>
    static T* Instance(Args&&... args) {
        if (instance == nullptr) {
            instance = new T(std::forward<Args>(args)...);
        }
        return instance;
    }
    
    static T* GetInstance() {
        if (instance == nullptr) {
            throw std::logic_error("please first initialize./n");
        }
        return instance;
    }
    
    static void DestoryInstance() {
        delete instance;
        instance = nullptr;
    }
private:
    static T* instance;
};

template <class T>
T* Singleton_new<T>::instance = nullptr;

struct A {
    A(){}
};

struct B {
    B(int x) {}
};

struct C {
    C(int x, double y) {}
};

int main() {
    Singleton_new<A>::Instance();
    Singleton_new<B>::Instance(1);
    Singleton_new<C>::Instance(1, 2.2);
    
    Singleton_new<A>::DestoryInstance();
    Singleton_new<B>::DestoryInstance();
    Singleton_new<C>::DestoryInstance();
}
