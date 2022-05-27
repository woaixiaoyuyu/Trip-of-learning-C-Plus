//
// Created by xiaoyuyu on 2022/5/27.
//

#ifndef MYCONCURRENCY_HIERARCHICAL_MUTEX_H
#define MYCONCURRENCY_HIERARCHICAL_MUTEX_H

#include <mutex>

// 带层级的锁
class hierarchical_mutex {
private:
    std::mutex internal_mutex;
    static thread_local size_t upper_bound_hierarchy;  // 一个线程生命周期内存在的静态成员
    size_t previous_hierarchy;
    size_t hierarchy;   // 当前锁的层级，数值越小，层级越高，高层级锁存在时，低层级的锁不能构造
public:
    hierarchical_mutex(size_t value) : hierarchy(value), previous_hierarchy(0) {}
public:
    void check_hierarchy() {
        if (hierarchy >= upper_bound_hierarchy) {
            throw std::logic_error("this hierarchy is invalid.\n");
        }
    }
    void lock() {
        check_hierarchy();
        internal_mutex.lock();
        previous_hierarchy = upper_bound_hierarchy;
        upper_bound_hierarchy = hierarchy;
    }
    void unlock() {
        hierarchy = previous_hierarchy;
        upper_bound_hierarchy = hierarchy;
        internal_mutex.unlock();
    }
    bool try_lock() {
        if (!internal_mutex.try_lock()) {
            return false;
        }
        previous_hierarchy = upper_bound_hierarchy;
        upper_bound_hierarchy = hierarchy;
        return true;
    }
};

#endif //MYCONCURRENCY_HIERARCHICAL_MUTEX_H
