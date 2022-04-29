//
//  main.cpp
//  mySmartPointer
//
//  Created by xiaoyuyu on 2022/4/29.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <iostream>
#include "myUniquePointer.cpp"
#include <string>

class A {
public:
    void show() {
        std::cout << "A::show()" << std::endl;
    }
};

void test1() {
    u_ptr<A> p1(new A);
    p1->show();

    // returns the memory address of p1
    std::cout << p1.get() << std::endl;

    // transfers ownership to p2
    u_ptr<A> p2 = std::move(p1);
    p2->show();
    std::cout << p1.get() << std::endl;
    std::cout << p2.get() << std::endl;

    // transfers ownership to p3
    u_ptr<A> p3 = std::move(p2);
    p3->show();
    std::cout << p1.get() << std::endl;
    std::cout << p2.get() << std::endl;
    std::cout << p3.get() << std::endl;
}

class Box {
private:
    int length, width, height;
public:
    static int cnt;
    Box(int _length = 0, int _width = 0, int _height = 0) : length(_length), width(_width), height(_height){
        cnt++;
    }
};

int Box::cnt = 0;

void test2() {
    u_ptr<Box> box1(new Box);   // cnt += 1;
    // creates a my_unique_ptr object holding an array of 'Box' objects
    u_ptr<Box[]> boxArr(new Box[3]);    // cnt += 3;
    Box b1 = boxArr[0]; // index based access
    std::cout << b1.cnt << std::endl;   // 4
}

void test3() {
    u_ptr<std::string> p(new std::string("hello world."));
    std::cout << *p << std::endl;
}

int main(int argc, const char * argv[]) {
    
    std::cout << "=====test1=====" << std::endl;
    test1();
    
    std::cout << "=====test2=====" << std::endl;
    test2();
  
    std::cout << "=====test3=====" << std::endl;
    test3();
    
    return 0;
}
