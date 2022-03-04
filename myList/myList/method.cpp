//
//  method.cpp
//  myList
//
//  Created by xiaoyuyu on 2022/3/2.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 声明和定义方法

#include <stdio.h>
#include <iostream>
#include "my_forward_list.h"

// my_forward_list 单链表
// my_list 双向链表
// insert(idx): -- 参数是需要插入的位置
// 1) get(idx) -- 返回对应位置的元素

template <typename T, template<typename> class my_list>
void insert(T val, int idx, my_list<T>& lst) {
    auto& it = lst.get(idx);   // 拿到对应位置的迭代器
    auto temp = lst.get(idx);
    decltype(temp)* new_int = new my_forward_iterator<T>(val);
    // 如果插入在头部,即idx==0
    // 出现这样的问题，主要是一开始没设计好
    if (idx == 0) {
        new_int->_right = &it;
        lst._first = new_int;
    } else {
        auto& prev = lst.prev(idx);   // 类似于--，获取上一个迭代器
        // 执行插入操作
        // my_forward_iterator<T>* new_it = new my_forward_iterator<T>(val);
        prev._right = new_int;    // 暂时用right指代指向的iterator的地址
        new_int->_right = &it;
    }
    // 调整每个iterator的游标
    auto first = lst._first;
    size_t cnt = 0;
    while (first != nullptr) {
        first->_idx = cnt;
        ++cnt;
        first = first->_right;
    }
}

// remove(idx): -- 参数是需要插入的位置
// 1) get(idx) -- 返回对应位置的元素
template <typename T, template<typename> class my_list>
void remove(int idx, my_list<T>& lst) {
    // 如果删除头部元素
    if (idx == 0) {
        auto& next = lst.next(idx);   // 类似于++，获取下一个迭代器
        lst._first = &next;
    } else if (idx == lst.size() - 1) {
        auto& prev = lst.prev(idx);   // 类似于--，获取上一个迭代器
        prev._right = nullptr;
        lst._last = &prev;
    } else {
        auto& prev = lst.prev(idx);   // 类似于--，获取上一个迭代器
        auto& next = lst.next(idx);   // 类似于++，获取下一个迭代器
        prev._right = &next;
    }
    // 调整每个iterator的游标
    auto first = lst._first;
    size_t cnt = 0;
    while (first != nullptr) {
        first->_idx = cnt;
        ++cnt;
        first = first->_right;
    }
}

// for_each: -- 参数需要起始元素的位置;结束元素的位置;每个元素执行的操作 -- 已废弃
// 额外需要实现的接口：
// 1) begin()
// 2) end()
