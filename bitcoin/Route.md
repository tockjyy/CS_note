# bitcoin网络协议

## 协议目录

|route_name|comment|
|:---|:---|
|version|如果命令是版本：节点对应的版本|
|addr|地址消息|
|inv|库存消息|
|getdata|获取数据|
|getblocks|从其他节点上获得主链上的所有块数据|
|tx|请求交易的信息|
|review||
|block||
|getaddr||
|checkorder||
|submitorder|提交一个新的交易订单|
|reply|交易结果的回复|


### 1.version
- 接收来自其他节点的版本信息，并更新连接信息
- 并且对第一个发送该协议的节点回复getblocks命令，以求更新最新的区块信息
- 如果没有接到一个新节点的这个协议，确认了一个可用的version信息，其他的协议都是会略过的

### 2.addr
- 接收来自其他节点的地址信息，如果本节点没有存就存下来
``` cpp
class CAddress
{
public:
    uint64 nServices;
    unsigned char pchReserved[12];
    unsigned int ip;
    unsigned short port;

    // disk only
    unsigned int nTime;

    // memory only
    unsigned int nLastFailed; // 对应这个地址最近连接失败时间
}
```

### 3.inv
- 所有类型的类都属于库存，如：
``` cpp
// 消息类型
enum INV_TYPE
{
    MSG_TX = 1,         // 交易消息
    MSG_BLOCK,          // 块信息
    MSG_REVIEW,         //
    MSG_PRODUCT,        // 产品消息
    MSG_TABLE,          // 表
};

static const char* ppszTypeName[] =
{
    "ERROR",
    "tx",
    "block",
    "review",
    "product",
    "table",
};

class CInv
{
public:
    INV_TYPE type;
    uint256 hash;
}
```
- 从某个节点上获取到该节点的全部库存，并对比本地是否存在，不存在即加入一个容器 multimap<**请求时间戳**, CInv> mapAskFor中，之后对通过getdata数据将内容发送出去的
``` cpp
foreach(const CInv& inv, vInv)//遍历所有库存
{
    if (fShutdown)
        return true;
    pfrom->AddInventoryKnown(inv); // 将对应的单库存增加到库存总库中 set<CInv>
    auto AlreadyHave = [](CTxDB& txdb, const CInv& inv) => bool
    {
        switch (inv.type)
        {
        case MSG_TX:        return mapTransactions.count(inv.hash)/*交易map*/ || txdb.ContainsTx(inv.hash);
        case MSG_BLOCK:     return mapBlockIndex.count(inv.hash)/*区块map*/ || mapOrphanBlocks/*孤儿块*/.count(inv.hash);
        case MSG_REVIEW:    return true;
        case MSG_PRODUCT:   return mapProducts/*产品map*/.count(inv.hash);
        }
        // Don't know what it is, just say we already got one
        return true;
    }
    bool fAlreadyHave = AlreadyHave(txdb, inv);//判断库存的类型，并把它加入到他应该属于的容器中

    if (!fAlreadyHave)
        pfrom->AskFor(inv);// 如果不存在，加入一个容器map<hash,time_t> mapAskFor,之后会使用getdata处理的
    else if (inv.type == MSG_BLOCK && mapOrphanBlocks.count(inv.hash))//如果是块库存而且在本地被定义是孤儿块，则发送协议getblocks，参数为全主链的hash列表和当前孤儿块的根
        pfrom->PushMessage("getblocks", CBlockLocator(pindexBest), GetOrphanRoot(mapOrphanBlocks[inv.hash]));
}
```

### 4.getdata
- 获取某一个类型的数据
``` cpp
else if (strCommand == "getdata")
{
    vector<CInv> vInv;
    vRecv >> vInv;//从网络接收缓存中获取对方节点想要获取的库存信息列表

    foreach(const CInv& inv, vInv)
    {
        if (fShutdown)
            return true;

        if (inv.type == MSG_BLOCK)//如果这个库存信息是区块类型
        {
            map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex/*区块map*/.find(inv.hash);
            if (mi != mapBlockIndex.end())//如果本节点有这个区块的信息
            {
                //// could optimize this to send header straight from blockindex for client
                CBlock block;
                block.ReadFromDisk((*mi).second, !pfrom->fClient);//从磁盘上读取此区块的信息
                pfrom->PushMessage("block", block);// 获取数据对应的类型是block，则发送对应的块信息
            }
        }
        else if (inv.IsKnownType())//如果是其他正确的类型
        {
            // Send stream from relay memory
            CRITICAL_BLOCK(cs_mapRelay)
            {
                map<CInv, CDataStream>::iterator mi = mapRelay.find(inv); // 重新转播的内容
                if (mi != mapRelay/*有关某个库存的转发消息*/.end())//如果存在有关本库存的消息，转发给这个节点
                    pfrom->PushMessage(inv.GetCommand()/*这个库存是什么类型的 参考3.ppszTypeName[]*/, (*mi).second);
            }
        }
    }
}
```

### 5.getblocks
- 这个协议的参数为：
``` cpp
networkbuffer << 一条区块hash链为X << 一个区块的hash值为Y
```
- 功能为：取从主链X的链首区块（即创世区块）到Y区块之间的所有区块，通过inv库存协议发送过去
- 特殊情况：如果Y区块不属于X链上，则输出所有的X链上的区块，即告诉对方我这个节点所认准的主链是哪一条

### 6.tx

- 请求交易的信息
- 阅读中，待更新

### 7.review

- 阅读中，待更新

### 8.block

- 阅读中，待更新

### 9.getaddr

- 阅读中，待更新

### 10.checkorder

- 阅读中，待更新

### 11.submitorder

- 提交一个新的交易订单
- 阅读中，待更新

### 12.reply

- 交易结果的回复
- 阅读中，待更新

