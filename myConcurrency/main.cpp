#include <iostream>
#include "my_accumulate.hpp"

void test_my_parallel_accumulate() {
    std::vector<int> v(48, 1);
    int result = my_parallel_accumulate(v.begin(), v.end(), 3);
    std::cout << "result is : " << result << std::endl;
}

int main() {
    /*
     * 多个单线程/进程间的通信(包含启动)要比单一进程中的多线程间的通信(包括启动)的开销大，
     * 若不考虑共享内存可能会带来的问题，多线程将会成为主流语言(包括C++)更青睐的并发途径。
     * 此外，C++标准并未对进程间通信提供任何原生支持，所以使用多进程的方式实现，这会依赖与平台相关的API。
     * 因此，本书只关注使用多线程的并发，并且在此之后所提到“并发”，均假设为多线程来实现。*/
    test_my_parallel_accumulate();
    return 0;
}
