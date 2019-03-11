# 建造者模式（Builder Pattern）
``` cpp

//订单
class Order
{
public:
    void SetName(const string& str) { name = str; }
    void SetPrice(const int& num) { price = num; }
    void SetType(const string& str) { type = str; }
private:
    string name;        // 订单名称
    int price;          // 订单价格
    string type;        // 订单类型
};

//建造者爸爸，父类，纯虚纯抽象类
class Builder
{
public:
    virtual void BuildName(const string& str) = 0;
    virtual void BuildPrice(const int& num) = 0;
    virtual void BuildType(const string& str) = 0;
    virtual void createOrder() = 0;
    virtual Order* getProduct() = 0;
};

//建造者A
class BuilderA : public Builder
{
public:
    BuilderA():order(nullptr){}
    virtual void BuildName(const string& str)
    {
        order->SetName("BuilderA:" + str);
    }
    virtual void BuildPrice(const int& num)
    {
        order->SetPrice(num);
    }
    virtual void BuildType(const string& str)
    {
        order->SetType("BuilderA:" + str);
    }
    virtual void createOrder() = 0
    {
        if (!order)order = new Order;
    }
    virtual Order* getProduct()
    {
        return order;
    }
private:
    Order* order;
};

//建造者B
class BuilderB : public Builder
{
public:
    BuilderB() :order(nullptr) {}
    virtual void BuildName(const string& str)
    {
        order->SetName("BuilderB:" + str);
    }
    virtual void BuildPrice(const int& num)
    {
        order->SetPrice(num);
    }
    virtual void BuildType(const string& str)
    {
        order->SetType("BuilderB:" + str);
    }
    virtual void createOrder() = 0
    {
        if (!order)order = new Order;
    }
    virtual Order* getProduct()
    {
        return order;
    }
private:
    Order* order;
};

//管理者
class Director
{
public:
    Director(Builder* builder) :m_builder(builder)
    {
    }
    void construct(string name, int price, string type)
    {
        m_builder->createOrder();
        m_builder->BuildName(name);
        m_builder->BuildPrice(price);
        m_builder->BuildType(type);
    }

private:
    Builder* m_builder;
};
```