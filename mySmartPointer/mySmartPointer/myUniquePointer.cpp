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

// class Deleter = std::default_delete<T>
template <typename T>
class u_ptr {
public:
    T* ptr = nullptr;
public:
    T& operator*() {
        return *(ptr);
    }
    
    T* operator->() {
        return ptr;
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
            delete ptr;
        }
        ptr = nullptr;
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
