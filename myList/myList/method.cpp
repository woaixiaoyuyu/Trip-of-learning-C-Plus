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

// my_forward_list 单链表
// my_list 双向链表
// insert(idx): -- 参数是需要插入的位置
// 1) get(idx) -- 返回对应位置的元素

//template <typename T, typename U>
//void insert(T val, U idx, my_forward_list<T>& lst) {
//    my_iterator it = lst.get(idx);   // 拿到对应位置的迭代器
//    my_iterator prev = it.prev();   // 类似于--，获取上一个迭代器
//    my_iterator next = it.next();   // 类似于++，获得下一个迭代器
//    // 执行插入操作
//    my_iterator new_it = new my_iterator(val);
//    prev.right = new_it;    // 暂时用right指代指向的iterator的地址
//    new_it.right = next;
//}



// remove(idx): -- 参数是需要插入的位置
// 1) get(idx) -- 返回对应位置的元素



// for_each: -- 参数需要起始元素的位置;结束元素的位置;每个元素执行的操作
// 额外需要实现的接口：
// 1) begin()
// 2) end()
