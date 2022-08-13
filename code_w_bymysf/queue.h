#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "deque.h"

template <class T,class container = deque<T>>
class queue{
    public:
    typedef int difference_type;

    typedef T value_type;

    typedef T* pointer;

    typedef T& reference;

    typedef size_t size_type;

    void push(value_type _val){
        cptr->push_front(_val);
    }

    void pop(){
        cptr->pop_back();
    }

    size_type size(){
        return cptr->size();
    }

    void clear(){
        cptr->clear();
    }

    queue();

    ~queue();

    private:

    container* cptr;

};

template <class T,class container>
queue<T,container>::queue(){
    cptr = new container();
}

template <class T, class container>
queue<T,container>::~queue(){
    delete cptr;
}

#endif