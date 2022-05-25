//
// Created by xiaoyuyu on 2022/5/25.
//
#include <cstdio>
#include <iterator>
#include <thread>
#include <vector>
#include <functional>
#include <numeric>

#ifndef MYCONCURRENCY_MY_ACCUMULATE_HPP
#define MYCONCURRENCY_MY_ACCUMULATE_HPP

template <typename InputIt, typename T>
void my_accumulate(InputIt first, InputIt last, T& init) {
    while (first != last) {
        init = init + *first;
        ++first;
    }
}

template <typename InputIt, typename T>
T my_parallel_accumulate(InputIt first, InputIt last, T init) {
    size_t length = std::distance(first, last);
    if (length == 0) {
        return init;
    }
    size_t min_per_thread = 5;
    size_t max_threads = length / min_per_thread + 1;
    size_t max_hardware_threads = std::thread::hardware_concurrency();
    size_t num_threads = std::min(max_hardware_threads, max_threads);
    size_t block_size = length / num_threads;

    std::vector<T> result(num_threads, 0);  // 存放每个thread的运行结果
    std::vector<std::thread> threads(num_threads);  // 存放执行的thread

    // start to split
    InputIt block_start = first;
    for (int i = 0; i < num_threads - 1; i++) { // 最后一块block_size是不满的，要单独处理
        InputIt block_last = block_start;
        std::advance(block_last, block_size);   // 移动last指向的位置
        threads[i] = std::thread(my_accumulate<InputIt, T>, block_start, block_last, std::ref(result[i]));
        block_start = block_last;
    }
    threads[num_threads - 1] = std::thread(my_accumulate<InputIt, T>, block_start, last, std::ref(result[num_threads - 1]));
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(result.begin(), result.end(), init);
}


#endif //MYCONCURRENCY_MY_ACCUMULATE_HPP
