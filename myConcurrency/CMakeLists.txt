cmake_minimum_required(VERSION 3.22)
project(myConcurrency)

set(CMAKE_CXX_STANDARD 14)

add_executable(myConcurrency main.cpp my_accumulate.hpp threadsafe_stack.hpp)
