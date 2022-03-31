# My_STL  
根据侯捷《STL源码剖析》实现的STL版本, 其中包含部分笔记.
# 项目启动-结束日期
2022-2-11 ———— 至今
# allocator 空间配置器
defalloc.h 仅包含对new、delete的简单封装，仅适用于旧式容器。
2jjallocator.h 包含一个简单的空间配置器，若干接口，但缺少部分接口
