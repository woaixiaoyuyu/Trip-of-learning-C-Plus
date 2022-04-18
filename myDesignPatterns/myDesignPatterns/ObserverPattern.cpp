//
//  ObserverPattern.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 最基本的观察者模式

#include <iostream>
#include <stdio.h>
#include <list>

class Observer {
public:
    Observer() = default;
    ~Observer() = default;
    virtual void Update() = 0;  // 纯虚函数，抽象类
};

class Subject {
private:
    std::list<Observer*> observers;
    int state = 0;
public:
    ~Subject() {};
    Subject() {};
    
    int GetState() {
        return state;
    }
    
    void SetState(int state) {
        this->state = state;
        NotifyAll();
    }
    
    void Attach(Observer* o) {
        observers.emplace_back(o);
    }
    
    void Detach(Observer* o) {
        observers.remove(o);
    }
    
    void NotifyAll() {
        for (Observer* elem : observers) {
            elem->Update();
        }
    }
};

class Event1 : public Observer {
private:
    Subject* subject;
public:
    Event1(Subject& p) {
        subject = &p;
        subject->Attach(this);
    }
    
    ~Event1() {}
    
    void Update() override {
        std::cout << "event 1 get state " << subject->GetState() << std::endl;
    }
};

class Event2 : public Observer {
private:
    Subject* subject;
public:
    Event2(Subject& p) {
        subject = &p;
        subject->Attach(this);
    }
    
    ~Event2() {}
    
    void Update() override {
        std::cout << "event 2 get state " << subject->GetState() << std::endl;
    }
};


//int main() {
//    Subject subject;
//    Event1 event1(subject);
//    subject.SetState(1);
//    Event2 event2(subject);
//    std::cout << subject.GetState() << std::endl;
//    subject.SetState(2);
//    std::cout << subject.GetState() << std::endl;
//}
