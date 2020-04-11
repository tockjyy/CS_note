# 多用operator操作符

### 1.当map的键是一个联合索引时
- 使用简单粗暴的方式，我见过很多代码都是做成员变量拼接，功能挺好，就是感觉不好看
``` cpp

class Student
{
public:
    uint32_t _uClassID;  // 班级ID
    uint32_t _uStuentID; // 学号
    string   _sName;     // 姓名

    uint64_t GenKey()
    {
        return ((uint64_t)_uClassID << 32) + _uStuentID;
    }

    static uint64_t GenKey(uint32_t uClassID, uint32_t uStudentID)
    {
        return ((uint64_t)uClassID << 32) + uStudentID;
    }
};

class StudentMgr
{
public:
    const Student * GetStudent(uint32_t uClassID, uint32_t uStudentID)
    {
        auto key = Student::GenKey(uClassID, uStudentID);
        auto itr = _mapStudents.find(key);
        if(itr == _mapStudents.end())
        {
            return NULL;
        }
        return &(*itr);
    }
    bool PushStudents(const Student& stStudent)
    {
        auto key = stStudent.GenKey();
        return _mapStudents.insert(make_pair(key, stStudent))->second;
    }
protected:
    map<uint64_t, Student> _mapStudents;
}

```
- 如果使用operator，如下写法
``` cpp
class StudentKey
{
public:
    StudentKey(uint32_t uClassID, uint32_t uStudentID)
        :_uClassID(uClassID)
        ,_uStuentID(uStuentID)
    {
    }
    uint32_t _uClassID;  // 班级ID
    uint32_t _uStuentID; // 学号
    // 提供给红黑树做排序
    bool operator < (const StudentKey& key)
    {
        if(_uClassID < key._uClassID)return true;
        if(_uStuentID < key._uStuentID)return true;
        return false;
    }
    // 提供给匹配子叶节点
    bool operator == (const StudentKey& key)
    {
        return (_uClassID == key._uClassID) && (_uStuentID == key._uStuentID);
    }
}

class Student
{
public:
    StudentKey _stKey;
    string   _sName;     // 姓名
};

class StudentMgr
{
public:
    const Student * GetStudent(uint32_t uClassID, uint32_t uStudentID)
    {
        auto key = StudentKey(uClassID, uStudentID);
        auto itr = _mapStudents.find(key);
        if(itr == _mapStudents.end())
        {
            return NULL;
        }
        return &(*itr);
    }
    bool PushStudents(const Student& stStudent)
    {
        const auto& key = stStudent._stKey;
        return _mapStudents.insert(make_pair(key, stStudent))->second;
    }
protected:
    map<StudentKey, Student> _mapStudents;
}
```

### 2.当一些常用的方法，能用operator实现功能的话方便理解函数名称
- 按常规方式写一个玩家类，实现通货管理
``` cpp
// 通货类型
enum EnumMoneyType
{
    GOLD = 0,   // 黄金
    SILVER, // 银币
    COPPER, // 铜钱
    MONEY_MAX_TPYE
};

class Player
{
public:
    uint64_t AddMoney(uint64_t val, EnumMoneyType type)
    {
        if(type >= MONEY_MAX_TPYE) return 0;
        _stMoneyVal[type] += val;
        return _stMoneyVal[type];
    }
    uint64_t _stMoneyVal[MONEY_MAX_TPYE];
    ... ...
};

```
- 用operator实现
