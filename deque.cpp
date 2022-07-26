#include <iostream>
#include "deque.h"
#include "myallocator.h"

/***************deque-iterator实现部分******/
// 为迭代器指向新的buffer
template <class T, size_t buf_size>
void deque_iterator<T, buf_size>::set_new_node(map_pointer new_node){
    buf = new_node;
    first = *new_node;
    cur = first;
    // 与iterator end同理,指向最后一个元素的下一个位置
    last = first + buffer_size();
}

template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::reference
deque_iterator<T, buf_size>::operator*(){
    return *cur;
}

template <class T, size_t buf_size>
typename deque_iterator<T,buf_size>::self& 
deque_iterator<T,buf_size>::operator++(){
    // cur的更新包含在了set_new_node中
    // sgi版本将cur的更新独立出来了
    ++cur;
    if(cur == last){
        set_new_node(buf+1);
    }
    return *this;
}

template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self 
deque_iterator<T,buf_size>::operator++(int){
    reference tmp = *this;
    ++(*this);
    return tmp;
}


template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self&
deque_iterator<T, buf_size>::operator--(){
    if(cur == first){
        set_new_node(buf-1);
    }
    else{
        --cur;
    }
    return (*this);
}

template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self
deque_iterator<T, buf_size>::operator--(int){
    reference tmp = *this;
    -- *this;
    return tmp;
}


// 待验证
// 少考虑了offset小于0的情况
template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self&
deque_iterator<T, buf_size>::operator+=(const int step){
    size_type rest = cur-first;
    different_type offset = step+(cur-first);
    if(offset>=0 && offset <= different_type(buffer_size())){
        cur = first + offset;
    }
    else{
        different_type buf_offset = offset>0?
        offset/different_type(buffer_size()): -different_type((-offset-1)/buffer_size())-1;
        set_new_node(buf+buf_offset);
        cur = first + (offset - buf_offset*different_type(buffer_size()));
    }
    return *this;
}

// 待验证
// operator -=调用operator +=
template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self&
deque_iterator<T, buf_size>::operator-=(const int step){
    return *this +=(-step);
}

template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self
deque_iterator<T, buf_size>::operator+(const int step) const{
    pointer tmp = *this;
    return tmp+=step;
}

template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::self
deque_iterator<T, buf_size>::operator-(const int step) const{
    pointer tmp = *this;
    return tmp-=step;
}

// (buf-x.buf-1) 计算出两个迭代器相差多少块buffer
// (cur-first) 为当前buffer块被占用的部分
// (x.last-x.cur) 为最后一块buffer被占用的部分
template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::different_type
deque_iterator<T, buf_size>::operator-(const self x)const{
    return different_type(buffer_size()*(buf-x.buf-1)+(cur-first)
    +(x.last-x.cur));
}

template <class T, size_t buf_size>
bool deque_iterator<T, buf_size>::operator>(const self x) const{
    return this->buf == x.buf?(this->cur<x.cur):(buf<x.buf);
}

template <class T, size_t buf_size>
typename deque_iterator<T, buf_size>::reference
deque_iterator<T, buf_size>::operator[](const int step) const{
    return *(*this+=step);
}




/***************deque实现部分**************/

/*
template <class T, size_t buf_size>
deque<T,buf_size>::deque(){
    map = buffer_allocator.allocate(map_size);// 分配mapcenter
}
*/
/*
template <class T, size_t buf_size>
typename deque<T,buf_size>::map_pointer 
deque<T,buf_size>::create_mapnode(){
    map = buffer_allocator::allocate(map_size);
    for(map_pointer cur = map; map<map+map_size; ++cur){
        *map = allocator::allocate(buf_size);
    }
    return map;
}
*/

template <class T, size_t buf_size>
typename deque<T, buf_size>::size_type 
deque<T,buf_size>::initial_map_size(){
    // 一个buffer-center至少包含八个节点
    return size_type(8);
}

template <class T, size_t buf_size>
deque<T,buf_size>::deque(size_type n, value_type val){
    // 需要一个确定mapsize 的方法
    // 需要节点数 = (num of element)/每个缓冲区可容纳的元素个数+1
    size_type num_of_node = (n/buffer_size())+1;
    // 一个map需要管理几个节点，前后各需要预留一个，故(num_of_node+2)
    // 至少需要8个
    map_size = max(initial_map_size(),num_of_node+2);
    map = buffer_allocator::allocate(map_size);
    map_pointer cur;
    for(cur = map; cur<map+map_size; ++cur){
        *map = allocator::allocate(buffer_size());
    }
    int num=0;// 记录初始化的元素个数
    for(cur = map+1; cur<map+map_size-1; ++cur){
        // 因为前后各预留了一个缓冲区,所以注意区间范围
        for(pointer bcur = *cur; bcur<buffer_size()+*cur&&num<n; ++bcur){
            construct(bcur, val);
            ++num;
        }
    }
    // 还需要一步更新start和finish
}