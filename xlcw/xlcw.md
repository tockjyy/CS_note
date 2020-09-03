# 服务器开发DEMO

## 索引
* 1.编码命名规范
* 2.简单的ZONE上的GM写法
* 3.简单的UT写法
* 4.热更新的写法
* 5.打LOG的注意点

## 1.编码命名规范
``` cpp
// 虚拟的例子
-- PlayerMgr.h

// 使用前置声明，降低头文件互相包含而导致的编译效率问题
class cs::CSGetPlayerInfoReq;
class cs::CSGetPlayerInfoRes;
class CPlayer;

enum EnmMoneyType
{
    ENM_GOLD = 1,
    ENM_COIN = 2
};

class CPlayerMgr // 类名称以C开头，每个英文单词首字母大写
{
public:
    // 全局或静态变量使用 "g_"为前缀
    static int g_iMaxLevel;
    // 函数名和参数名请用标准的英文命名，如果仅仅是命名无法看出用途，请写出详细的注释，尤其是一些特殊的参数，英文不好理解的话请务必注释
    int GetPlayerInfo(CPlayer* pPlayer, const cs::CSGetPlayerInfoReq& stReq);
private:
    // 成员变量使用 "m_"为前缀
    uint64_t m_ullRoleID;
};

// 根据变量类型，变量名前面要接不同的小写前缀，方便阅读者理解
int iNum;
double dwNum;
CPlayer* pPlayer;
char[20] szName;
uint64_t ullLevel;
uint32_t uLevel;
std::string strName;
CPlayer stPlayer;
std::map<int, CPlayer*> mapPlayers;
std::vector<CPlayer*> vecPlayers;
bool bIsOpen;
...
//以上是常用的，尽量遵照就好

```

- 模块区分规范

``` cpp
// 基础模块的方法封装
Class CZoneMsgMgr
{
public:
    // 给Online发送一个网络包
    static int Send2Online(ss::SSPkgHead& stPkgHead, const void * pBody)
    {
        if(!pBody)
        {
            return -1;
        }
        int iOnlineAddr = CZoneConfMgr::Instance()->GetOnlineAddr();
        if(iOnlineAddr == -1)
        {
            return -1;
        }
        return SendOneSSMsg(iOnlineAddr, stPkgHead, pBody);
    }
private:
    // 像这种基础函数最好私有化，否则经常有人为了方便，直接在具体业务里又写了类似上面Send2Online的方法，也不是不行，这样就不方便代码管理了
    // 发送一个进程间数据包
    static int SendOneSSMsg(int iDstAddr, ss::SSPkgHead& stPkgHead, const void * pBody);
}

```

## 2.简单的ZONE上的GM写法

以下文件都在目录src/zone/gm/下
``` cpp
-- BattleUnionGM.h
#ifndef BATTLEUNION_GM_H_
#define BATTLEUNION_GM_H_

#include "Command.h"
#include <vector>
#include <string>

class CBattleUnionGM
{
    public:
        //开始宴会 参数和返回值是标准格式，请勿随意改变
        int GMStartMeet(CPlayer * pPlayer, const std::vector<std::string> & vParamList );
        ··· ···
};

#endif
```

``` cpp
-- BattleUnionGM.cpp
// 包含需要调用的头文件
#include "BattleUnionGM.h"
#include "BattleUnionMgr.h"
#include "CommandMgr.h"
··· ···

int CBattleUnionGM::GMStartMeet(CPlayer * pPlayer, const std::vector<std::string> & vParamList)
{
    // 判断参数数量
    if(vParamList.size() < 2)
    {
        return -1;
    }
    // 读取参数
    if(vParamList[1] == "all")
    {
        CBattleUnionMgr::Instance()->XXXX();
    }
    else if(vParamList[1] == "self")
    {
        CBattleUnionMgr::Instance()->YYYY();
    }
    else
    {
        CCommandMgr::Instance()->NotifyStringToClient(pPlayer, "failed cmd:unionmeetstart [self/all]");
        return -1;
    }
    // 这个函数会让客户端上显示调用GM后显示的字符串
    CCommandMgr::Instance()->NotifyStringToClient(pPlayer, "startmeet ok");
    return 0;
}
```

``` cpp
-- IntallGMFunc.h // 这里要包含GM头文件
··· ···
#include "BattleUnionGM.h"
··· ···
class CIntallGMFunc : public CSington<CIntallGMFunc>
{
    ··· ···
private:
    ··· ···
    CBattleUnionGM m_BattleUnionGM;//联盟GM
};
```
``` cpp
-- IntallGMFunc.cpp // 这个文件里要定义GM文件的入口
··· ···
#include "IntallGMFunc.h"

··· ···
Regist("unionmeetstart", // GM命令
"unionmeetstart [self/all]", // 命令说明
&CBattleUnionGM::GMStartMeet,&m_BattleUnionGM); // 函数指针和GM类实例指针


```

