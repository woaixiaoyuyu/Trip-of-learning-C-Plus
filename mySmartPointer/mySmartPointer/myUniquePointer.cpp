//
//  myUniquePointer.cpp
//  mySmartPointer
//
//  Created by xiaoyuyu on 2022/4/29.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <functional>

template <typename T, class Deleter = std::default_delete<T>>
class u_ptr {
public:
    T* ptr = nullptr;
    Deleter deleter;   // 存储自定义的删除器
public:
    T& operator*() {
        return *(ptr);
    }
    
    T* operator->() {
        return ptr;
    }
    
    u_ptr() : ptr(nullptr){
        
    }
    
    u_ptr(T* obj) : ptr(obj) {
    }
    
    u_ptr(T* obj, Deleter f) : ptr(obj), deleter(f){
    }
    
    u_ptr(u_ptr& p) = delete;
    
    u_ptr& operator=(const u_ptr& p) = delete;
    
    u_ptr(u_ptr&& p) {
        ptr = p.ptr;
        p.ptr = nullptr;
    }
    
    u_ptr& operator=(const u_ptr&& p) {
        ptr = p.ptr;
        p.ptr = nullptr;
    }
    
    ~u_ptr() {
        if (ptr != nullptr) {
            deleter(ptr);
        }
    }
    
    // 返回指向被管理对象的指针，如果无被管理对象，则为 nullptr。
    T* get() {
        return operator->();
    }
};


template <typename T>
class u_ptr<T[]> {
public:
    T* ptr = nullptr;
public:
    T& operator*() {
        return *(ptr);
    }
    
    T* operator->() {
        return ptr;
    }
    
    T& operator[](int idx) {
        if (idx < 0) {
            throw std::logic_error("can't access idx lower than zero.");
        }
        return ptr[idx];
    }
    
    u_ptr() : ptr(nullptr){
        
    }
    
    u_ptr(T* obj) : ptr(obj){
    }
    
    u_ptr(u_ptr& p) = delete;
    
    u_ptr& operator=(const u_ptr& p) = delete;
    
    u_ptr(u_ptr&& p) {
        ptr = p.ptr;
        p.ptr = nullptr;
    }
    
    u_ptr& operator=(const u_ptr&& p) {
        ptr = p.ptr;
        p.ptr = nullptr;
    }
    
    ~u_ptr() {
        if (ptr != nullptr) {
            delete[] ptr;
        }
        ptr = nullptr;
    }
    
    // 返回指向被管理对象的指针，如果无被管理对象，则为 nullptr。
    T* get() {
        return operator->();
    }
};

namespace detail {
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
std::enable_if_t<!std::is_array<T>::value, u_ptr<T>>
make_uptr(Args&&... args) {
    return u_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
begin with C++14
template< class T >
using remove_all_extents_t = typename remove_extent<T>::type;
*/

// 对于数组
template<class T>
std::enable_if_t<detail::is_unbounded_array_v<T>, u_ptr<T>>
make_uptr(std::size_t n) {
    return u_ptr<T>(new std::remove_all_extents_t<T>[n]());
}
