#include "vector.h"
#include "myallocator.h"

template <class T>
typename vector<T>::pointer vector<T>::fill_initial
(size_type _s,const value_type& elem)
{
    // 带初始值的填充函数
    pointer tmp = allocator::allocate(_s);
    pointer t = tmp;
    pointer end = tmp + _s;

    while(t!=end){
        construct(t,elem);
        t++;
    }
    return tmp;
}

template <class T>
typename vector<T>::pointer vector<T>::fill_cus
(size_type _s)
{
    // 调用默认构造函数的填充函数
    pointer tmp = allocator::allocate(_s);
    pointer t = tmp,end = tmp+_s;
    if(is_trivial<value_type>())return tmp;
    else{
        while(t!=end){
            default_construct(t);
            ++t;
        }
    }
    return tmp;
}

template<class T>
void vector<T>::fill_destruct(pointer _start, pointer _end)
{
    // 批量析构
    if(is_trivial<value_type>())return;
    else{
        pointer tmp = _start;
        while(tmp!=_end){
            destroy(&*tmp);
        // ++tmp妥当吗?是不是应该tmp+=sizeof(value_type)
        // 指针指向的是一个内存块，偏移以一个字节为单位偏移
            ++tmp;
        }
        return;
    }

}

template<class T>
void vector<T>::construct(iterator ite, const value_type& _s){
    // 判断T类型是否为trivial，是的话直接通过反取指针赋值
    // 否则通过new构造
    if(is_trivial<value_type>()){
        *ite = _s;
    }
    else{
        new(ite) (T)(_s);
    }
}

template<class T>
void vector<T>::default_construct(iterator ite){
    new(ite) (T)();
}

template <class T>
void vector<T>::destroy(iterator p){
    // 调用元素的析构函数
    p->~T();
}

template <class T>
vector<T>::vector(){
    start = fill_cus(1);
    finish = start;
    end_of_storage = finish;
}

template <class T>
vector<T>::vector(size_type _s, const value_type& elem){
    start = fill_initial(_s,elem);
    finish = start+_s;
    end_of_storage = finish;
    // 这边结束怎么就调用了一次析构函数？
    // 为了使函数调用成功而进行隐式类型转换
    // elem 被从10转成了一个Node临时对象，所以这边出函数时会多调用一次析构
}

/*
template <class T>
vector<T>::vector(long _s, const value_type& elem){
    start = fill_initial(_s,elem);
    finish = start+_s;
    end_of_storage = finish;
}
*/

template <class T>
vector<T>::vector(size_type _s)
{
    start = fill_cus(_s);
    finish = start;
    end_of_storage = start+_s;
}

template <class T>
vector<T>::~vector(){
    fill_destruct(start,finish);
    allocator::deallocate(start, sizeof(value_type));
}

template <class T>
void vector<T>::push_back(value_type _s){
    if(finish != end_of_storage){// 如果还有空间直接构造
        construct(finish, _s);
        ++finish;
    }
    else{// 否则调用辅助函数
        insert_aux(finish, _s);
    }
}

template <class T>
void vector<T>::insert_aux(iterator _pos, value_type _s){
    // 为什么在insert_aux里面还要再判断一次finish和endofstorage
    // 因为insert_aux不止被push_back调用，还被insert调用
    if(finish!=end_of_storage){
        // 考虑insert的分支
        value_type v_tmp = *_pos;
        value_type mid;
        destroy(_pos);
        construct(_pos,_s);
        ++_pos;
        // 循环赋值
        while(_pos != finish){
            mid = *_pos;
            destroy(_pos);
            construct(_pos, v_tmp);
            ++_pos;
            v_tmp = mid;
        }
        construct(finish, mid);
        ++finish;
    }
    else{
        // 扩充内存
        size_type old_size = this->size();
        size_type size = old_size==0? 1:2*old_size;
        // 新内存的头指针
        iterator new_begin = allocator::allocate(size);
        iterator tmp = new_begin;
        iterator old_start = start;
        // 要分两种情况，一种pushback一种insert
        copy(tmp, old_start, _pos);
        construct(tmp++, _s);
        // old_start
        copy(tmp, old_start, finish);

        allocator::deallocate(start,old_size);
        start = new_begin;
        end_of_storage = new_begin+size;
        finish = new_begin+old_size+1;
    }
}

template <class T>
typename vector<T>::iterator vector<T>::copy
(iterator& _new, iterator& o_start, const iterator& o_end){
    while(o_start != o_end){
            // 一边赋值给新内存，一边销毁对象
        construct(_new, *o_start);
        destroy(o_start);
        ++o_start;
        ++_new;
    }
    return _new;
}

template <class T>
void vector<T>::insert(iterator _pos, value_type _v){
    insert_aux(_pos, _v);
}

template <class T>
void vector<T>::pop_back(){
    --finish;
    destroy(finish);
}

template <class T>
void vector<T>::clear(){
    fill_destruct(start,finish);
}

template <class T>
void vector<T>::erase(iterator _begin,iterator _end){
// 删除区间[_begin,_end)上的元素
    fill_destruct(_begin, _end);
    copy(_begin,_end,finish);
    finish -=(_end-_begin);
}