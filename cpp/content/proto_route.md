# 简单的协议路由选择
## 1. 源码与使用方法
``` cpp
//msg.h

struct Work
{
    string body;
    uint32_t cmd;
};

class MsgHandle
{
protected:
public:
    virtual void Handle(const string& body) = 0;
    static MsgHandle* CreateProtocol(const uint32_t& cmd);
    static void RouteHandle(const Work& work_)
    {
        MsgHandle* pHandle = CreateProtocol(work_.cmd);
        if (pHandle)
        {
            pHandle->Handle(work_.body);
        }
    }
};
```
``` cpp
//msg.cpp
#include "msg.h"
#include "proto1.h"

#define defRoute(cmdid,className) \
case cmdid:{return new className();}break;

#define defRouteDefault()   \
default: return NULL;

MsgHandle * MsgHandle::CreateProtocol(const uint32_t & cmd)
{
    switch (cmd)
    {
        defRoute(1, proto1)
            defRouteDefault()
    }
}
```
``` cpp
//proto.h
#include "msg.h"

class proto1 : public MsgHandle
{
public:
    virtual void Handle(const string& body) override
    {
        cout << body << endl;
    }
};

```
``` cpp
//main.cpp
#include "msg.h"

int main(void)
{
    Work work;
    work.cmd = 1;
    work.body = "ABCD";
    MsgHandle::RouteHandle(work);
}

```