
## 特性介绍
该对象池是线程安全的，并且不用使用者考虑堆内存回收，会在对象池离开作用域时自动清理
### 1.对象池使用例子
``` c++
//main.cpp
#include "objectpool.h"
#include <string>

using namespace std;

class test_object
{
public:
    int id_;
    string name_;
public:
    auto GetName() { return name_; } // need c++17
    void SetName(const string& str) { name_ = str; }
};

int main(void)
{
    ObjectPool<test_object> obj_pool_;
    cout << "size: " << obj_pool_.size() << endl; // size:0
    cout << "idle_size: " << obj_pool_.idle_size() << endl; // idle_size:0
    do
    {
        SmartObject<test_object> obj1(obj_pool_);
        obj1->SetName("obj1");
        cout << obj1->GetName() << endl; //obj1
        cout << "size: " << obj_pool_.size() << endl; // size:1
        cout << "idle_size: " << obj_pool_.idle_size() << endl; // idle_size:0
    } while (0);
    cout << "size: " << obj_pool_.size() << endl; // size:1
    cout << "idle_size: " << obj_pool_.idle_size() << endl; // idle_size:1
    do 
    {
        SmartObject<test_object> obj2(obj_pool_);
        cout << obj2->GetName() << endl; //obj1
        cout << "size: " << obj_pool_.size() << endl; // size:1
        cout << "idle_size: " << obj_pool_.idle_size() << endl; // idle_size:0
        SmartObject<test_object> obj3(obj_pool_);
        obj3->SetName("obj3");
        cout << obj3->GetName() << endl; //obj3
        cout << "size: " << obj_pool_.size() << endl; // size:2
        cout << "idle_size: " << obj_pool_.idle_size() << endl; // idle_size:0
    } while (0);
    cout << "size: " << obj_pool_.size() << endl; // size:2
    cout << "idle_size: " << obj_pool_.idle_size() << endl; // idle_size:2
    return 0;
}

```

### 2.对象池源码
``` c++
//objectpool.h
#include <atomic>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

/* 自旋锁
*  特性：当锁被另一个线程占用时，当前线程不交出线程所有权，无限循环检查锁的占用情况
*  优点：不用释放线程操作
*  缺点：如果锁长时间被另外一个线程占用，当前线程会陷入长时间的死循环
*  适用场景：对响应要求高，并且响应所需时间短的情求
*/
class SpinLock
{
private:
    std::atomic<bool> _flag = ATOMIC_VAR_INIT(false);
public:
    SpinLock() = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator= (const SpinLock&) = delete;
    void lock()
    {
        bool expected = false;
        while (!_flag.compare_exchange_strong(expected, true))
            expected = false;
    }
    void unlock()
    {
        _flag.store(false);
    }
};
//智能的锁使用类，在该变量作用域内，锁会自动的占用和释放
template<typename T>
class SmartLock
{
private:
    T& _mutex;
public:
    SmartLock(T& mutex_) :_mutex(mutex_)
    {
        _mutex.lock();
    }
    ~SmartLock()
    {
        _mutex.unlock();
    }
};
//对象池类
template <class T>
class ObjectPool
{
public:
    T* Pop()
    {
        SmartLock<SpinLock> mutex(lock_);
        if (pool_.empty())
        {
            T* ptr = new T();
            instances_.push_back(ptr);
            return ptr;
        }
        else
        {
            T* ptr = pool_.front();
            pool_.pop();
            return ptr;
        }
    }
    void Push(T* ptr)
    {
        SmartLock<SpinLock> mutex(lock_);
        pool_.push(ptr);
    }
    inline size_t idle_size()
    {
        return pool_.size();
    }
    inline size_t size()
    {
        return instances_.size();
    }
    ~ObjectPool()
    {
        for (auto info : instances_)
        {
            delete info;
        }
    }
private:
    SpinLock lock_;
    queue<T*> pool_;
    vector<T*> instances_;
};

//对象池使用类，在该变量作用域外，对象会自动返还对象池
template <class TObject>
class SmartObject
{
 public:
    SmartObject(ObjectPool<TObject>* pool):
        pool_(pool),
        pApi_(NULL)
    {
        pApi_ = pool_->Pop();
    }
    ~SmartObject()
    {
        pool_->Push(pApi_);
    }
    TObject* operator->()
    {
        return pApi_;
    }
private:
    ObjectPool<TObject>* pool_;
    TObject* pApi_;
};

```