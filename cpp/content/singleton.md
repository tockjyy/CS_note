## 特性介绍
此为简单的单例类，会在程序结束时自动析构实体类,实体类最好将初始化写在默认构造函数里

### 1.使用例子
``` c++
//main.cpp
#include "singleton.h"
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
    Singleton<test_singleton>::Instance()->SetName("test");
    cout << Singleton<test_singleton>::Instance()->GetName() << endl;

    return 0;
}
```

### 2.源码
``` c++
//singleton.h
#include <cstddef>
template <class T>
class Singleton
{
public:
    static T* Instance()
    {
        if (Instance_ == NULL)
        {
            static CGarbo cGarbo;		//垃圾收集
            Instance_ = new T;
        }
        return  Instance_;
    }
private:
    static T*  Instance_;
    class CGarbo // 它的唯一工作就是在析构函数中删除SingleTon的实例  
    {
    public:
        ~CGarbo()
        {
            if (Singleton::Instance_ != NULL)
                delete Singleton::Instance_;
        }
    };
    Singleton(void);
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);
};

template <class T>
T* Singleton<T>::Instance_ = NULL;
```