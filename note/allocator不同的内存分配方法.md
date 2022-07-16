# 问题的提出
在编写vector时，自己写的fillinitial(用于在内存块上构造对象)的实现如下:
```C++
pointer fillinitial(size_type _s, const value_type& elem){
        pointer tmp = allocator::allocate(_s);// 申请内存空间
        pointer t = tmp;
        pointer end = tmp + _s;
        while(tmp!=end){
            *tmp = elem;
            ++tmp;
        }
    }
```
一开始只是用int进行测试，因为int是C语言内置的类型，因此用operator=构造对象没什么问题。
但是后来使用std::string测试时，产生了段错误。
原因是此处的赋值方式在string(非C语言内置类型)中不适用，对于这类类型，需要区分出来调用new。

# SGISTL中如何做？
在SGISTL中，函数先判断T类型是否为trivial type？
如果为trivial type调用algorithm头文件中的fill_n对内存进行填充，实现方法与如下一致，为指针地址直接赋值。
```C++
        while(tmp!=end){
            *tmp = elem;
            ++tmp;
        }
```
如果不为tirival type则在每块内存上调用construct函数,
construct函数调用placement new对内存进行构造对象

# 补充知识new、operator new、 placement new
## new
new 是对堆内存中空间的申请和释放，是不能被重载的。
## operator new
operator new可以被重载，实际上new的执行有三个步骤：
1. 申请内存 
2. 调用operator new构造对象 
3. 返回指针

## placement new
placement new是operator new的一个重载版本，placement new允许你在一个已经分配好的内存块上构造一个新的对象。语法如下
```C++
new(内存地址) 对象类型();
// 如下
new(&p) vector(1);
```

# 如何分辨这两个不同类型
使用is_trivial<template _Ty>()判断一个类型是否为trivial类型
```C++
is_trivial<std::string>();// 返回false
is_trivial<int>();// 返回true
```