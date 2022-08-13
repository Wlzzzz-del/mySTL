# My_STL  
参考侯捷《STL源码剖析》实现的STL版本, 其中包含部分笔记.
# 项目启动-结束日期
2022-2-11 ———— 至今
# direction
## code_in_book
defalloc.h 仅包含对new、delete的简单封装，仅适用于旧式容器。
2jjallocator.h 包含一个简单的空间配置器，若干接口，但缺少部分接口
## code_w_bymysf
该部分代码为自己实现。
+ myallocator.h 对new\delete进行一层简单的封装
+ deque.h\deque.cpp 实现了双向队列
+ mylist.h 实现了链表
+ vector.h\vector.cpp 实现了vector
## note
包含部分对原书的手写笔记
## pic
包含部分课堂的截图