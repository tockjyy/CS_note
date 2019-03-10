

# 1.C++编程技巧笔记

## 1.1.Effective C++

- [1.关于const,enum,inline与define的使用](effective/1.md)
```
1.Define与Const
2.Define与Inline
3.Define与Enum
```
- [2.尽可能的多使用const](effective/2.md)
```
1.常量指针的特性
2.返回值使用const的好处
3.const成员函数
4.mutable与const
5.避免重复const和non-const两种不同代码的方法
```
- [3.有关类初始化的问题](effective/3.md)
```
1.内置类型请一定注意初始化
2.成员变量初始化时尽量使用成员初始列
3.跨编译单元类型初始化顺序问题
```
- [4.若不想使用编译器默认生成的函数就要明确拒绝它](effective/4.md)
```
1.C++类默认编写和调用了哪些函数
2.如何拒绝编译器自动产生的函数
3.防止隐式调用构造函数explicit(待补充)
```
- [5.为什么多态基类声明为虚析构函数](effective/5.md)
```
1.不含有虚析构函数的类不要轻易使用父类指针指向子类
2.使用virtual函数，使父类函数调用到子类函数
3.绝不在构造和析构函数中调用virtual函数
```
- [6.不要让异常逃离析构函数](effective/6.md)
- [7.operator操作符相关注意点](effective/7.md)
```
1.令operator操作符返回一个*this的引用的用途
2.在operator=处理自我赋值
```
- 持续更新中



# 2.一些简单的C++方法经验

### [线程安全的对象池编写与详细解释](content/objectpool.md)

### [简单的单例类编写](content/singleton.md)

### [简单的单例保护类编写](content/singleton_protect.md)

### [简单的线程安全的工作队列的编写](content/work_queue.md)

### [简单的协议路由类的编写](content/proto_route.md)

### [实用的网络消息缓存类的编写](content/message_buffer.md)

### [c++11到c++17的特性对c++编程者的影响](content/cpp11_exp.md)

## 参考资料
- C++ reference : (https://en.cppreference.com/w/cpp)
- Effective C++ 改善程序与设计的55个具体做法 第三版(作者：Scott Meyers)
- More Effective C++ 35个改善程序与设计的有效方法(作者：Scott Meyers)