//
//  forInt.hpp
//  MyVector
//
//  Created by xiaoyuyu on 2022/2/10.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//


// 仿照着写一个vector
// 这个版本存放的内容类型只能是int

//Iterators
//
//begin() – Returns an iterator pointing to the first element in the vector
//end() – Returns an iterator pointing to the theoretical element that follows the last element in the vector
//rbegin() – Returns a reverse iterator pointing to the last element in the vector (reverse beginning). It moves from last to first element
//rend() – Returns a reverse iterator pointing to the theoretical element preceding the first element in the vector (considered as reverse end)
//cbegin() – Returns a constant iterator pointing to the first element in the vector.
//cend() – Returns a constant iterator pointing to the theoretical element that follows the last element in the vector.
//crbegin() – Returns a constant reverse iterator pointing to the last element in the vector (reverse beginning). It moves from last to first element
//crend() – Returns a constant reverse iterator pointing to the theoretical element preceding the first element in the vector (considered as reverse end)
#include <stdio.h>
#include <iostream>

using namespace std;

// only for int
class vectorForInt {
public:
    int start;  // 第一个元素的下标
    int finish; // 最后一个元素的下标的后一位
    int cap;    // capacity for vector
    int size;   // size of vector
    int* store; // warehouse for storing
    
    // 默认的构造函数
    vectorForInt() {
        this->store = nullptr;
        this->start = -1;
        this->finish = this->start;
        this->cap = 0;
        this->size = 0;
    }
    
    // 第一个参数是size，第二个参数代表添加size个elem
    vectorForInt(int size, int elem) {
        this->store = new int[size + 1];    // 多一个元素空间，用来存放end()，因为end()实际上是最后一个元素的下一个元素
        this->start = 0;
        this->finish = size - 1;
        this->cap = size;
        this->size = size;
    }
    
    vectorForInt(const vectorForInt & p) {
        this->start = p.start;
        this->finish = p.finish;
        this->cap = p.cap;
        this->size = p.size;
        this->store = new int[this->size];
        for (int i = 0; i < this->size; i++) {
            this->store[i] = p.store[i];
        }
    }
    
    ~vectorForInt() {
        delete[] store;
        store = nullptr;
    }
    
    // begin()，实际返回的是一个iterator
    // 在这里，就是返回warehouse第一个元素的地址
    // 也就是 warehoue的地址，因为这里warehouse实际就是个数组
    int* begin() {
        return &store[start];
    }
    
    // end()，实际也是返回一个iterator
    // 在这里，就是返回warehouse最后一个元素的地址的后一个占位符的地址
    int* end() {
        return &store[finish] + 1;
    }
    
    int mySize() const {
        return this->size;
    }
    
    int myCapacity() const {
        return this->cap;
    }
    
    bool empty() {
        return start == finish;
    }
    
    int front() const {
        return store[start];
    }
    
    int back() const {
        return store[finish];
    }
    
    // 动态扩容
    // 规则和正常的vector一样
    // 新的vector的cap是原先的两倍
    void re_construct() {
        // 如果当前vector的size是0,则新的vector的size和cap都是1
        if (size == 0) {
            start = 0;  // 最开始是-1
            cap = 1;
            store = new int[1];
        } else {
            // 构造新的warehouse
            cap = 2 * cap;
            int * temp = new int[cap];
            // 迁移旧数据
            for (int i = 0; i < size; i++)
                temp[i] = store[i];
            // 销毁旧的warehouse
            delete [] store;
            store = nullptr;    // 以防万一，怕use after free
            store = temp;
        }
    }
    
    // 重载re_construct,为了匹配resize
    void re_construct(int newSize) {
        // 构造新的warehouse
        cap  = newSize;
        int * temp = new int[cap];
        // 迁移旧数据
        for (int i = 0; i < size; i++)
            temp[i] = store[i];
        // 销毁旧的warehouse
        delete [] store;
        store = nullptr;    // 以防万一，怕use after free
        store = temp;
    }
    
    void push_back(const int elem) {
        // 先判断store是否已经满了
        if (size == cap) {
            // 如果已经装满了，需要动态扩容
            re_construct();
        }
        // 没装满或者动态扩容后，直接装
        ++finish;
        ++size;
        store[finish] = elem;
    }
    
    void pop_back() {
        --finish;
        --size;
    }
    
    void myResize(int newSize) {
        this->cap = newSize;
        // 如果new size没有超过cap，则无事发生
        // 否则需要动态扩容
        re_construct(newSize);
    }
    
    // 重载operator[]
    int operator[](int idx) const {
        // 判断是否越界
        if (idx > size) {
            cout << "out of boundry" << endl;
            exit(-1);
        }
        return store[idx];
    }

};

// 重载operator<<
std::ostream & operator<<(std::ostream & os, const vectorForInt & p) {
    int size = p.mySize();
    for (int i = 0; i < size; i++) {
        os << "第" << i << "个元素是" << p[i] << endl;
    }
    return os;
}
