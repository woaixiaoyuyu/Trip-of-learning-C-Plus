//
//  my_forward_list.cpp
//  myList
//
//  Created by xiaoyuyu on 2022/3/2.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "my_forward_iterator.h"

// 自定义一个单链表

// 自定义的链表类需要实现的接口
// 1) begin()
// 2) end()
// 3) get(idx) -- 返回对应位置的元素
// 4) size()
// 5) empty()
// 6) prev() -- 返回要插入位置的前一个元素的位置
// 7) next() -- 返回要插入位置的下一个元素的位置

template <typename T>
class my_forward_list {
public:
    using value_type = T;
    using iterator = my_forward_iterator<value_type>;
    using const_iterator = const my_forward_iterator<value_type>;
private:
    iterator* _first; // 指向iterator所在链表的第一个iterator
    iterator* _last; // 指向iterator所在链表的最后一个iterator
public:
    my_forward_list(iterator* first = nullptr) : _first(first), _last(nullptr) {
        _last = _first; // 初始化时，指向同一个iterator
    }
    ~my_forward_list() {
        while (_first != _last) {
            iterator* temp = _first;
            _first = _first->_right;
            delete temp;
            temp = nullptr;
        }
        delete _first;
        _first = nullptr;
    }
    my_forward_list(const my_forward_list& p) {
        delete _first;
        delete _last;
        _first = nullptr;
        _last = nullptr;
        iterator* first = p._first;
        iterator* last = p._last;
        _first = &iterator(first->_idx,first->_value,nullptr);
        iterator* temp = _first;
        while (first != last) {
            first = first->_right;
            temp->_right = &iterator(first->_idx,first->_value,nullptr);
            temp = temp->_right;
        }
        _last = &iterator(first->_idx,first->_value,nullptr);
        temp->_right = _last;
    }
    my_forward_list& operator=(const my_forward_list& p) {
        if (this == &p) {
            delete _first;
            delete _last;
            _first = nullptr;
            _last = nullptr;
        }
        this->_first = p._first;
        this->_last = p._last;
        return *this;
    }
    iterator& prev(int idx);
    iterator& next(int idx);
    const_iterator& begin();
    const_iterator& end();
    const_iterator& get(int idx);
    size_t size();
    bool empty();
    void push_back(const_iterator& p);
    void pop_back();
};

template <typename T>
typename my_forward_list<T>::const_iterator& my_forward_list<T>::begin() {
    return *(this->_first);
}

template <typename T>
typename my_forward_list<T>::const_iterator& my_forward_list<T>::end() {
    return *(this->_last);
}

template <typename T>
typename my_forward_list<T>::const_iterator& my_forward_list<T>::get(int idx) {
    // 比较iterator中的下标_idx;
    iterator* temp = _first;
    while (temp->_idx != idx) {
        temp = temp->_right;
    }
    return *temp;
}

template <typename T>
typename my_forward_list<T>::iterator& my_forward_list<T>::prev(int idx) {
    // 从first开始遍历，遍历到需要迭代器的前一个迭代器
    iterator* temp = _first;
    while (temp->_right->_idx != idx) {
        temp = temp->_right;
    }
    return *temp;
}

template <typename T>
typename my_forward_list<T>::iterator& my_forward_list<T>::next(int idx) {
    iterator* temp = _first;
    while (temp->_idx != idx) {
        temp = temp->_right;
    }
    return *(temp->_right);
}

template <typename T>
size_t my_forward_list<T>::size() {
    iterator* first = _first;
    iterator* last = _last;
    size_t cnt = 0;
    if (first == nullptr)
        return cnt;
    while (first != last) {
        first = first->_right;
        cnt++;
    }
    return ++cnt;
}

template <typename T>
bool my_forward_list<T>::empty() {
    if (this->_first == nullptr)
        return true;
    return false;
}

template <typename T>
void my_forward_list<T>::push_back(const_iterator& p) {
    iterator temp = iterator(p);
    if (this->size() == 0) {
        _first = &temp;
        _first->_idx = 0;
        _last = _first;
    } else {
        size_t idx = _last->_idx;
        _last->_right = &temp;
        _last = _last->_right;
        _last->_idx = ++idx;
    }
}

template <typename T>
void my_forward_list<T>::pop_back() {
    if (this->empty()) {
        std::cout << "empty list can't use pop." << std::endl;
        exit(EXIT_FAILURE);
    }
    size_t idx = _last->_idx;
    iterator* temp = prev(idx);
    delete _last->_right;
    _last->_right = nullptr;
    temp->_right = nullptr;
    _last = temp;
}
