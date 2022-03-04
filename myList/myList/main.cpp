//
//  main.cpp
//  myList
//
//  Created by xiaoyuyu on 2022/3/2.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 想要设计的内容：
// 想实现同时满足forward list和list都可以使用的insert单个元素/remove单个元素/for_each方法

// 梳理一下思路：
// 首先要考虑使用这三个方法，需要对应的类实现哪些接口
// insert(idx): -- 参数是需要插入的位置
// 1) get(idx) -- 返回对应位置的元素
// remove(idx): -- 参数是需要插入的位置
// 1) get(idx) -- 返回对应位置的元素
// for_each: -- 参数需要起始元素的位置;结束元素的位置;每个元素执行的操作
// 额外需要实现的接口：
// 1) begin()
// 2) end()

// 自定义的链表类需要实现的接口
// 1) begin()
// 2) end()
// 3) get(idx) -- 返回对应位置的元素
// 4) size()
// 5) empty()
// 6) prev() -- 返回当前指向迭代器的前一个元素的位置
// 7) next() -- 返回当前指向迭代器的下一个元素的位置

// 链表底层的iterator需要实现的接口
// 3) operator*() -- 获取值
// 4) operator->() -- 和operator*()对应

#include <iostream>
#include "method.cpp"
using namespace std;

void test_forward() {
    my_forward_list<int> lst;
    cout << "empty or not: " << lst.empty() << endl;
    cout << "size is: " << lst.size() << endl;
    lst.push_back(3);
    lst.push_back(11);
    cout << "empty or not: " << lst.empty() << endl;
    cout << "size is: " << lst.size() << endl;
    auto it = lst.begin();
    auto it1 = lst.get(1);
    cout << "lst 第0个元素是: " << *it << endl;
    cout << "lst 第1个元素是: " << *it1 << endl;
    insert(100, 1, lst);
    insert(200, 0, lst);
    lst.pop_back();
    cout << "size is: " << lst.size() << endl;
    remove(2,lst);
    remove(0,lst);
    cout << "size is: " << lst.size() << endl;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    test_forward();
    return 0;
}
