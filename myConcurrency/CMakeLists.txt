cmake_minimum_required(VERSION 3.22)
project(myConcurrency)

set(CMAKE_CXX_STANDARD 17)

add_executable(myConcurrency main.cpp my_accumulate.hpp threadsafe_stack.hpp hierarchical_mutex.cpp hierarchical_mutex.h threadsafe_queue.hpp my_async.hpp quick_sort.hpp threadsafe_map.hpp thread_pool_basic.h)
