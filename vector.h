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

    pointer fillinitial(size_type _s, const value_type& elem){
        pointer tmp = allocator::allocate(_s);
        pointer t = tmp;
        pointer end = tmp + _s;

        while(t!=end){
           if(is_trivial<value_type>())
           *t = elem;
           else
           new(t) (T)(elem);
           t++;
        }
        return tmp;
    }
    vector(){
    }
    vector(size_type _s, const value_type& elem){
        start = fillinitial(_s,elem);
        finish = start+_s;
        end_of_storage = start+_s;
    }
    vector(size_type _s){fillinitial(_s,NULL);}




    private:
    pointer start;
    pointer finish;
    pointer end_of_storage;
};