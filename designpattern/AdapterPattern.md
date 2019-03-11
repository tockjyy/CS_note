# 适配器模式（Adapter Pattern）
- 无非是将现有一些类的功能组合增删，来实现新的目标功能
- 以下例子是提取部分功能来实现目标功能的

``` cpp
class Deque  //双端队列，被适配类，双端都可进出，包含栈和队列的所有子功能
{
public:
    void push_back(int x)
    {
        cout << "Deque push_back:" << x << endl;
    }
    void push_front(int x)
    {
        cout << "Deque push_front:" << x << endl;
    }
    void pop_back()
    {
        cout << "Deque pop_back" << endl;
    }
    void pop_front()
    {
        cout << "Deque pop_front" << endl;
    }
};

class Sequence  //顺序类，目标类，纯虚进出方法
{
public:
    virtual void push(int x) = 0;
    virtual void pop() = 0;
};

class Stack:public Sequence, private Deque   // 栈, 适配类，先进后出
{
public:
    void push(int x)
    {
        push_back(x);
    }
    void pop()
    {
        pop_back();
    }
};

class Queue:public Sequence, private Deque  //队列，适配类，先进先出
{
public:
    void push(int x)
    {
        push_back(x);
    }
    void pop()
    {
        pop_front();
    }
};
```
- 以下例子是合并多个功能来实现目标功能的
``` cpp

enum DO {
    FLY = 1,
    EAT,
    RUN,
    FIGHT
};

class bird
{
public:
    void flying()
    {
        cout << "I belive I can fly!" << endl;
    }
};

class pig
{
public:
    void delicious()
    {
        cout << "I am delicious!" << endl;
    }
    void runing()
    {
        cout << "I can run,but slow" << endl;
    }
};

class flypig : public pig,bird
{
public:
    void nothing()
    {
        cout << "nothing" << endl;
    }
    void play(DO doplay)
    {
        switch (doplay)
        {
        case FLY:flying(); break;
        case EAT:delicious(); break;
        case RUN:runing(); break;
        case FIGHT:nothing(); break;
        default: break;
        }
    }
};
```