//
//  VisitorPattern_new.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <iostream>

template <typename... Types>
struct Visitor;

template <typename T, typename... Types>
struct Visitor<T, Types...> : Visitor<Types...> {
    using Visitor<Types...>::Visit; // 通过using避免隐藏基类的Visit同名方法
    virtual void Visit(const T&) = 0;
};

template <typename T>
struct Visitor<T> {
    virtual void Visit(const T&) = 0;
};

struct stA;
struct stB;

struct Base {
    using MyVisitor = Visitor<stA, stB>;
    /**
     using MyVisitor = Visitor<stA, stB>; 会自动生成stA,stB的visit虚函数，其实就是模版的递归调用
     struct Visitor<stA, stB> {
        virtual void Visit(const stA&) = 0;
        virtual void Visit(const stB&) = 0;
     };
     当访问者需要增加stC接口时，直接定义一个新类型就可以了
     using MyVisitor = Visitor<stA, stB, stC>;
     */
    virtual void Accept(MyVisitor&) = 0;
};

struct stA : Base {
    double val;
    void Accept(Base::MyVisitor& v) override{
        v.Visit(*this);
    }
};

struct stB : Base {
    int val;
    void Accept(Base::MyVisitor& v) override{
        v.Visit(*this);
    }
};

struct PrintVisitor : Base::MyVisitor {
    void Visit(const stA& a) {
        std::cout << "from stA: " << a.val << std::endl;
    }
    
    void Visit(const stB& b) {
        std::cout << "from stB: " << b.val << std::endl;
    }
};

void TestVisitor2() {
    PrintVisitor vis;
    stA a;
    a.val = 8.97;
    stB b;
    b.val = 8;
    Base* base = &a;
    base->Accept(vis);
    base = &b;
    base->Accept(vis);
}

//int main() {
//    TestVisitor2();
//}
