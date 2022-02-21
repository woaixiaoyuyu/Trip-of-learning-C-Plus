//
//  main.cpp
//  MyVector
//
//  Created by xiaoyuyu on 2022/2/10.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <iostream>
#include <stdio.h>
//#include "forInt.hpp"
#include "forDefault.hpp"
#include <string>

using namespace std;

//void show(const vectorForInt & p) {
//    cout << "size = " << p.size << " cap = " << p.cap << endl;
//}

template <typename T>
void show(const vectorForDefault<T> & p) {
    cout << "size = " << p.mySize() << " cap = " << p.myCap() << endl;
}

//void testForInt() {
//    vectorForInt test;
//    show(test); // size = 0 cap = 0
//    cout << test.empty() << endl;
//
//    // cout << test[2] << endl;    // out of boundry
//
//    test.push_back(1);
//    show(test); // size = 1 cap = 1
//    cout << test.empty() << endl;
//
//    test.push_back(2);
//    show(test); // size = 2 cap = 2
//
//    test.push_back(3);
//    show(test); // size = 3 cap = 4
//
//    test.push_back(1);
//    show(test); // size = 4 cap = 4
//
//    test.push_back(1);
//    show(test); // size = 5 cap = 8
//
//    test.push_back(1);
//    show(test); // size = 6 cap = 8
//
//    test.push_back(1);
//    show(test); // size = 7 cap = 8
//
//    test.push_back(1);
//    show(test); // size = 8 cap = 8
//
//    test.push_back(1);
//    show(test); // size = 9 cap = 16
//
//    test.pop_back();
//    show(test); // size = 8 cap = 16
//
//    test.myResize(18);
//    show(test); // size = 18 cap = 18
//
//    cout << "front = " << test.front() << " back = " << test.back() << endl;
//
//    cout << "第三个元素是 " << test[2] << endl;
//
//    cout << test;
//
//    cout << test.store << endl;
//
//    cout << &test.store[0] << endl;
//
//    cout << &test.store[test.start] << endl;
//
//    cout << test.begin() << endl;
//
//    cout << &test.store[test.finish] << endl;
//
//    cout << test.end() << endl;
//
//    cout << &test.store[test.finish] + 1 << endl;
//}

void testForDeafult() {
    vectorForDefault<int> p;
    cout << p.empty() << endl;  // 1
    show(p);    // size = 0 cap = 0
    
    p.insert(p.begin(), 10);
    
    for (int i = 1; i < 9; i++)
        p.push_back(i);
    cout << p.empty() << endl;  // 0
    show(p);    // size = 8 cap = 8
    
    p.push_back(20);
    show(p);    // size = 9 cap = 16
    
    cout << p << endl;
    
    cout << "front = " << p.front() << " back = " << p.back() << endl;  // front = 1 back = 20

    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    p.resize(18);
    show(p);    // size = 18 cap = 18
    cout << p << endl;
    
    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    p.pop_back();
    show(p);    // size = 17 cap = 18
    
    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    cout << p[3] << endl;   // 3
    
    p.insert(p.begin() + 9, 1233);  // 这里有个bug，思考ing
//    cout << p.begin() << endl;
//    cout << p.begin() + 9 << endl;
//    cout << &p[9] << endl;
//    cout << &p[10] << endl;
//    cout << &p.getWarehouse()[9] << endl;
//    cout << &p.getWarehouse()[10] << endl;
//    cout << p.begin() + 9
    
    cout << p << endl;
    
    p.erase(p.begin() + 1);
    cout << p << endl;
}

void testForDeafult2() {
    vectorForDefault<string> p;
    cout << p.empty() << endl;  // 1
    show(p);    // size = 0 cap = 0
    
    for (int i = 1; i < 9; i++)
        p.push_back("the " + to_string(i) + " sentence");
    cout << p.empty() << endl;  // 0
    show(p);    // size = 8 cap = 8
    
    p.push_back(to_string(20));
    show(p);    // size = 9 cap = 16
    
    cout << p << endl;
    
    cout << "front = " << p.front() << " back = " << p.back() << endl;  // front = 1 back = 20

    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    p.resize(18);
    show(p);    // size = 18 cap = 18
    cout << p << endl;
    
    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    p.pop_back();
    show(p);    // size = 17 cap = 18
    
    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    cout << p[3] << endl;   // 4
    
    p.insert(p.begin() + 9, "hello world.");
    
    cout << p << endl;
    
    p.clear();
    cout << p << endl;  // 空
    
    p.push_back("oh my god.");
    cout << p << endl;  // 第1个元素是 oh my god.
    cout << p.front() << endl;  // oh my god.
    cout << p.back() << endl;  // oh my god.
}

void testForDeafult3() {
    vectorForDefault<char> p;
    cout << p.empty() << endl;  // 1
    show(p);    // size = 0 cap = 0
    
    for (int i = 97; i < 105; i++)
        p.push_back(i);
    cout << p.empty() << endl;  // 0
    show(p);    // size = 8 cap = 8
    
    p.push_back(116);
    show(p);    // size = 9 cap = 16
    
    cout << p << endl;
    
    cout << "front = " << p.front() << " back = " << p.back() << endl;  // front = 1 back = 20

    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    p.resize(18);
    show(p);    // size = 18 cap = 18
    cout << p << endl;
    
    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    p.pop_back();
    show(p);    // size = 17 cap = 18
    
    cout << (p.begin() == &p[0]) << endl;   // 1 true
    
    cout << (p.end() == &p.getWarehouse()[p.mySize()])  << endl;   // 1 true
    
    cout << p[3] << endl;   // 4
    
    p.insert(p.begin() + 9, 'w');
    
    cout << p << endl;
}

// test for char*
void testForDeafult4() {
    vectorForDefault<char*> p;
    char * s = "hello world";
    char * s2 = "goodbye world";
    char * s3 = "lol";
    p.push_back(s);
    p.push_back(s2);
    p.insert(p.begin()+1, s3);
    cout << p;
    cout << p.front() << endl;
    cout << p.back() << endl;
    
    p.pop_back();
    cout << p;
    
    p.clear();
    cout << p;
    
    p.push_back(s);
    p.push_back(s2);
    p.push_back(s);
    p.push_back(s2);
    p.push_back(s);
    p.push_back(s2);
    cout << p;
    
    p.erase(p.begin()+2);
    cout << p;
}

class Person {
private:
    int age;
    
public:
    Person() {
        age = 0;
    }
    
    Person(int age) {
        this->age = age;
    }
    
    friend ostream& operator<<(ostream & os, const Person& p) {
        os << "age is: " << p.age;
        return os;
    }
};

void testForDefult5() {
    vectorForDefault<Person> p;
    Person a(3);
    p.push_back(a);
    cout << p[0] << endl;
}

int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
//    int a[3] = {1,2,3};
//    cout << a << endl;
//    cout << a + 1 << endl;
//    cout << &a[1] << endl;
//    cout << &a[0] + 1 << endl;
    // testForInt();
    // testForDeafult();   // test for int
    // testForDeafult2();   // test for string
    // testForDeafult3();   // test for char
    // testForDeafult4();
    testForDefult5();
//    string a = "what";
//    string b = "how";
//
//    cout << &a << endl;
//    cout << &b << endl;
//    cout << &a - &b << endl;
    
    return 0;
}