## 3.简单UT的写法
- UT，原意为单元测试，不过在我们游戏里的作用是强制影响游戏内数据的接口
- 只要有DECLAREPLATFORMUTCMD宏，不管定义在哪，都能加入UT指令，不过一般在src/zone/ut/目录下
``` cpp
#include "PlatformUtBase.h"
··· ···
DECLAREPLATFORMUTCMD(UnionMeet, "ShowUnionMeet [unionid or 0]", UT_DEFAULT_TYPE)
{
    if (argc < 2)
    {
        // 控制台输出字符串
        xlapp_ctrl_printf("usage :ShowUnionMeet [unionid or 0]\n");
    }
    int iChoose = atoi(argv[1]);

    (iChoose == 0)?ShowAllMeeting():ShowSingleMeeting(iChoose);
    return 0;

}
```

## 4.热更新的写法
- 一个普通的data文件加载类，最好存在src/base/comm/目录下
``` cpp
-- UnionTrainResMgr.h

#include "PlatResLoader.h"
#include "Sington.h"
#include "PBResDataLoader.hpp"
#include "dataconfig_leaguetrain.pb.h"
··· ···

typedef dataconfig::LeagueTrain PBLeagueTrain;
typedef dataconfig::LeagueTrainArray PBLeagueTrainArray;
class CLeagueTrain : public CPlatMapResHolder<uint32_t, PBLeagueTrainArray, PBLeagueTrain >
{
public:
    virtual uint32_t GetKey(const PBLeagueTrain& pbConfig)
    {
        return pbConfig.level();
    }
    ··· ···
};
··· ···
class CUnionTrainResMgr : public CSington<CUnionTrainResMgr>
{
public:
    //初始化
    int Init(int IsSmallServer, int iServerType);
    // 获取对应等级传功的奖励
    PBLeagueTrain* GetTrainReward(int level);
    ··· ···
private:
    CLeagueTrain m_LeagueTrain;
    ··· ···
};

```

``` cpp
-- UnionTrainResMgr.cpp
#include "UnionTrainResMgr.h"
#include "CommConfMgr.h"
#include "LoaderMgr.h"
··· ···
const static char * pLeagueTrain = "../../binconf/zone/data/dataconfig_leaguetrain.data";
··· ···
int CUnionTrainResMgr::Init(int IsSmallServer, int iServerType)
{
    // pLeagueTrain所指向的文件名称会被存下来，热更的时候会自动拉取这个文件
    int iRet = m_LeagueTrain.LoadResData(RedirectConfigDataFile(pLeagueTrain,
        IsSmallServer, iServerType));
    if (0 != iRet)
    {
        LOG_ERR("error while load %s, ret:%d", pLeagueTrain, iRet);
        return iRet;
    }
    /* 这个宏决定了可以热更
        第一个参数决定热更命令为：
        ./admin.sh script control ${SvrName} "ReloadDataconfig dataconfig_leaguetrain.data";
        第二个参数决定热更会更新的容器：
        m_LeagueTrain
        第三个参数决定了会调用哪一个函数进行热更，这个类必须继承了CBaseLoader，并且ReLoadResData非纯虚函数，并且调用m_LeagueTrain.ReLoadResData()进行更新
    */
    DECL_CONFIG_LOADER("dataconfig_leaguetrain.data", &m_LeagueTrain, CLeagueTrain);
    return 0;
}


```

## 5.打LOG的注意点
- 打日志的时机：
- 1.协议或者接口的关键传参过程最好把输入和输出都打印DEBUG日志出来，方便开发线调试，访问过与频繁的酌情考虑
- 2.一些关键代码错误或是发生业务数据发生不合理数据处理时尽量以ERROR方式打印出关键数据以便程序猿定位错误所在点
``` cpp
-- PlayerMgr.cpp
int CPlayerMgr::SubItem(CPlayer* pPlayer, int iItemID, int iNum)
{
    ··· ···

    // 错误的示范
    LOG_ERR("items of the player is not enough!");
    /* 最后输出的结果：
    [20190821 00:00:00.364998]ERROR|....../src/zone/player/playermgr.cpp:94|SubItem|items of the player is not enough!
    除了知道有用户减物品的时候物品不足，就没知道什么东西了，最后就算查到有了错误也不知道错误出现的具体信息，输出的这些文字也不用这么复杂，因为日志有打印LOG出现在哪个文件哪一行哪一个函数，所以文字并不需要这么长，只要方便查找就好
    */

    // 推荐写法
    LOG_ERR("NoEnough: RoleID:%"PRIu64" ItemID:%d SubNuk:%d HasNum:%d",
        pPlayer->GetRoleID(), iItemID, iNum, pPlayer->GetItemNum(iItemID));
    /*
        NoEnough方便和文件名和函数名一起查询定位，其他的尽量输出与错误有关的参数，并在参数前面输出参数名，省去了去代码里对照查看的时间
    */
    ··· ···
}
```
