#ifndef FORWARDLIST_H
#define FORWARDLIST_H
#include <iostream>

template <class _Ty>
class Forward_List
{
private:
    // Node define
    struct Node{
        _Ty val;
        Node*   Next;
    };

    // basic typedef 
    typedef _Ty         _ValueType;
    typedef _Ty*        _Valueptr;
    typedef _Ty&        _ValueRef;
    typedef const _Ty   const_value_type;
    typedef const _Ty*  const_value_ptr;
    typedef const _Ty&  const_value_ref;
    typedef Node*       iterator;
    typedef Node*       _Nodeptr;
    typedef int         difference_type;
    typedef int         _Location;
    typedef size_t      _size;

    // Head define
    _Nodeptr _Head;
    _Nodeptr _Cur;
    _size _Size;


    // API to visit node
    struct _Acc
    {
        typedef _Nodeptr&       _refnode;
        typedef _ValueRef   _refval;

        static _refnode _next(_Nodeptr _n){return (_refnode)_n->Next;}
        static _refval  _val(_Nodeptr _n){return (_refval)_n->val;}
        // offer a normal version for begin
        static _Nodeptr _nornext(_Nodeptr _n){return _n->Next;}
    };

private:
    // create new node, ignore value
    _Nodeptr buy_node()
    {
        _Nodeptr p = (_Nodeptr)malloc(sizeof(Node));
        assert(p!=NULL);
        return p;
    }

    // delete a node
    void del_node(_Nodeptr n){
        delete n;
    }

public:
    // ctr
    explicit Forward_List():_Head(buy_node())
    ,_Cur(_Head),_Size(0){}

    ~Forward_List(){
        _Nodeptr temp = _Head;
        _Nodeptr t;
        while(temp != _Cur){
            t = temp;
            temp = _Acc::_nornext(temp);
            del_node(t);
        }
        del_node(temp);
    }

    void push_back(const _ValueType& v){
        _Nodeptr p = buy_node();
        _Acc::_next(_Cur) = p;
        _Acc::_val(p) = v;
        _Size ++;
    }

    void insert(_Location loc, _ValueType v){}

    void erase(_Location loc){}

    // need to define iterator plusplus
    iterator begin(){
        return _Acc::_nornext(_Head);
    }

    iterator end(){
        // may be error.
        return _Acc::_nornext(_Cur);
    }

    _size size(){
        return _Size;
    }

    bool empty()
    {
        return _Size?true:false;
    }
    
};

#endif