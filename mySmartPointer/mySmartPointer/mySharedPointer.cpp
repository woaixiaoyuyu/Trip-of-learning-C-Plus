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
#include <mutex>

template <typename T, class Deleter = std::default_delete<T>>
struct c_block {
    int cnt = 1;
    Deleter deleter;
    c_block(Deleter deleter) : deleter(deleter) {}
};

template <typename T, class Deleter = std::default_delete<T>>
class s_ptr {
public:
    std::mutex* s_lock = new std::mutex();
    T* ptr = nullptr;
    c_block<T, Deleter>* block = nullptr;  // 控制块
public:
    s_ptr() {
        block = new c_block<T, Deleter>(std::default_delete<T>());
    }
    s_ptr(T* ptr) : ptr(ptr) {
        block = new c_block<T, Deleter>(std::default_delete<T>());
    }
    s_ptr(T* ptr, Deleter deleter) : ptr(ptr) {
        block = new c_block<T, Deleter>(deleter);
    }
    ~s_ptr() {
        draw_back();
    }
    void draw_back() {
        if (block->cnt == 0) {
            block->deleter(ptr);
            block = nullptr;
            delete s_lock;
            s_lock = nullptr;
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
    void reset() {
        std::lock_guard<std::mutex> locker(*s_lock);
        block->cnt -= 1;
        draw_back();
        ptr = nullptr;
    }
    // 若 ptr 所指向的对象已被占有，则函数通常会导致未定义行为
    // Y 必须是完整类型且可隐式转换为 T
    template <typename Y>
    void reset(Y* ptr) {
        std::lock_guard<std::mutex> locker(*s_lock);
        if (!std::is_convertible<Y*, T*>::value) {
            throw std::logic_error("can't convert those type.\n");
        }
        block->cnt -= 1;
        draw_back();
        this->ptr = ptr;
        block->cnt = 1;
        s_lock = ptr->s_lock;
    }
    int use_count() {
        return block->cnt;
    }
    s_ptr(const s_ptr& p) {
        std::lock_guard<std::mutex> locker(*s_lock);
        ptr = p.ptr;
        block = p.block;
        block->cnt += 1;
        s_lock = p.s_lock;
    }
    s_ptr& operator=(const s_ptr& p) {
        std::lock_guard<std::mutex> locker(*s_lock);
        ptr = p.ptr;
        block = p.block;
        block->cnt += 1;
        s_lock = p.s_lock;
    }
    s_ptr(s_ptr&& p) {
        std::lock_guard<std::mutex> locker(*s_lock);
        block->cnt -= 1;
        draw_back();
        ptr = p.ptr;
        block = p.block;
        s_lock = p.s_lock;
        
        p.ptr = nullptr;
        p.block = nullptr;
        p.s_lock = nullptr;
    }
    s_ptr& operator=(s_ptr&& p) {
        std::lock_guard<std::mutex> locker(*s_lock);
        block->cnt -= 1;
        draw_back();
        ptr = p.ptr;
        block = p.block;
        s_lock = p.s_lock;
        
        p.ptr = nullptr;
        p.block = nullptr;
        p.s_lock = nullptr;
    }
    
};

// 数组
template <typename T, class Deleter>
class s_ptr<T[], Deleter> {
public:
    T* ptr = nullptr;
    c_block<T, Deleter>* block;  // 控制块
public:
    s_ptr() {
        block = new c_block<T, Deleter>(std::default_delete<T>());
    }
    s_ptr(T* ptr) : ptr(ptr) {
        block = new c_block<T, Deleter>(std::default_delete<T>());
    }
    s_ptr(T* ptr, Deleter deleter) : ptr(ptr) {
        block = new c_block<T, Deleter>(deleter);
    }
    ~s_ptr() {
        draw_back();
    }
    void draw_back() {
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
            throw std::logic_error("can't access idx lower than zero.\n");
        }
        return ptr[idx];
    }
    T* get() {
        return operator->();
    }
    void reset() {
        block->cnt -= 1;
        draw_back();
        ptr = nullptr;
    }
    // 若 ptr 所指向的对象已被占有，则函数通常会导致未定义行为
    // Y 必须是完整类型且可隐式转换为 T
    template <typename Y>
    void reset(Y* ptr) {
        if (!std::is_convertible<Y*, T*>::value) {
            throw std::logic_error("can't convert those type.\n");
        }
        block->cnt -= 1;
        draw_back();
        this->ptr = ptr;
        block->cnt = 1;
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
    s_ptr(s_ptr&& p) {
        block->cnt -= 1;
        draw_back();
        ptr = p.ptr;
        block = p.block;
        
        p.ptr = nullptr;
        p.block = nullptr;
    }
    s_ptr& operator=(s_ptr&& p) {
        block->cnt -= 1;
        draw_back();
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
make_sptr(Args&&... args) {
    return s_ptr<T>(new T(std::forward<Args>(args)...));
}

// 对于数组
template<class T>
std::enable_if_t<detail2::is_unbounded_array_v<T>, s_ptr<T>>
make_sptr(std::size_t n) {
    return s_ptr<T>(new std::remove_all_extents_t<T>[n]());
}
