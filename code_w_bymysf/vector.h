#ifndef _VECTOR_H
#define _VECTOR_H

#include <iostream>
#include "myallocator.h"

template <class T>
class vector{
    public:
    typedef T value_type;
    typedef T* pointer;
    typedef T* iterator;
    typedef T& reference;
    typedef size_t size_type;
    typedef ptrdiff_t different_type;
    typedef aloc::allocator<T> allocator;

private:
    void construct(iterator ite, const value_type& _s);
    void default_construct(iterator ite);
    pointer fill_initial(size_type _s, const value_type& elem);
    pointer fill_cus(size_type _s);
    void fill_destruct(pointer _start, pointer _end);
    void destroy(iterator p);

public:
    vector();
    vector(size_type _s, const value_type& elem);
//    vector(long _s, const value_type& elem);

    vector(size_type _s);

    ~vector();

    pointer begin(){
        return start;
    }

    pointer end(){
        return finish;
    }

    size_type size(){
        return size_type(finish-start);
    }

    size_type capacity(){
        return size_type(end_of_storage-start);
    }

    bool empty(){
        return (finish-start)==0;
    }

/*增加新元素s时，如果超过容量，则扩充至原容量的两倍，如果两倍仍然不足，就扩充至足够大的容量*/
/*容量的扩充必须经历 重新配置、元素移动、释放原空间的操作*/
    void push_back(value_type _s);

    void insert_aux(iterator _pos, value_type _s);

    void insert(iterator _pos, value_type _v);

    void pop_back();

    reference operator[](int _n){
        return (reference)*(begin+_n);
    }
    reference front(){
        return (reference)*begin;
    }

    reference back(){
        return (reference)*(finish-1);
    }

    iterator copy(iterator& _new, iterator& o_start ,const iterator& o_end);

    void clear();

    void erase(iterator _begin, iterator _end);

    private:
    pointer start;
    pointer finish;
    pointer end_of_storage;
};
#endif