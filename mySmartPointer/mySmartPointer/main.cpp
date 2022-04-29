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
    std::string *s = p.get();
    std::cout << *s << std::endl;
}

struct Vec3 {
    int x, y, z;
    // C++20 起不再需要以下构造函数
    Vec3(int x = 0, int y = 0, int z = 0) noexcept : x(x), y(y), z(z) {
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v)
    {
        return os << '{' << "x:" << v.x << " y:" << v.y << " z:" << v.z  << '}';
    }
};
 
void test4() {
    // 使用默认构造函数。
    u_ptr<Vec3> v1 = make_uptr<Vec3>();
    // 使用匹配这些参数的构造函数
    u_ptr<Vec3> v2 = make_uptr<Vec3>(0, 1, 2);
//    // 创建指向 5 个元素数组的 unique_ptr
//    u_ptr<Vec3[]> v3 = make_uptr<Vec3[]>(5);
//
//    std::cout << "make_uptr<Vec3>():      " << *v1 << '\n'
//              << "make_uptr<Vec3>(0,1,2): " << *v2 << '\n'
//              << "make_uptr<Vec3[]>(5):   " << '\n';
//    for (int i = 0; i < 5; i++) {
//        std::cout << "     " << v3[i] << '\n';
//    }
}


int main(int argc, const char * argv[]) {
    
    std::cout << "=====test1=====" << std::endl;
    test1();
    
    std::cout << "=====test2=====" << std::endl;
    test2();
  
    std::cout << "=====test3=====" << std::endl;
    test3();
    
    std::cout << "=====test4=====" << std::endl;
    test4();
    
    return 0;
}
