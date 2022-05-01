//
//  mySharedPointer.cpp
//  mySmartPointer
//
//  Created by xiaoyuyu on 2022/5/1.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <functional>

template <typename T, class Deleter = std::default_delete<T>>
struct c_block {
    int cnt = 1;
    Deleter deleter;
};

template <typename T, class Deleter = std::default_delete<T>>
class s_ptr {
public:
    T* ptr = nullptr;
    c_block<T, Deleter>* block;  // 控制块
public:
    s_ptr() {}
    s_ptr(T* ptr) : ptr(ptr) {}
    s_ptr(T* ptr, Deleter deleter) : ptr(ptr) {
        block->deleter = deleter;
    }
    ~s_ptr() {
        if (block->cnt == 0) {
            block->deleter(ptr);
            block = nullptr;
        }
    }
    T& operator*() {
        return *ptr;
    }
    T* operator->() {
        return ptr;
    }
    T* get() {
        return operator->();
    }
    int use_count() {
        return block->cnt;
    }
    s_ptr(const s_ptr& p) {
        ptr = p.ptr;
        block = p.block;
        block->cnt += 1;
    }
    s_ptr& operator=(const s_ptr& p) {
        ptr = p.ptr;
        block = p.block;
        block->cnt += 1;
    }
    s_ptr(const s_ptr&& p) {
        block->cnt -= 1;
        ~s_ptr();
        ptr = p.ptr;
        block = p.block;
        
        p.ptr = nullptr;
        p.block = nullptr;
    }
    s_ptr& operator=(const s_ptr&& p) {
        block->cnt -= 1;
        ~s_ptr();
        ptr = p.ptr;
        block = p.block;
        
        p.ptr = nullptr;
        p.block = nullptr;
    }
    
};

// 数组
template <typename T, class Deleter>
class s_ptr<T[], Deleter> {
public:
    T* ptr = nullptr;
    c_block<T, Deleter>* block;  // 控制块
public:
    s_ptr() {}
    s_ptr(T* ptr) : ptr(ptr) {}
    s_ptr(T* ptr, Deleter deleter) : ptr(ptr) {
        block->deleter = deleter;
    }
    ~s_ptr() {
        if (block->cnt == 0) {
            block->deleter(ptr);
            block = nullptr;
        }
    }
    T& operator*() {
        return *ptr;
    }
    T* operator->() {
        return ptr;
    }
    T operator[](int idx) {
        if (idx < 0) {
            throw std::logic_error("can't access idx lower than zero.");
        }
        return ptr[idx];
    }
    T* get() {
        return operator->();
    }
    int use_count() {
        return block->cnt;
    }
    s_ptr(const s_ptr& p) {
        ptr = p.ptr;
        block = p.block;
        block->cnt += 1;
    }
    s_ptr& operator=(const s_ptr& p) {
        ptr = p.ptr;
        block = p.block;
        block->cnt += 1;
    }
    s_ptr(const s_ptr&& p) {
        block->cnt -= 1;
        ~s_ptr();
        ptr = p.ptr;
        block = p.block;
        
        p.ptr = nullptr;
        p.block = nullptr;
    }
    s_ptr& operator=(const s_ptr&& p) {
        block->cnt -= 1;
        ~s_ptr();
        ptr = p.ptr;
        block = p.block;
        
        p.ptr = nullptr;
        p.block = nullptr;
    }
    
};

namespace detail2 {
template<class>
constexpr bool is_unbounded_array_v = false;
template<class T>
constexpr bool is_unbounded_array_v<T[]> = true;
 
template<class>
constexpr bool is_bounded_array_v = false;
template<class T, std::size_t N>
constexpr bool is_bounded_array_v<T[N]> = true;
} // namespace detail

template<typename T, typename... Args>
std::enable_if_t<!std::is_array<T>::value, s_ptr<T>>
make_uptr(Args&&... args) {
    return s_ptr<T>(new T(std::forward<Args>(args)...));
}

// 对于数组
template<class T>
std::enable_if_t<detail2::is_unbounded_array_v<T>, s_ptr<T>>
make_uptr(std::size_t n) {
    return s_ptr<T>(new std::remove_all_extents_t<T>[n]());
}
