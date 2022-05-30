//
// Created by xiaoyuyu on 2022/5/30.
//
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

#ifndef MYCONCURRENCY_THREADSAFE_QUEUE_HPP
#define MYCONCURRENCY_THREADSAFE_QUEUE_HPP

template <typename T>
class threadsafe_queue {
private:
    std::condition_variable condition;
    std::queue<T> q;
    std::mutex m;
public:
    threadsafe_queue() : q(std::queue<T>()) {}
    threadsafe_queue(const threadsafe_queue<T>& p) {
        std::lock_guard<std::mutex> lock(m);
        q = p.q;
    }
    threadsafe_queue& operator=(const threadsafe_queue<T>& p) = delete;
    bool empty() {
        std::lock_guard<std::mutex> lock(m);
        return q.empty();
    }
    void push(T elem) {
        std::lock_guard<std::mutex> lock(m);
        q.push(elem);
        condition.notify_one();
    }
    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            throw std::logic_error("can't pop empty queue.\n");
        } else {
            value = q.front();
            q.pop();
        }
    }
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            throw std::logic_error("can't pop empty queue.\n");
        } else {
            std::shared_ptr<T> res = std::make_shared<T>(q.front());
            q.pop();
            return res;
        }
    }
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lock(m);   // 这里必须选择unique_lock，因为锁要定期检查来选择开关
        condition.wait(lock, [this]() -> bool {
            return !q.empty();
        });
        value = q.front();
        q.pop();
    }
    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lock(m);
        condition.wait(lock, [this]() -> bool {
            return !q.empty();
        });
        std::shared_ptr<T> res = std::make_shared<T>(q.front());
        q.pop();
        return res;
    }
};


#endif //MYCONCURRENCY_THREADSAFE_QUEUE_HPP
