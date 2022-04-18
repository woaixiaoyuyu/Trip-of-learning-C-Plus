//
//  ObjectPoolPattern.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 对象池模式经典版

#include <stdio.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <functional>

const int MaxObjectNum = 10;

template <typename T>
class ObjectPool {
    template <typename... Args>
    using Constructor = std::function<std::shared_ptr<T>(Args...)>;
private:
    std::multimap<std::string, std::shared_ptr<T>> object_map;
    bool needClear;
public:
    ObjectPool() : needClear(false) {}
    ~ObjectPool() {
        needClear = true;
    }
    
    // 默认创建多少个对象
    template <typename... Args>
    void Init(size_t num, Args&&... args) {
        if (num <= 0 || num > MaxObjectNum) {
            throw std::logic_error("object num out of range");
        }
        auto constructName = typeid(Constructor<Args...>).name();   // 不区分引用
        for (size_t i = 0; i < num; i++) {
            object_map.emplace(constructName,
                               std::shared_ptr<T>(
                                             new T(std::forward<Args>(args)...),
                                             [=](T* p) {
                                                // 删除器中不删除对象，而是回收到对象池中，以供下次使用
                                                createPtr<T>(std::string(constructName), args...);
                                            }
            ));
        }
    }
    
    template <typename TT, typename... Args>
    void createPtr(std::string& constructName, Args... args) {
        // 设置了自定义的删除器
        std::shared_ptr<TT>(new T(args...), [constructName, this](TT* t) {
            if (needClear) {
                delete[] t;
            } else {
                object_map.emplace(constructName, std::shared_ptr<TT>(t));
            }
        });
    }
    
    // 从对象池中获取一个对象
    template<typename... Args>
    std::shared_ptr<T> Get() {
        std::string constructName = typeid(Constructor<Args...>).name();
        // 范围以二个迭代器定义，一个指向首个不小于 key 的元素，另一个指向首个大于 key 的元素
        auto range = object_map.equal_range(constructName);
        for (auto it = range.first; it != range.second; it++) {
            auto ptr = it->second;
            object_map.erase(it);
            return ptr;
        }
        
        return nullptr;
    }
};
