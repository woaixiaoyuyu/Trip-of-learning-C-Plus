//
// Created by xiaoyuyu on 2022/5/27.
//

#include <mutex>
#include <stack>
#include <memory>

#ifndef MYCONCURRENCY_THREADSAFE_STACK_HPP
#define MYCONCURRENCY_THREADSAFE_STACK_HPP

template <typename T>
class threadsafe_stack {
private:
    std::mutex m;
    std::stack<T> s;
public:
    threadsafe_stack() : s(std::stack<T>()) {}
    threadsafe_stack(const threadsafe_stack<T>& p) {
        std::lock_guard<std::mutex> lock(m);    // 上锁，之后会自动释放
        s = p.s;
    }
    threadsafe_stack& operator=(const threadsafe_stack<T>& p) = delete;
public:
    bool empty() {
        std::lock_guard<std::mutex> lock(m);
        return s.empty();
    }
    void push(T elem) {
        std::lock_guard<std::mutex> lock(m);
        s.push(elem);
    }

    int size() {
        std::lock_guard<std::mutex> lock(m);
        return s.size();
    }

    void pop(T& value) {
        // 把弹出的值赋值给value
        std::lock_guard<std::mutex> lock(m);
        if (!s.empty()) {
            T temp = s.top();
            value = temp;
            s.pop();
        } else {
            throw std::logic_error("can't pop empty stack.\n");
        }
    }
    std::shared_ptr<T> pop() {
        // 对于大规模的元素而言，直接返回副本会有大量的空间开销，所以返回指针
        // 并且智能指针能帮助管理内存
        std::lock_guard<std::mutex> lock(m);
        if (!s.empty()) {
            std::shared_ptr<T> p = std::make_shared<T>(s.top());
            s.pop();
            return p;
        } else {
            throw std::logic_error("can't pop empty stack.\n");
        }
    }
};


#endif //MYCONCURRENCY_THREADSAFE_STACK_HPP
