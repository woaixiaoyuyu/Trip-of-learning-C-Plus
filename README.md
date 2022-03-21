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

- xxx_text.cpp/xxx_echo.cpp 是网络编程的基础测试/回声测试
- mp_server_echo.cpp 是多进程的服务器，客户端还是用xxx_echo.cpp就可以了
