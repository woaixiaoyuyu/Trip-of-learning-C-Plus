//
//  VisitorPattern.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <iostream>

struct ConcreteElement1;
struct ConcreteElement2;

// 访问者基类
struct Visitor {
    virtual ~Visitor() {}
    virtual void Visit(ConcreteElement1* element) = 0;
    
    virtual void Visit(ConcreteElement2* element) = 0;
};

// 被访问者基类
struct Element {
    virtual ~Element() {}
    virtual void Accept(Visitor& visitor) = 0;
};

// 具体访问者
struct ConcreteVisitor : public Visitor {
    void Visit(ConcreteElement1* element) override {
        std::cout << "Visit ConcreteElement1" << std::endl;
    }
    
    void Visit(ConcreteElement2* element) override {
        std::cout << "Visit ConcreteElement2" << std::endl;
    }
};

// 具体被访问者
struct ConcreteElement1 : public Element {
    void Accept(Visitor& visitor) override {
        visitor.Visit(this);    // 二次分派
    }
};

struct ConcreteElement2 : public Element {
    void Accept(Visitor& visitor) override {
        visitor.Visit(this);
    }
};

void TestVisitor() {
    ConcreteVisitor v;
    std::unique_ptr<Element> emt1(new ConcreteElement1());
    std::unique_ptr<Element> emt2(new ConcreteElement2());
    emt1->Accept(v);
    emt2->Accept(v);
}

//int main() {
//    TestVisitor();
//}
