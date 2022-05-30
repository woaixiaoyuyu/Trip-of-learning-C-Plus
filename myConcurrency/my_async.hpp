//
// Created by xiaoyuyu on 2022/5/30.
//

#include <future>
#include <functional>

#ifndef MYCONCURRENCY_MY_ASYNC_HPP
#define MYCONCURRENCY_MY_ASYNC_HPP

template <typename R, typename ...Args> // R是函数的返回值，以及promise的类型
class my_packaged {
private:
    std::promise<R> p;
    std::function<R(Args...)> f;
public:

    /*
     * F是R<(Args...)>整个function的类型
     * */
    template<typename F>
    my_packaged(F&& f) : f(std::forward<F>(f)) {}

    my_packaged(const my_packaged&) = delete;   // 拷贝构造
    my_packaged& operator=(const my_packaged&) = delete;    // operator=左值

    my_packaged(const my_packaged&& other) : p(std::move(other.p)), f(std::move(other.f)) {}    // 移动构造
    my_packaged& operator=(const my_packaged&& other) {   // operator=右值
        p = std::move(other.p);
        f = std::move(other.f);
        return *this;
    }

    void operator()(Args&&... pa) {
        p.set_value(f(std::forward<Args>(pa)...));
    }

    std::future<R> get_future() {
        return p.get_future();
    }
};

template <typename R, typename ...Args>
std::future<R> my_async(std::function<R(Args)...> f) {
    my_packaged<R, Args...> package(f);
    std::future<R> result = package.get_future();
    std::thread p(std::move(package));
    p.detach();
    return result;
}

#endif //MYCONCURRENCY_MY_ASYNC_HPP
