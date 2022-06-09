# Trip-of-learning-C-Plus
A collection of some projects during learning c++.

## myVector

学完基础语法后，做的第一个小项目，模拟了一个vector，底层用数组实现。

可以练习基础的class，友元，重载，模版类，模版特化等方法。

## myDB

想做一个简单的data base，参考链接：https://cstack.github.io/db_tutorial/

更多是了解一下数据库具体有哪些组件，相关概念还需要研读很多书籍才行。

## myList

之前实现过一个底层是数组的非常simple的vector，这次更进一步。

需要完成的目标：

- 编写能同时满足单链表和双链表容器的insert/remove方法
- 两种容器自己实现
- 底层实现对应的iterator

当然最后没有全部完成，也是由于一开始顶层设计出了问题，哪怕是一个小问题，实现后也遇到了很大的挫折，这就是设计的魅力～

最后只实现了单链表的结构，但是收获还是很大的，一层一层抽丝剥茧一般的设计，经历过了才有收获嘛。

灵感来自于书籍：深入应用c++11的1.4中的自定义的range方法，看了你就知道为什么了，培养自己的设计思路。

## myBTree

学习了B Tree的原理之后，想学习一下别人的代码，删除就不学了，看一下插入和搜寻就不错了，仓库里是programiz和geekforgeek的代码，geek的代码比较好，自己消化了一下。

本B Tree只有遍历，查询和插入功能，最难的删除功能没有实现。

## myTTT(Tic-Tac-Toe)

想做个双人对战的井字游戏，至于原因是因为想学习一下简单的网络编程，用一个双人交互小游戏入手，是个不错的选择。本项目只能在mac/linux上运行。

如果顺利的话，会扩展成五子棋哦～

练手文件：

- xxx_text.cpp/xxx_echo.cpp 是网络编程的基础测试/回声测试
- mp_server_echo.cpp 是多进程的服务器，客户端还是用xxx_echo.cpp就可以了
- select_server_echo.cpp 是select结构的回声服务器

游戏文件：

- simpleTTT_server.cpp 只能双人游戏，不能高并发的服务器版本，只用到了select，可玩
- myTTT系列，目前是个bug，谨慎运行

## myDesignPatterns

参考如下资料，整理的设计模式以及改进：

- 菜鸟
- <<深入应用C++11>>
- <<Design Patterns, Elements of Reusable Object Oriented Software (Gang of Four)>>

代码的介绍文档：https://woaixiaoyuyu.github.io/2022/04/18/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F%E6%94%B6%E9%9B%86/

以后要是有新的收获会进一步补充。

## mySmartPointer

准备实现一下智能指针，便于我进一步理解，心血来潮。

重点放在单个变量上，数组只实现了一小部分。

完成unique_ptr和shared_ptr就算成功，把一些最最最常见的api实现就行，顺便让shared_ptr支持多线程。

## myTestForObjectModel

在阅读了《深入探索C++对象模型》之后，自己也想做个小实验，看看书上说的对不对，温故而知新嘛，还是要自己尝试一下的，顺便还可以学一下gdb的基本操作，一举两得。

代码解读视频：https://www.bilibili.com/video/BV1jB4y197eb?spm_id_from=333.999.0.0

## myConcurrency

关于《C++ Concurrency in Action》的学习，仓库里是一些自己实践的代码案例：

- 实现std::accumulate的并行版（my_accumulate.hpp）
- 线程安全的堆栈类（threadsafe_stack.hpp）
- 简单的层级互斥量实现（hierarchical_mutex.h）
- 线程安全队列的接口，简单使用一下`std::condition_variable`（tlreadsafe_queue.hpp）
- 尝试使用promises自定义packaged_task和async（my_async.hpp）
- 条件变量+超时判断（main.cpp）
- 快排/并发快排（quick_sort.hpp）
- 实现线程安全map（threadsafe_map.hpp）
- 线程池基础版（thread_pool_basic.h）
- 线程池中级版（thread_pool_basic.h）
- 线程池高级版（施工ing）

辅助学习资料：http://shouce.jb51.net/cpp_concurrency_in_action/
