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
    +(x.last-x.cur));// 这边计算地址差
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

// buffer-center的最低节点数
template <class T, size_t buf_size>
typename deque<T, buf_size>::size_type 
deque<T,buf_size>::initial_map_size(){
    // 一个buffer-center至少包含八个节点
    return size_type(8);
}
template <class T, size_t buf_size>void
deque<T, buf_size>::uninitialized_fill(pointer _begin,pointer _end,const value_type val){
    // [_begin, _end)
    pointer cur;
    for(cur = _begin; cur<_end; ++cur){
        construct(cur, val);
    }
}

template <class T, size_t buf_size>void 
deque<T, buf_size>::fill_initialized(size_type n,const value_type val){
   create_mapnode(n);
   map_pointer tmp;
   for(tmp= start.buf; tmp<finish.buf; ++tmp){
    uninitialized_fill(*tmp,*tmp+buffer_size(), val);
   }
   uninitialized_fill(finish.first, finish.cur, val);
}

template <class T, size_t buf_size> void 
deque<T ,buf_size>::create_mapnode(size_type n){
    // 负责产生并安排deque结构
    size_type num_of_node = (n/buffer_size())+1;
    // 一个map需要管理几个节点，前后各需要预留一个，故(num_of_node+2)
    // 至少需要8个
    map_size = max(initial_map_size(),num_of_node+2);
    // 分配mapcenter
    map = buffer_allocator::allocate(map_size);
    map_pointer nstart,nfinish,cur;
    // mapcenter不直接为所有buffer分配内存，而是先分配需要初始化的部分
    // nstart 挑选在buffercenter中最中间的位置，前后都预留了可以扩充的余地
    nstart = map+ (map_size-num_of_node)/2;
    nfinish = nstart + num_of_node - 1;

    for(cur = nstart; cur<=nfinish; ++cur){
        *cur = allocator::allocate(buffer_size());
    }
    // 更新deque的start和finish
    start.set_new_node(nstart);
    finish.set_new_node(nfinish);
    finish.cur = finish.first+(n%buffer_size());
}

// deque构造函数
template <class T, size_t buf_size>
deque<T,buf_size>::deque(size_type n, value_type val):map(0),map_size(0),start(),finish(){
    fill_initialized(n,val);
}

// deque析构函数
template <class T, size_t buf_size>
deque<T,buf_size>::~deque(){
    map_pointer tmp = map;
    for(;tmp<map+map_size;++tmp){
        // 先对每个对象析构
        for(pointer bfcur = *tmp;bfcur<bfcur+buffer_size(); ++bfcur){
            destroy(bfcur);
        }
        // 再释放对象数组的内存
        allocator::deallocate(*tmp);
    }
    // 释放mapcenter
    buffer_allocator::deallocate(map);
}


// 元素的构造
template <class T, size_t buf_size>
void deque<T, buf_size>::construct(pointer p,const value_type val){
    if(is_trivial<T>()){
        *p = val;
    }
    else{
        new(p) (T)(val);
    }
}

// 元素的析构
template <class T, size_t buf_size>
void deque<T, buf_size>::destroy(pointer p){
    if(is_trivial<T>())return;
    else p->~T();
}

template <class T, size_t buf_size>
void deque<T, buf_size>::push_back_aux(value_type val){
    // 如果buffer空间不够还需补写一个函数
    // reverse_map_at_back()
    construct(finish.cur, val);
    *(finish.buf+1)=allocator::allocate(buffer_size());
    finish.set_new_node(finish.buf+1);
}

template <class T, size_t buf_size>
void deque<T, buf_size>::push_back(value_type val){
    /* 有几种情况:
    如果当前buffer还有空间能够插入，则直接插入
    如果当前buffer没有空间能够插入, 调用辅助函数aux:
    如果当前buffer不是最后一个buffer，则插入到下一个buffer中
    如果当前buffer是最后一个buffer，则引起deque重新配置空间并插入
    */
   if(finish.cur+1 ==finish.last)push_back_aux(val);
   else{
    construct(finish.cur,val);
    ++finish.cur;
   }
}

template <class T, size_t buf_size>
void deque<T, buf_size>::push_front_aux(value_type val){
    // reverse_map_at_back
    *(start.buf-1) = allocator::allocate(buffer_size());
    start.set_new_node(start.buf-1);
    start.cur = start.last-1;
    construct(start.cur,val);
}

template <class T, size_t buf_size>
void deque<T, buf_size>::push_front(value_type val){
    if(start.cur == start.first)push_front_aux(val);
    else{
        --start.cur;
        construct(start.cur,val);
    }
}

template <class T, size_t buf_size>
void deque<T, buf_size>::pop_back(value_type val){

}

template <class T, size_t buf_size>
void deque<T, buf_size>::pop_front(value_type val){

}

