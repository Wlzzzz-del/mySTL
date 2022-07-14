#ifndef _ALLOC_
#define _ALLOC_
#include <new.h>
#include <iostream>
using namespace std;
// 简单的空间配置器
namespace aloc{
template<class T>
class allocator{
   typedef T* pointer;
   typedef T value_type;
   typedef const T* const_pointer;
   typedef T&  reference;
   typedef const T& const_reference;
   typedef size_t sizetype;
   typedef int difference_type;
   public:
   allocator(){};
   allocator(const allocator&){};
   ~allocator(){};

   pointer address(reference _p){
    return (pointer)&_p;
   };// 返回一个对象的地址
   pointer address(const_reference _p){
    return (const_pointer)&_p;
   };

   static pointer allocate(sizetype n){
    set_new_handler(0);
    pointer tmp = (pointer)malloc(sizetype(sizeof(T)*n));
    if(tmp == 0){
       cout<<"out of memory."<<endl;
       exit(1);
    }
    return tmp;
   };// 配置存储n个T对象的空间

   static void deallocate(pointer _p, sizetype n){
    free(_p);
   };
   sizetype max_size()const{
    return max(sizetype(1),sizetype(4096/sizeof(T)));
   };// 可成功配置的最大量
};
};
#endif