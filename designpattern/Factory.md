# 工厂模式
- 应用场景1：根据日志类型做不同文件写入的分发
- 应用场景2：根据配置文件的db类型，调用不同的DB产品库
- 太多太多了
``` cpp
enum PetType
{
    CAT = 1,
    DOG = 2
};

// 宠物类
class Pet
{
public:
    virtual void Play() = 0;
};

class Dog : public Pet
{
public:
    virtual void Play()
    {
        cout << "汪汪" << endl;
    }
};

class Cat : public Pet
{
public:
    virtual void Play()
    {
        cout << "喵喵" << endl;
    }
};

//宠物工厂
class PetFactory
{
public:
    static Pet* BuyNewPet(PetType type)
    {
        switch (type)
        {
            case CAT: return new Cat;
            case DOG: return new Dog;
            default: return nullptr;
        }
    }
};

int main(void)
{
    auto pet1 = PetFactory::BuyNewPet(DOG);
    pet1->Play();
    auto pet2 = PetFactory::BuyNewPet(CAT);
    pet2->Play();
    delete pet1;
    delete pet2;
    getchar();
    return 0;
}
```