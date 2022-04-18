//
//  SingletonPattern.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 单例模式 重载

#include <stdio.h>
#include <stdexcept>


template <typename T>
class Singleton {
public:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton&) = delete;
    virtual ~Singleton() = delete;
public:
    // 参数个数为0
    static T* Instance() {
        if (instance == nullptr) {
            instance = new T();
        }
        return instance;
    }
    
    // 参数个数为1
    template <typename T0>
    static T* Instance(T0 arg0) {
        if (instance == nullptr) {
            instance = new T(arg0);
        }
        return instance;
    }
    
    // 参数个数为2
    template <typename T0, typename T1>
    static T* Instance(T0 arg0, T1 arg1) {
        if (instance == nullptr) {
            instance = new T(arg0, arg1);
        }
        return instance;
    }
    
    // 参数个数为3
    template <typename T0, typename T1, typename T2>
    static T* Instance(T0 arg0, T1 arg1, T2 arg2) {
        if (instance == nullptr) {
            instance = new T(arg0, arg1, arg2);
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
T* Singleton<T>::instance = nullptr;

//struct A {
//    A(){}
//};
//
//struct B {
//    B(int x) {}
//};
//
//struct C {
//    C(int x, double y) {}
//};
//
//int main() {
//    Singleton<A>::Instance();
//    Singleton<B>::Instance(1);
//    Singleton<C>::Instance(1, 2.2);
//
//    Singleton<A>::DestoryInstance();
//    Singleton<B>::DestoryInstance();
//    Singleton<C>::DestoryInstance();
//}
