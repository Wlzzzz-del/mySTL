#include "myallocator.h"
#include <iostream>

template <class T>
class List{
    private:
    struct Node{
        Node* _prev;
        Node* _next;
        T _val;
    };

    private:
    typedef size_t sizetype;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_ptr;
    typedef const T& const_ref;
    typedef T value_type;
    typedef Node* Nodeptr;
    typedef const Node* cNodeptr;
    // 指定内存配置器
    typedef aloc::allocator<Node> allocator;

    Nodeptr Head;
    sizetype Length=0; 

    struct ACC{
        typedef Nodeptr& ref_Nodeptr;
        typedef value_type& ref_val;
        static ref_Nodeptr _Next(Nodeptr _p){
            return (ref_Nodeptr)_p->_next;
        }
        static ref_Nodeptr _Prev(Nodeptr _p){
            return (ref_Nodeptr)_p->_prev;
        }
        static ref_val _Val(Nodeptr _p){
            return (ref_val)_p->_val;
        }
    };

    public:
    struct iterator{
        friend class List;
        private:
        Nodeptr _ptr;
        public:
        iterator(){}
        iterator(Nodeptr ptr):_ptr(ptr){}
        iterator operator++(){
            this->_ptr = ACC::_Next(this->_ptr);
            return *this;
        }
        iterator operator++(int){
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        reference operator*(){
            return (reference)ACC::_Val(this->_ptr);
        }
        iterator operator--(){
            this->_ptr = ACC::_Prev(this->_ptr);
            return *this;
        }
        iterator operator--(int){
            iterator tmp = *this;
            --*this;
            return tmp;
        }
        bool operator==(iterator ite){
            return (this->_ptr==ite._ptr);
        }
        bool operator!=(iterator ite){
            return !(ite==*this);
        }
    };

    struct const_iterator{
        private:
        Nodeptr _ptr;
        public:
        const_iterator(){}
        const_iterator(Nodeptr ptr):_ptr(ptr){}

        const_iterator operator++(){
            this->_ptr = ACC::_Next(this->_ptr);
            return (const_iterator)*this;
        }
        const_iterator operator++(int){
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }
        const_iterator operator--(){
            this->_ptr = ACC::_Prev(this->_ptr);
            return (const_iterator)*this;
        }
        const_iterator operator--(int){
            const_iterator tmp = *this;
            --*this;
            return tmp;
        }
        const_ref operator*(){
            return (const_ref)ACC::_Val(this->_ptr);
        }
        bool operator==(const_iterator ite){
            return (ite._ptr==this->_ptr);
        }
        bool operator!=(const_iterator ite){
            return (ite!=*this);
        }
    };

    List(){
        Head = buy_mem();
        ACC::_Next(Head)=Head;
        ACC::_Prev(Head)=Head;
        ACC::_Val(Head)=0;
    }
    ~List(){
        clear();
        del_mem(Head);
    }
    void show(){
        int count = Length;
        Nodeptr temp = ACC::_Next(Head);
        while(count>0){
            std::cout<<temp->_val<<std::endl;
            temp = ACC::_Next(temp);
            --count;
        }
    }
    void push_back(value_type v){
        Nodeptr temp = buy_mem();
        Nodeptr mid = Head->_prev;
        ACC::_Val(temp)=v;
        ACC::_Prev(Head)=temp;
        ACC::_Prev(temp)=mid;
        ACC::_Next(mid)=temp;
        ACC::_Next(temp)=Head;
        ++Length;
    }

    iterator begin(){
        return ++iterator(Head);
    }
    const_iterator cbegin(){
        return ++const_iterator(Head);
    }
    iterator end(){
        return iterator(Head);
    }
    const_iterator cend(){
        return const_iterator(Head);
    }

// 如何从外部类访问内部类的私有成员
/*
C++ 禁止从外部类访问内部类的私有成员
但是允许内部类访问外部类的私有成员
*/
/*
在内部类声明外部类为友元函数即可
*/
    void insert(iterator _pos, value_type v){
        Nodeptr tmp = buy_mem();
        tmp->_val = v;
        Nodeptr mid = ACC::_Next(_pos._ptr);
        ACC::_Prev(mid) = tmp;
        ACC::_Next(tmp) = mid;
        ACC::_Next(_pos._ptr) = tmp;
        ACC::_Prev(tmp) = _pos._ptr;
        ++Length;
    }

    // 头插法
    void push_front(value_type v){
        Nodeptr tmp = buy_mem();
        ACC::_Val(tmp) = v;
        Nodeptr mid = ACC::_Next(Head);
        ACC::_Next(Head) = tmp;
        ACC::_Prev(tmp) = Head;
        ACC::_Next(tmp) = mid;
        ACC::_Prev(mid) = tmp;
        ++Length;
        return ;
    }

    // 头删法
    void pop_front(){
        Nodeptr tmp = ACC::_Next(Head);
        if(tmp == Head)return;
        Nodeptr mid = ACC::_Next(tmp);
        ACC::_Prev(mid) = Head;
        ACC::_Next(Head) = mid;
        --Length;
        allocator::deallocate(tmp,1);
        return;
    }

    // 尾删法
    void pop_back(){
        Nodeptr tmp = ACC::_Prev(Head);
        if(tmp == Head)return;
        Nodeptr mid = ACC::_Prev(tmp);
        ACC::_Next(mid) = Head;
        ACC::_Prev(Head) = mid;
        --Length;
        allocator::deallocate(tmp,1);
        return;
    }

    void erase(iterator _pos){
        Nodeptr frt = ACC::_Prev(_pos._ptr);
        Nodeptr aft = ACC::_Next(_pos._ptr);
        ACC::_Next(frt) = aft;
        ACC::_Prev(aft) = frt;
        allocator::deallocate(_pos._ptr,1);
        --Length;
    }

    iterator find(value_type v){
        int count = Length;
        Nodeptr temp = ACC::_Next(Head);
        while(count>0){
            if(ACC::_Val(temp)==v)break;
            temp = ACC::_Next(temp);
            --count;
        }
        if(count==0)return iterator(Head);
        else return iterator(temp);
    }

    sizetype size(){
        return Length;
    }
    void clear(){
        Nodeptr temp = ACC::_Next(Head);
        Nodeptr del;
        while(Length>0){
            del = temp;
            temp = ACC::_Next(temp);
            del_mem(del);
            --Length;
        }
    }
    bool empty(){
        return (Length==0);
    }

    reference operator[](int x){
        if((sizetype)x>Length)exit(1);
        Nodeptr temp = ACC::_Next(Head);
        while(x>0){
            temp = ACC::_Next(temp);
            --x;
        }
        return (reference)ACC::_Val(temp);
    }

    private:
    Nodeptr buy_mem(){
        Nodeptr temp;
        temp = allocator::allocate(1);
        return temp;
    }

    void del_mem(Nodeptr _p){
        allocator::deallocate(_p,1);
    }
     
};