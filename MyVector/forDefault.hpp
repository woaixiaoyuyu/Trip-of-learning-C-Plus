//
//  forDefault.hpp
//  MyVector
//
//  Created by xiaoyuyu on 2022/2/11.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 仿照着写一个vector
// 上个版本存放的内容类型只能是int
// 这个版本构思的时候，发现了上个版本好多没测出来的逻辑上的bug，这个版本修补一下

// 这是第二个版本，可以存放所有默认的基础类型

#include <stdio.h>
#include <iostream>

using namespace std;

template <typename T>
class vectorForDefault {
private:
    int first;  // 第一个元素的下标
    int last; // 最后一个元素的下标的后一位
    int cap;    // capacity for vector
    int size;   // size of vector
    T* warehouse; // warehouse for storing
    // 底层实际设计的是一个数组，不是迭代器
    // 如果需要传递自定义类型，需要特化模版类
    // 如果不想特化模版类，也就是将自定义类型和默认类型一视同仁
    // 需要识别出传进来的T的类型，是默认类型，还是自定义类型
    // 要仿照实际中vector的value_type变量
    // 实际还是需要将warehouse构造成一个类，来对标iterator
    
    // 默认的动态扩容
    // 规则和正常的vector一样
    // 新的vector的cap是原先的两倍
    void re_construct() {
        // 如果当前vector的size是0,则新的vector的size和cap都是1
        if (size == 0) {
            first = 0;  // 最开始是-1
            cap = 1;
            warehouse = new T[1];
        } else {
            // 构造新的warehouse
            cap = 2 * cap;
            T * temp = new T[cap];
            // 迁移旧数据
            for (int i = 0; i < size; i++)
                temp[i] = warehouse[i];
            // 销毁旧的warehouse
            delete [] warehouse;
            warehouse = nullptr;    // 以防万一，怕use after free
            warehouse = temp;
        }
    }
    
    // resize会调用的重载后的动态扩容
    // 重载re_construct,为了匹配resize
    void re_construct(int n) {
        // 构造新的warehouse
        cap  = n;
        T * temp = new T[cap + 1];
        // 迁移旧数据
        for (int i = 0; i < size; i++)
            temp[i] = warehouse[i];
        for (int i = last + 1; i < n; i++) {
            T x;
            temp[i] = x;
        }
        size = n;
        // 销毁旧的warehouse
        delete [] warehouse;
        warehouse = nullptr;    // 以防万一，怕use after free
        warehouse = temp;
    }
    
public:
    // 默认构造函数
    vectorForDefault();
    
    // n,elem 重载的构造函数
    // 第一个参数是size，第二个参数代表添加size个elem
    vectorForDefault(const int & len, const T & elem);
    
    // 拷贝构造函数
    vectorForDefault(const vectorForDefault & p);
    
    // 析构函数
    ~vectorForDefault() {
        delete [] warehouse;
        warehouse = nullptr;
    }
    
    // front
    T front() const {
        // 判断是否为空
        if (empty()) {
            cout << "out of boundry" << endl;
            exit(-1);
        } else {
            return this->warehouse[first];
        }
    }
    
    // back
    T back() const {
        // 判断是否为空
        if (empty()) {
            cout << "out of boundry" << endl;
            exit(-1);
        } else {
            return this->warehouse[last];
        }
    }
    
    // begin
    T* begin() const {
        // 判断是否为空，如果为空，指向提前为end()设置的标志位
        if (size == 0) {
            return &warehouse[cap];
        } else {
            return &warehouse[first];
        }
    }
    
    // end
    T* end() const {
        // 判断是否为空，如果为空，指向提前为end()设置的标志位
        if (size == 0) {
            return &warehouse[cap];
        } else {
            return &warehouse[last + 1];
        }
    }
    
    // push_back
    void push_back(const T & elem);
    
    // pop_back
    void pop_back() {
        --last;
        --size;
    }
    
    // size
    int mySize() const {
        return this->size;
    }
    
    // capacity
    int myCap() const {
        return this->cap;
    }
    
    // resize
    void resize(int n);
    
    // empty
    bool empty() const {
        // V.begin() == V.end()
        return begin() == end();
    }
    
    // get warehouse 便于之后测试
    T* getWarehouse() const {
        return warehouse;
    }
    
    // get last 便于之后测试
    int getLast() {
        return last;
    }
    
    // insert
    void insert(T* idx, T val);
    
    // erase
    void erase(T* idx);
    
    // clear
    void clear();
    
    // 重载operator[]
    T& operator[](int idx) const;
    
    // 重载operator<<
    friend ostream& operator<<(ostream & os, const vectorForDefault<T> & p) {
        int size = p.mySize();
        for (int i = 0; i < size; i++) {
            os << "第" << i + 1 << "个元素是 " << p[i] << endl;
        }
        return os;
    }
};

