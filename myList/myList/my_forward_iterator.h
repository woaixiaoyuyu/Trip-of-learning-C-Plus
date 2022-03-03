//
//  my_forward_iterator.cpp
//  myList
//
//  Created by xiaoyuyu on 2022/3/2.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <iostream>

// 链表底层的iterator需要实现的接口
// 1) operator*() -- 获取值
// 2) operator->() -- 和operator*()对应
// 3) operator++() -- 前缀++
// 4) operator++(int) -- 后缀++

template <typename T>
class my_forward_iterator {
public:
    using value_type = T;
    size_t _idx;    // 链表目前指向的iterator所处在链表中的位置
    value_type _value;
    my_forward_iterator* _right; // 指向下一个iterator
public:
    my_forward_iterator(value_type value = value_type(), size_t idx = 0, my_forward_iterator* right = nullptr) : _idx(idx), _value(value), _right(right) {};
    my_forward_iterator(const my_forward_iterator& p) {
        this->_idx = p._idx;
        this->_value = p._value;
        if (p._right != nullptr)
            this->_right = new my_forward_iterator(*(p._right));
        else _right = nullptr;
    }
    my_forward_iterator& operator=(const my_forward_iterator& p) {
        if (this == &p) {
            return *this;
        }
        this->_idx = p._idx;
        this->_value = p._value;
        this->_right = p._right;
        return *this;
    }
//    my_forward_iterator& operator=(const my_forward_iterator& p) {
//        if (this != &p) {
//            delete this->_right;
//            this->_right = nullptr;
//        }
//        this->_idx = p._idx;
//        this->_value = p._value;
//        this->_right = p._right;
//        return *this;
//    }
    ~my_forward_iterator() {
//        if (_right != nullptr) {
//            delete _right;
//            _right = nullptr;
//        }
    }
    my_forward_iterator& operator++(void);
    my_forward_iterator operator++(int);
    value_type operator*();
    // value_type operator->();
    
};

template <typename T>
my_forward_iterator<T>& my_forward_iterator<T>::operator++() {
    return *(this->_right);
}

template <typename T>
my_forward_iterator<T> my_forward_iterator<T>::operator++(int) {
    my_forward_iterator temp;
    temp = new my_forward_iterator(this->_idx,this->_value, this->_right);
    this = this->_right;
    return temp;
}

template <typename T>
T my_forward_iterator<T>::operator*() {
    return this->_value;
}

/**template <typename T>
T my_forward_iterator<T>::operator->() {
    return this->_value;
}**/
