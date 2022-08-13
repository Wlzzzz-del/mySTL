#ifndef _STACK_H_
#define _STACK_H_
#include "deque.h"

template<class T,class container = deque<T>>
class stack{
    public:
    typedef size_t size_type;
    typedef T value_type; 
    typedef T* pointer;
    typedef T& reference;
    typedef int difference_type;
    stack();
    ~stack();
    size_type size(){
        return cptr->size();
    }
    void push(value_type _val){
        cptr->push_back(_val);
    }
    void pop(){
        cptr->pop_back();
    }
    value_type top(){
        return *(cptr->begin());
    }
    void clear(){
        cptr->clear();
    }

    private:
    container* cptr; 
};

template<class T,class container>
stack<T,container>::stack(){
    // 没有默认ctor
    cptr = new container();
}
template<class T,class container>
stack<T, container>::~stack(){
    delete cptr;
}

#endif