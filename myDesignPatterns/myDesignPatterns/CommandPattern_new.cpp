//
//  CommandPattern_new.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <functional>
#include <type_traits>

// 我直接看了作者的代码，对于C++初学者的我来说，非常不友好，我做了很多注释。

template<typename Ret = void>
struct CommCommand
{
private:
    std::function<Ret()> m_f;

public:
    // 接受可调用对象的函数包装器
    // 接受function、函数对象、lamda和普通函数的包装器
    /**
     判断F是不是member_function_pointer，如果是，获得F的类型，否则无法获得
     只是对F做个判断，如果失败了，函数就无法执行
     */
    template< class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
    void Wrap(F && f, Args && ... args)
    {
        m_f = [&]{return f(args...); };
    }

    // 接受成员函数的包装器
    // 接受常量成员函数的函数包装器
    /**
     R 成员函数的返回值的类型
     C 成员函数属于的类的类型
     DArgs 成员函数的形参
     P 调用函数的实例的类型，和C区分开，是因为可能有多态，编译的时候不会绑定虚函数
     Args 实际调用的函数的形参
     */
    template<class R, class C, class... DArgs, class P, class... Args>
    void Wrap(R(C::*f)(DArgs...) const, P && p, Args && ... args)
    {
        m_f = [&, f]{return (*p.*f)(args...); };
    }

    // 接受成员函数的包装器
    // 接受非常量成员函数的函数包装器
    // 和上面的函数相比少了个const
    template<class R, class C, class... DArgs, class P, class... Args>
    void Wrap(R(C::*f)(DArgs...), P && p, Args && ... args)
    {
        m_f = [&, f]{return (*p.*f)(args...); };
    }

    Ret Excecute()
    {
        return m_f();
    }
};
