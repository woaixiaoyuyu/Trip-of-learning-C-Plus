//
// Created by xiaoyuyu on 2022/5/31.
//

#include <list>
#include <iterator>
#include <thread>
#include <future>

#ifndef MYCONCURRENCY_QUICK_SORT_HPP
#define MYCONCURRENCY_QUICK_SORT_HPP

/*
 * 原文介绍这里用了函数式编程，但我自己的理解里，函数式编程不能有副作用，所以稍微改了一下，
 * 当然空间消耗也更大，但不多，毕竟input改成const &了。
 * */
template <typename T>
std::list<T> quick_sort(const std::list<T>& input) {
    std::list<T> result;
    if (input.size() == 0) {
        return result;
    }
    std::list<T> temp(input.begin(), input.end());
    // 将input的第一个元素移动到result中，作为一开始的判断标志
    result.splice(result.begin(), temp, temp.begin());
    // 利用partition快速分区
    T flag = *result.begin();
    typename std::list<T>::iterator divide_point = std::partition(temp.begin(), temp.end(), [&](const T& elem) -> bool {
       return elem < flag;
    });
    std::list<T> lower(temp.begin(), divide_point);
    std::list<T> higher(divide_point, temp.end());
    result.splice(result.begin(), quick_sort(lower));
    result.splice(result.end(), quick_sort(higher));
    return result;
}

/*
 * 并发快排，用期望接受返回值就可以异步了！！！
 * */
template<typename T>
std::list<T> parallel_quick_sort(const std::list<T>& input) {
    std::list<T> result;
    if (input.size() == 0) {
        return result;
    }
    std::list<T> temp(input.begin(), input.end());
    // 将input的第一个元素移动到result中，作为一开始的判断标志
    result.splice(result.begin(), temp, temp.begin());
    // 利用partition快速分区
    T flag = *result.begin();
    typename std::list<T>::iterator divide_point = std::partition(temp.begin(), temp.end(), [&](const T& elem) -> bool {
        return elem < flag;
    });
    std::list<T> lower(temp.begin(), divide_point);
    std::list<T> higher(divide_point, temp.end());
    std::future<std::list<T>> low = std::async(parallel_quick_sort, std::ref(lower));
    std::future<std::list<T>> high = std::async(parallel_quick_sort, std::ref(higher));
    result.splice(result.begin(), low.get());
    result.splice(result.end(), high.get());
    return result;
}

#endif //MYCONCURRENCY_QUICK_SORT_HPP
