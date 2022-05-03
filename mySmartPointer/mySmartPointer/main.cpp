//
//  main.cpp
//  mySmartPointer
//
//  Created by xiaoyuyu on 2022/4/29.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <iostream>
#include <string>
#include <thread>
#include "myUniquePointer.cpp"
#include "mySharedPointer.cpp"

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
    Vec3(int _x = 0, int _y = 0, int _z = 0) noexcept : x(_x), y(_y), z(_z) {
    }
    
    Vec3& operator*() {
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v)
    {
        os << '{' << "x:" << v.x << " y:" << v.y << " z:" << v.z  << '}';
        return os;
    }
};
 
void test4() {
    // 使用默认构造函数。
    u_ptr<Vec3> v1 = make_uptr<Vec3>();
    // 使用匹配这些参数的构造函数
    u_ptr<Vec3> v2 = make_uptr<Vec3>(Vec3(0, 1, 2));
    // 创建指向 5 个元素数组的 unique_ptr
    u_ptr<Vec3[]> v3 = make_uptr<Vec3[]>(5);
    
    Vec3* x = v1.get();
    std::cout << *x << std::endl;

    std::cout << "make_uptr<Vec3>():      " << *v1 << '\n'
              << "make_uptr<Vec3>(new Vec3(0, 1, 2)): " << *v2 << '\n'
              << "make_uptr<Vec3[]>(5):   " << '\n';
    for (int i = 0; i < 5; i++) {
        std::cout << "     " << v3[i] << '\n';
    }
}

// 测试u_ptr自定义删除器
struct deleter {
    void operator()(Box* ptr) {
        std::cout << "仿函数 deleter1." << std::endl;
    }
};

struct deleter2 {
    void operator()(Box* ptr) {
        std::cout << "仿函数 deleter2." << std::endl;
    }
};

void test5() {
    auto f = [](Box* ptr) -> void {
        std::cout << "lambda deleter1." << std::endl;
    };
    auto f2 = [](Box* ptr) -> void {
        std::cout << "lambda deleter2." << std::endl;
    };
    u_ptr<Box, void(*)(Box* ptr)> box1(new Box, f);
    u_ptr<Box, std::function<void(Box* ptr)>> box2(new Box, f2);
    u_ptr<Box, std::function<void(Box* ptr)>> box3(new Box, deleter());
    deleter2 deletee;
    u_ptr<Box, std::function<void(Box* ptr)>> box4(new Box, deletee);
}

void test6() {
    s_ptr<A> p1(new A);
    std::cout << p1.get() << std::endl;
    p1->show();
    std::cout << p1.use_count() << std::endl;
    s_ptr<A> p2(p1);
    p2->show();
    std::cout << p1.get() << std::endl;
    std::cout << p2.get() << std::endl;
    
    // Returns the number of shared_ptr objects
    // referring to the same managed object.
    std::cout << p1.use_count() << std::endl;
    std::cout << p2.use_count() << std::endl;
    
    // Relinquishes ownership of p1 on the object
    // and pointer becomes NULL
    p1.reset();
    std::cout << p1.get() << std::endl;
    std::cout << p2.use_count() << std::endl;
    std::cout << p2.get() << std::endl;
}

void thr(s_ptr<A> p) {
    std::cout << p.use_count() << std::endl;
    s_ptr<A> q(p);
    std::cout << q.use_count() << std::endl;
}

void thr2(s_ptr<A> p) {
    std::cout << p.use_count() << std::endl;
    s_ptr<A> q(p);
    std::cout << q.use_count() << std::endl;
}

// 测一下s_ptr<T> 的多线程
void test7() {
    s_ptr<A> p = make_sptr<A>();
    std::cout << p.use_count() << std::endl;
    // 每构造一个 thread，都需要把 p 复制到 thread 内部
    // 构造 std::thread 的时候，线程会先用你提供的参数构造线程内的对象，然后该对象被移动给线程函数的实参
    std::thread t1(thr, p);
    std::thread t2(thr2, p);
    t1.join();
    t2.join();
    std::cout << p.use_count() << std::endl;
}


int main(int argc, const char * argv[]) {
    
    // u_ptr test
    std::cout << "=====test1=====" << std::endl;
    test1();
    
    std::cout << "=====test2=====" << std::endl;
    test2();
  
    std::cout << "=====test3=====" << std::endl;
    test3();
    
    std::cout << "=====test4=====" << std::endl;
    test4();
    
    std::cout << "=====test5=====" << std::endl;
    test5();
    
    // s_ptr test
    std::cout << "=====test6=====" << std::endl;
    test6();
    
    std::cout << "=====test7=====" << std::endl;
    test7();
    
    return 0;
}