template <typename T>
vectorForDefault<T>::vectorForDefault() : size(0), cap(0) ,first(-1), last(-1) {
    warehouse = new T[1];   // 开辟一个元素为了以后当作last后的一个标识位，用来存放end()
    // 其实多存一位没什么用，直接操作地址也可以，有点多此一举
}

template <typename T>
vectorForDefault<T>::vectorForDefault(const int & len, const T & elem) {
    size = len;
    cap = len;
    first = 0;
    last = len - 1;
    warehouse = new T[cap + 1]; // 多开辟一个空间放end()
    for (int i = 0; i < len; i++)
        warehouse[i] = elem;
}

template <typename T>
vectorForDefault<T>::vectorForDefault(const vectorForDefault & p) {
    // 先销毁原先的空间，因为这次设计上默认会开辟一个空间，所以百分之一百需要销毁，不判断warehouse是否是空指针
    delete [] this->warehouse;
    this->warehouse = nullptr;
    // 执行拷贝
    // Constructs a container with a copy of each of the elements in x, in the same order.
    this->cap = p.cap;
    this->first = p.first;
    this->last = p.last;
    this->size = p.size;
    this->warehouse = new T[cap + 1];
    for (int i = 0; i < p.size; i++) {
        // 如果拷贝对象的size为0，则会直接跳出
        this->warehouse[i] = p.warehouse[i];
    }
}

template <typename T>
void vectorForDefault<T>::push_back(const T & elem) {
    // 先判断是否已经装满了
    if (size == cap) {
        // 如果已经装满了，需要动态扩容
        re_construct();
    }
    // 没装满或者动态扩容后，直接装
    ++last;
    ++size;
    warehouse[last] = elem;
    // 如果clear之后，虽然没有销毁空间，但是first还是会变成-1，需要还原
    if (first == -1)
        ++first;
}

template <typename T>
void vectorForDefault<T>::resize(int n) {
    // If the given value of n is less than the size at present then extra elements are demolished.
    // If n is more than current size of container then upcoming elements are appended at the end of the vector.
    // 自己上一个版本的resize明显是写错了
    if (n < 0) {
        cout << "out of boundry" << endl;
        exit(-1);
    } else if (size <= n & size > 0) {
        if (cap >= n) {
            for (int i = last + 1; i < n; i++) {
                T x;
                warehouse[i] = x;
            }
        }
        else {
            re_construct(n);
        }
        last = n - 1;
    } else if (n == 0) {
        first = -1;
        last = -1;
    } else {
        // n < size
        last = last - (n - size);
    }
    size = n;
}

template <typename T>
T& vectorForDefault<T>::operator[](int idx) const {
    if (idx >= size) {
        cout << "out of boundry" << endl;
        exit(-1);
    }
    return this->getWarehouse()[idx];
}

template <typename T>
void vectorForDefault<T>::insert(T* idx, T val) {
    // 先判断idx是否越界
    // 先结合idx的地址，判断出是T[]中的下标位置
//    cout << idx << endl;
//    cout << warehouse << endl;
//    cout << idx - warehouse << endl;
    int pos = (int)(idx - warehouse);
    if (pos < 0 || pos > size) {
        cout << "out of boundry" << endl;
        exit(-1);
    }
    // 插入元素
    // 先判断是否还能插入元素，又cap和size决定
    if (size == cap) {
        // 先扩容，再插入新元素
        re_construct();
    }
    // 直接插入元素
    // 采用原地搬迁数据
    // idx类比于一个iterator的地址，往那块地址内容中插入元素，同时它之后包括自己的内容向后迁移
    T temp;
    T prev = warehouse[pos];
    for (int i = pos + 1; i < size + 1; i++) {
        temp = warehouse[i];
        warehouse[i] = prev;
        prev = temp;
    }
    warehouse[pos] = val;
    ++last;
    ++size;
}

template <typename T>
void vectorForDefault<T>::erase(T* idx) {
    // 先判断idx是否越界
    // 先结合idx的地址，判断出是T[]中的下标位置
//    cout << idx << endl;
//    cout << warehouse << endl;
//    cout << idx - warehouse << endl;
    int pos = (int)(idx - warehouse);
    if (pos < 0 || pos > size) {
        cout << "out of boundry" << endl;
        exit(-1);
    }
    // 直接删除元素
    // 采用原地搬迁数据
    // idx类比于一个iterator的地址，往那块地址内容中插入元素，同时它之后的内容向前迁移
    for (int i = pos; i < size - 1; i++) {
        warehouse[i] = warehouse[i + 1];
    }
    --last;
    --size;
}

template <typename T>
void vectorForDefault<T>::clear() {
    // 清空元素，不销毁空间
    size = 0;
    last = -1;
    first = -1;
}
