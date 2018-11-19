# 浅谈c++11之后的特性与c++98编程之间的便利
关于这方面的描述网上早已经烂大街，我这里只是有写体验心得，具体内容我抄下来也没意思，[`官方的文档`](http://en.cppreference.com/w/cpp)要多漂亮有多漂亮
## 1.auto关键字
auto变量类型，可以根据赋值结果来推算变量的类型，对运行性能没有任何影响，这一切都是在编译器层面优化的。
``` cpp
int add(int x,int y){return x+y;}
auto sum = add(1,2); //sum变量会根据add函数的返回值来为sum声明为int类型
```
这一切都是在编译器层面优化的，所以auto型的变量必须要赋初值，否则编译器不知道它的初始类型
``` cpp
int add(int x,int y){return x+y;}
auto sum;//这种方式是错误的，编译器不知道sum的初始类型，无法精准分配局部变量空间
sum = add(1,2);
```
当然还有更骚的，不记得是14还是17标准之后，返回值也可以用auto，不过其他的高级语言不知道早几万年前就有这功能了，所以c++牺牲了点编译性能弄了这个还是比较能抚慰我们程序猿脆弱的心灵的，话不多说简单的例子来了
``` cpp
auto add(int x,int y){return x+y;}
auto sum;//这种方式是错误的，编译器不知道sum的初始类型，无法精准分配局部变量空间
sum = add(1,2);
```

## 2.迭代器的福音
在c++98中，遍历一个stl的容器一般都是要使用迭代器来进行操作，比如
``` cpp
map<int,int> map_ = {...};
for(map<int,int>::iterator itr = map_.begin(); itr != map.end(); ++itr)
{
    int first = itr->first;
    int second = itr->second;
}
```
或者
``` cpp
map<int,int> map_ = {...};
map<int,int>::iterator itr = map_.begin();
while(itr != map_.end())
{
    int first = itr->first;
    int second = itr->second;
    if(second == xxx)
    {
        //这种情况一般用于需要删除其中某元素时使用，erase的返回值里有返回下一个元素的迭代器
        itr = map_.erase(itr);
        continue;
    }
    ++itr;
}
```
而在c++11后，程序猿的就可以从繁重的体力劳动中解放出来了，之前的两种场景只需要这么写了：
``` cpp
map<int,int> map_ = {...};
for(auto& info : map_)
{
    int first = info.first;
    int second = info.second;
}
```
AND
``` cpp
map<int,int> map_ = {...};
auto itr = map_.begin();
while(itr != map_.end())
{
    int first = itr->first;
    int second = itr->second;
    if(second == xxx)
    {
        //这种情况一般用于需要删除其中某元素时使用，erase的返回值里有返回下一个元素的迭代器
        itr = map_.erase(itr);
        continue;
    }
    ++itr;
}
```
## 3.匿名函数
``` cpp
auto func_add = [](int x,int y) -> int
{
    return x + y;
};
//use
auto sum = func_add(1,2);
```
或者可以不用变量存函数指针，直接用
``` cpp
auto sum = [](int x,int y) -> int {return x + y;}(1,2);
```
这么用感觉意义不大啊，所以一般是用来写线程的回调函数比较多，我演示下不用匿名函数的方式
``` cpp
void myThread(int first, int second)
{
    cout << "in my thread,first = " << first << ",second = " << second << endl;
    cout << "sleep 2s" << endl;
    Sleep(2000);
    cout << "weak up" << endl;
}

int main()
{

    std::thread t1(&myThread, 10, 20);//创建一个分支线程，回调到myThread函数里
    t1.join();

    cout << "in major thread";//在主线程
}
```
使用匿名函数的方式
``` cpp
int main()
{

    std::thread t1([](int first, int second){
        cout << "in my thread,first = " << first << ",second = " << second << endl;
        cout << "sleep 2s" << endl;
        Sleep(2000);
        cout << "weak up" << endl;
    }, 10, 20);//创建一个分支线程，回调到myThread函数里
    t1.join();

    cout << "in major thread";//在主线程
}
```
不用满天飞的找回调函数感觉小爽，当然在stl有些容器有std::sort方法里也有用来排序的回调函数，用这个也是不用把你的排序函数写到山的那边海的那边

## 4.智能指针
当年苦哈哈的使用c++98的时候，new和delete是程序员心中永远的痛，收捡堆内存这活，不是人干的，当时好羡慕那种有GC的语言，我们那时候心情好就自己写简单的自动回收类，心情不好就野指针漫天飞，后来听前辈说boost的智能指针好用，用了一下就不能自拔了，后来c++11的标准出来了之后，智能指针也加入了，用法和boost一模一样，怎么用我就不说了，文档漂亮多了

['Dynamic memory management document'](http://en.cppreference.com/w/cpp/memory)

如果不用智能指针，我就自己写一个：
``` cpp
template <class T>
class smart_ptr
{
private:
    T* ptr_;
    void clear()
    {
        if (this->ptr_ != nullptr)
        {
            delete this->ptr_;
        }
    }
public:
    smart_ptr() :ptr_(new T)
    {}
    smart_ptr(T* ptr) :ptr_(ptr)
    {}
    smart_ptr<T>& operator = (T* ptr)
    {
        clear();
        this->ptr_ = ptr;
        return *this;
    }
    T* operator->()
    {
        return this->ptr_;
    }
    ~smart_ptr()
    {
        clear();
    }
};
//use
int main()
{
    smart_ptr<string> pstring;
    pstring->append("first");
    pstring = new string("second");
    cout << pstring->c_str() << endl;
}
```
不得不说，原生用指针的效率最好，但是一旦用合理的规范来管理你的代码，也就是良好的编程习惯，原生的指针基本上不会为程序猿带来多少烦恼，std提供的智能指针功能很强大的，不过适用的场景还是要程序猿自己去辨别利弊

## 5.namespace嵌套写法的优化
这个很简单，c++17新加的特性，纯写法特性
``` cpp
//old
namespace one{
    namespace two{
        void xx_function(void);
    }
}
//after c++17
namespace one::two{
    void xx_function(void);
}
```

## 6.元组的使用(tuple)
看下面，python可以多返回值，有没有羡慕嫉妒恨，不用这样，c++11之后也可以的
``` py
def AddAndSub(x,y):
    retun (x+y, x-y)
```
c++98
``` cpp
struct AddSub{
    int add;
    int sub;
};
AddSub AddAndSub(int x, int y)
{
    AddSub result;
    result.add = x+y;
    result.sub = x-y;
    return result;
}
```
有没有感觉很傻，有了元组以后就是
``` cpp
#include <tuple>
using namespace std;

tuple<int,int> AddAndSub(int x,int y)
{
    return make_tuple(x+y,x-y);
}

int add,sub;
tie(add, sub) = AddAndSub(2,1);
//add = 3 sub = 1

//可以忽略某个元素
tie(add, std::ignore) = AddAndSub(7,1);
//add = 8 sub = 1

```