## 特性介绍
此为保护型的单例类，可以将一个非单例类型的变量转化成单例方式使用(将一个生存周期长的局部变量可以转换成静态变量，从而跨作用域)，当然，你用的时候要确定这个变量是否还在生存周期内

### 1.使用例子
``` c++
//main.cpp
#include "singleton_protect.h"
#include <iostream>
#include <string>

using namespace std;

class test_singleton
{
public:
    test_singleton() :
        id_(0),
        name_("")
    {
    }
private:
    int id_;
    string name_;
public:
    auto GetName() { return name_; } // need c++17
    void SetName(const string& str) { name_ = str; }
};

int main(void)
{
    test_singleton test;
    SingletonProtect<test_singleton>::Instance(&test)->SetName("ABCD");
    cout << SingletonProtect<test_singleton>::Instance()->GetName() << endl;
    cout << test.GetName() << endl;

    return 0;
}
```

### 2.源码
``` c++
//singleton_protect.h
#include <cstddef>
template <class T>
class SingletonProtect
{
public:
    static T* Instance(T* ptr = NULL)
    {
        if (Instance_ == NULL)
        {
            Instance_ = ptr;
        }
        return  Instance_;
    }
private:
    static T*  Instance_;
    SingletonProtect(void);
    SingletonProtect(const SingletonProtect&);
    SingletonProtect& operator= (const SingletonProtect&);
};

template <class T>
T* SingletonProtect<T>::Instance_ = NULL;
```