#ifndef _DEQUE_H_
#define _DEQUE_H_
#include <iostream>
#include "myallocator.h"
inline size_t deque_bufsize(size_t n, size_t sz){
    return n!=0? n:(sz<512?size_t(512/sz):size_t(1));
}

template <class T,size_t buf_size=0>
class deque_iterator{
    public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef pointer* map_pointer;
    typedef size_t size_type;
    typedef deque_iterator self;
    typedef int different_type;

    // 返回buffer_size
    static size_type buffer_size(){
        return deque_bufsize(buf_size,sizeof(T));
    }

    // 使迭代器指向新的buffer
    void set_new_node(map_pointer new_node);
    reference operator*();

    bool operator==(const self& _it) const {
        return this->cur == _it.cur;
    }

    bool operator!=(const self& _it) const {
        return !(_it==*this);
    }

    // 注意这边
    // 后置返回的是临时对象，所以应是self类型
    // 前++
    self& operator++();

    // 后++
    self operator++(int);

    // 前--
    self& operator--();

    // 后--
    self operator--(int);
    // iterator+=
    self& operator+=(const int step);

    // iterator-=(调用+=来实现)
    self& operator-=(const int step);

    // iterator移动(调用+=来实现)
    self operator+(const int step) const;

    // iterator移动(调用+=来实现)
    self operator-(const int step) const;

    // 计算两个iterator之间的距离
    different_type operator-(const self x)const;

    // 实现随机存取
    reference operator[](const int step) const;

    // 对比两个iterator
    bool operator>(const self x)const;

    public:
    pointer first;
    pointer last;
    pointer cur;
    map_pointer buf;
};

template <class T, size_t buf_size=0>
class deque{
    public:
    typedef int different_type;
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef size_t diff_type;
    typedef size_t size_type;
    typedef const pointer const_ptr;
    typedef const reference ref_ptr;
    typedef pointer* map_pointer;

    // 定义iterator的类型
    typedef deque_iterator<T,buf_size> iterator;// iterator还需要重新定义

    // 构造函数
    deque(size_type n,value_type val);

    // 析构函数
    ~deque();

    // 返回指向首个元素的iterator
    iterator begin(){return start;}

    // 返回指向最后一个元素的下一个位置的iterator
    iterator end(){return finish;}

    // 返回deuqe的容量
    size_type cpacity(){return (size_type)buffer_size()*map_size;}

    // 返回deque中当前已存元素个数
    different_type size(){return different_type(finish-start);}

    // 返回deque是否为空
    bool empty(){return finish==start;}

    // 实现deque的随机存取(背后是调用iterator的[])
    reference operator[](const int step){return *(start+=step);}

    // 返回首个元素的引用值
    reference front(){return *start;}

    // 返回最后一个元素的引用值
    reference back(){
        iterator ite = finish-1;
        return *ite;
    }

    void push_back(value_type val);
    void push_front(value_type val);
    void pop_back(value_type val);
    void pop_front(value_type val);

    private:
    typedef aloc::allocator<T> allocator;

    typedef aloc::allocator<pointer> buffer_allocator;

    void fill_initial(size_type n, value_type val);

    size_type initial_map_size();

    // 元素的构造和析构
    void construct(pointer p,const value_type val);

    void destroy(pointer p);

    void create_mapnode(size_type n);

    void uninitialized_fill(pointer _begin,pointer _end,const value_type val);
    void fill_initialized(size_type n,const value_type val);

    static size_type buffer_size(){
        return deque_bufsize(buf_size,sizeof(T));
    }

    void push_back_aux(value_type val);
    void push_front_aux(value_type val);

    protected:
    map_pointer map;// buffer-center
    size_type map_size;// buffer个数
    iterator start;
    iterator finish;
};


#endif