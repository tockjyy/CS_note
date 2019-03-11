# 抽象工厂模式
- 简单的描述下，就是工厂的工厂，尴尬

``` cpp
enum PetType
{
	CAT = 1,
	DOG = 2
};

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
enum WeaponType
{
    SWORD = 1,
    GUN = 2
};

class Weapon
{
public:
    virtual void Play() = 0;
};

class Sword : public Weapon
{
public:
    virtual void Play()
    {
        cout << "砍砍" << endl;
    }
};

class Gun : public Weapon
{
public:
    virtual void Play()
    {
        cout << "砰砰" << endl;
    }
};

enum FactoryType
{
    PET = 1,
    WEAPON =2
};
class AbstractFactory
{
public:
    virtual Pet* BuyNewPet(PetType type) = 0;
    virtual Weapon* BuyNewWeapon(WeaponType type) = 0;
};

class PetFactory : public AbstractFactory
{
public:
    virtual Pet* BuyNewPet(PetType type)
    {
        switch (type)
        {
        case CAT: return new Cat;
        case DOG: return new Dog;
        default: return nullptr;
        }
    }
    virtual Weapon* BuyNewWeapon(WeaponType type)
    {
        return nullptr;
    }
};
class WeaponFactory : public AbstractFactory
{
public:
    virtual Pet* BuyNewPet(PetType type)
    {
        return nullptr;
    }
    virtual Weapon* BuyNewWeapon(WeaponType type)
    {
        switch (type)
        {
        case SWORD: return new Sword;
        case GUN: return new Gun;
        default: return nullptr;
        }
    }
};

static AbstractFactory* GetFactory(FactoryType type)
{
    switch (type)
    {
    case PET: return new PetFactory;
    case WEAPON: return new WeaponFactory;
    default: return nullptr;
    }
}

int main(void)
{
    auto factory = GetFactory(WEAPON);
    auto weapon = factory->BuyNewWeapon(GUN);
    weapon->Play();
    delete weapon;
    delete factory;
	getchar();
	return 0;
}
```