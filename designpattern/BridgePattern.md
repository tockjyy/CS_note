# 桥接模式（Bridge Pattern）
- 不同的分类分离，减少耦合而已
``` cpp
class Pet
{
public:
    virtual void buy() = 0;
};

class Cat : public Pet
{
public:
    void buy()
    {
        cout << "买了一个猫" << endl;
    }
};

class Dog : public Pet
{
public:
    void buy()
    {
        cout << "买了一个狗" << endl;
    }
};

class Shop
{
public:
    virtual void go_buy(Pet* pet) = 0;
};

class PetShop : public Shop
{
public:
    void go_buy(Pet* pet)
    {
        cout << "我在宠物商店 ";
        pet->buy();
    }
};

class PetHospital : public Shop
{
public:
    void go_buy(Pet* pet)
    {
        cout << "我在宠物医院 ";
        pet->buy();
    }
};

int main()
{
    auto cat = new Cat;
    auto dog = new Dog;
    auto petshop = new PetShop;
    auto petHosp = new PetHospital;
    petHosp->go_buy(cat);
    petHosp->go_buy(dog);
    petshop->go_buy(cat);
    petshop->go_buy(dog);
    getchar();
    return 0;
}
```