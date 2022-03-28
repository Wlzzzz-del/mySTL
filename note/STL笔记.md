# STL中使用仿函数
在C语言中，想要将函数作为参数传递，只能将函数指针作为参数传递
```C++
bool cmp(const void* elem1, const void* elem2);

void main()
{
    int ia[4] = {32, 92, 67, 58};

    qsort(ia,sizeof(ia)/sizeof(int),sizeof(int),cmp);//传递cmp指针，作为排序的策略
}
```
但是函数指针无法持有自己的状态，以及无法加另外的条件对函数进行修饰。  
于是在STL中采用仿函数(functor),所谓仿函数就是用起来像函数的东西。其本质是对括号()运算符进行重载。
```C++
template <typename T>
class max{
    public:
    T operator()(const T& elem1, const T& elem2)
    {
        if(elem1 > elem2) return elem1;
        else return elem2;
    }
};

int main()
{
    std::cout<<max<int>()(1 ,2)<<std::endl;// return 2
    getchar();
    return 0;
}
```
# Traits 编程技法
## 问题背景
迭代器所指对象的类型一般被声明typedef为value_type,如此做可以将value_type作为函数的返回类型。  
```C++
template <class _Ty>
class array{
   typedef  _Ty value_type;
}
```
问题是当Iterator是一个自定义class时，我们可以很方便地使用value_type,如
```C++
array::value_type val;
```
但是如果Iterator是C++自定义地五种基本类型的指针(int*\float*等)，那么这时候该如何使用value_type?

## 通过Traits萃取出所需的associated type
Iterator traits(萃取机)用来分离class iterator和 非class iterator。  
需要注意的是，对于非class iterator，其value_type 也十分明显。
```C++
// 以int*为例，其value_type就是int
int* a;// 实际上Iterator是一种泛化的指针，所以指针也算是一种iterator
```
## 简单的Traits例子
```C++
template<class T>
struct iterator_traits{
    // T::value_type即是调用T的类内自定义类型
    typedef typename T::value_type valuetype;
}

template<class T>
// 通过template偏特化实现
struct iterator_traits<T*>{
    // T本身就是类型
    typedef T value_type;
}

template<class T>
struct iterator_traits<const T*>{
    typedef T value_type;
}
```
## 结论
以上，算法在拿到任意一个iterator时，就可以直接通过萃取机直接使用其类型。

