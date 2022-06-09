//
// Created by xiaoyuyu on 2022/6/9.
//

#include <vector>
#include <thread>
#include <functional>
#include <future>
#include "threadsafe_queue.hpp"
#include <memory>

#ifndef MYCONCURRENCY_THREAD_POOL_BASIC_H
#define MYCONCURRENCY_THREAD_POOL_BASIC_H

/*
 * 一个便于集中等待线程执行完毕的聚合类
 * 利用了析构函数
 * */
class join_threads {
private:
    std::vector<std::thread>& threads;
public:
    join_threads(std::vector<std::thread>& _threads) : threads(_threads) {}
    ~join_threads() {
        size_t nums = threads.size();
        for (int i = 0; i < nums; i++) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
};

/*
 * 一个最基础的线程池，只能执行void()类型函数
 * 而且只能执行固定数量的任务，使用后的线程不可以循环利用
 * 另一个缺陷非常明显，使用thread，无法接受线程执行的返回值
 * */
class thread_pool_basic {
private:
    std::atomic_bool done;  // 作为判断异常的标志
    threadsafe_queue<std::function<void()>> queue;  // 存放执行的任务
    std::vector<std::thread> threads;   // 存放待执行的线程
    join_threads join;  // 便于让线程集中join，偷个懒
    // class调用析构函数时，先声明的成员后销毁，所以要注意成员的声明顺序
public:
    void worker_thread() {
        while (!done) {
            std::function<void()> task;
            if(queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();  // 等待执行，让线程先休息
            }
        }
    }

    thread_pool_basic() : done(false), join(threads) {
        size_t threads_count = std::thread::hardware_concurrency();
        try {
            for (int i = 0; i < threads_count; i++) {
                threads.emplace_back(std::thread(&thread_pool_basic::worker_thread, this));
            }
        } catch(...) {
            done = true;
            throw;
        }
    }

    template <typename FunctionType>
    void submit(FunctionType f) {
        queue.push(std::function<void()>(f));
    }
};

/*
 * 对函数的一个封装类，可移动，不可拷贝
 * */

class function_wrapper {
private:
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };

    template<typename F>
    struct impl_type : impl_base {
        F f;
        impl_type(F&& f_): f(std::move(f_)) {}
        void call() {
            f();
        }
    };
    std::unique_ptr<impl_base> impl;
public:
    template<typename F>
    function_wrapper(F&& f) : impl(new impl_type<F>(std::move(f))) {}

    void operator()() { impl->call(); }

    function_wrapper() = default;

    function_wrapper(function_wrapper&& other):
            impl(std::move(other.impl))
    {}

    function_wrapper& operator=(function_wrapper&& other)
    {
        impl=std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&)=delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;
};

/*
 * 一个最中级版本的线程池，能执行各种类型函数
 * 只能执行固定数量的任务，使用后的线程不可以循环利用
 * 利用async/future/packaged_task接收线程的返回值
 * 用packaged_task封装需要执行的任务
 * 因为std::function需要存储可复制构造的函数对象，而packaged_task是需要移动，所以要自己写一个对函数的封装
 * */
class thread_pool_mid {
private:
    std::atomic_bool done;  // 作为判断异常的标志
    threadsafe_queue<function_wrapper> queue;  // 存放执行的任务
    std::vector<std::thread> threads;   // 存放待执行的线程
    join_threads join;  // 便于让线程集中join，偷个懒
    // class调用析构函数时，先声明的成员后销毁，所以要注意成员的声明顺序
public:
    void worker_thread() {
        while (!done) {
            function_wrapper task;
            if(queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();  // 等待执行，让线程先休息
            }
        }
    }

    thread_pool_mid() : done(false), join(threads) {
        size_t threads_count = std::thread::hardware_concurrency();
        try {
            for (int i = 0; i < threads_count; i++) {
                threads.emplace_back(std::thread(&thread_pool_mid::worker_thread, this));
            }
        } catch(...) {
            done = true;
            throw;
        }
    }

    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        using result_type = typename std::result_of<FunctionType()>::type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res = task.get_future();
        queue.push(std::move(task));
        return res;
    }
};
#endif //MYCONCURRENCY_THREAD_POOL_BASIC_H
