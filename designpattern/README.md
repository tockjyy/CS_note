# 设计模式笔记(C++)
## 0.序言
```
依我自己的看法，没有标准的模式，只有万变的需求
不用苛求什么标准和模式，一切以应用场景为主，模式只是用来参考
并不是什么不变的，要学习他们这种解决问题的思路，见得多了才能设计得更好
```

## 1. 创建型模式
### [工厂模式（Factory Pattern）](Factory.md)
### [抽象工厂模式（Abstract Factory Pattern）](AbstractFactory.md)
### [单例模式（Singleton Pattern）](Singleton.md)
### [建造者模式（Builder Pattern）](Builder.md)
### 原型模式（Prototype Pattern）
```
这个不单独拉出来说了，依我看完全就是父类一个纯虚的clone函数
每个子类都要去重载下这个克隆函数
还不如吧赋值构造和operator=的功效写完善再直接clone调用
也不是没意义吧，算是标准化吧
```

## 2.结构型模式
### [适配器模式（Adapter Pattern）](AdapterPattern.md)
### [桥接模式（Bridge Pattern）](BridgePattern.md)
### 组合模式（Composite Pattern）
``` cpp
相同等级的元素组合起来挂载在上级元素下，简单的树型结构就算
如：
class 国家
{
    map<省名称,省>
}
class 省
{
    map<市名称,市>
}
class 市
{
    set<县名称>
}
```
### 装饰器模式（Decorator Pattern）
``` sql
差不多就是写出可以方便添加和删除的新功能而不会产生耦合
比如一些高级语言给数据库提供的操作函数很多是类似这种格式
sql.from("tablexxx")
    ->select("a")
    ->select("b")
    ->where("xx","=",8)
    ->where("yy","<",10)
    ->orderDESC(); 
不一定完全一样哈，我只是大概描述一下，组成的SQL是
select a,b from tablexxx where xx = 8 and yy < 10 DESC;
个人理解，大概就是怎么一种方便增减的思想，我描述不到的话别打我，看别人的例子都是类套类，我倒觉得不怎么实用
```
### 外观模式（Facade Pattern）
``` cpp
就是把一堆复杂操作封装到一起，让调用的时候就调一个高级接口
如程序启动时：
initLogFile(xxx)//初始化日志文件
initDB(xxx)//初始化数据库
initRD(xxx)//初始化缓存
要让外面看起来简化就写个函数，再调用
init(xxx)
{
    initLogFile(xxx)//初始化日志文件
    initDB(xxx)//初始化数据库
    initRD(xxx)//初始化缓存 
}
我的理解就是封装好看一些的意思
```
### 享元模式（Flyweight Pattern）
``` cpp
单例模式的懒汉模式就是典型的利用了享元模式的思想
template <class T>
class Singleton
{
public:
    static T* Instance()
    {
        if (Instance_ == NULL)      //如果已经实例化过了，就不要再重新分配了
        {
            Instance_ = new T;
        }
        return  Instance_;
    }
private:
    static T* Instance_;
}
```
### 代理模式（Proxy Pattern）
```
不难理解，这中思想应用场景也很多
比如防火墙拒绝了固定端口或ip交互的网络消息
智能指针代替用户手动回收堆内存等等
```

## 3.行为型模式

### 责任链模式（Chain of Responsibility Pattern）
```
就是一系列的判断链，也有很多场景会用到
如：
某氪金手游，不同的vip等级每日奖励不一样，
判断顺序就是：
vip4->vip3->vip2->vip1->屌丝
有一个先满足就跳出并处理
```
### 命令模式（Command Pattern）
```
有一种网络模型比较像这个模式
网络线程只负责接收和发送网络包
通过两个条件变量配合建立两个队列，一个接收网络包队列，一个发送网络包队列
由工作线程负责从接收网络包队列拿接收网络包，处理后将发送网络包放入发送网络包队列
网络线程只是负责收发用户的网络消息，不参与逻辑的详细处理
```
## 解释器模式（Interpreter Pattern）
```
之后慢慢看，没弄明白运用点在哪？
```
迭代器模式（Iterator Pattern）

中介者模式（Mediator Pattern）

备忘录模式（Memento Pattern）

观察者模式（Observer Pattern）

状态模式（State Pattern）

空对象模式（Null Object Pattern）

策略模式（Strategy Pattern）

模板模式（Template Pattern）

访问者模式（Visitor Pattern）