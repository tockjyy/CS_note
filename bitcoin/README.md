# bitcoin源码阅读笔记

- 以下内容非摘抄，仅仅是自己对1.0版本的源码的阅读看法

## 1.第一步-快速阅读架构
### 1.1.比特币源码中的几个概念
```
区块：每10分钟生成一个新的区块，每个区块都包含比特币值
区块链：区块在程序中是一个反向的单向链表存在的，方便出现分支
交易：每个区块中包含一个交易链，这个链表明了这个区块中比特币的流通记录
钱包：属于本地用户的比特币交易的集合
库存：为一些数据的统一称呼，交易与区块都是库存的一种
节点：P2P网络中其他可以连接上的机器
主链：区块链中最长的一条无分支的链
创世区块：本聪规定的第一个区块，代码里写死的
币基交易：即一个区块里的第一个交易，没有交易输入，为用户挖矿所得
```
- 概念的包含关系
```
区块链 = 多个区块的带分支的链
区块 = 区块内比特币的所有交易
钱包 = 所有本地交易
```

### 1.2.比特币的特性
```
1.每10分钟生成一个新的区块，看谁先算出来，必须算出前一个块才能接着算下一个，否则其他人也不认，如果有别人算出了最新的区块，我也不墨迹了，验证是真的已经被别人算出来之后就开始算下一个区块

2.每一个区块内包含一定量的比特币值，并随着区块数量的增加，币值越来越少，原始比特币数量是50个，每产生210000个区块币值在原来的基础上减少一半，需要的时间是210000*10/(60*24*360)=4.0509...（年）将近于每4年减一半

3.一个新的区块在算出来后是不能被交易的，必须要被网络认可，如何确定是否被认可呢，在这个区块后面如果有120个新的区块生成(10min*120=20hour)，才可以被交易，好像还根据认可度有比特币奖励(有待继续阅读确认)

4.在每一个区块内都包含着一条交易默克尔树，代表了这个区块里的比特币的交易关系

5.用户的标识为一组ECC生成的不对称密钥，用户在网络上公开公钥，私钥保存在本地，交易中会写入公钥和用私钥进行签名的内容，其他用户就可以校验内容的真实性，当无法拥有用户的私钥时，无法伪造用户的任何行为在网络上传播

6.网络节点的发现是依赖chat.freenode.net这个IRC服务的#bitcoin聊天室

7.网络协议上不做任何的数据安全确认，谁说的都往逻辑里走，不过人数多的一方网络扩散要快一些，很容易压倒性优势淘汰搞事情的节点
```
### 1.3.对于比特币的坑点的想法-自问自答
- 问1：如果有的节点只进行POW计算，而不浪费任何算力来帮助其他用户做认证怎么办？
- 答1：如果不做认证，无法确认现在有没有人已经算出了最新节点并得到大多数节点的认可，有可能机器还在算已经过时的节点
- 问2：那我是否只处理有关新区块变化的协议，不处理用户交易的校验呢？
- 答2：这个我觉得有可能哦，不知道内部有没有什么机制杜绝，看代码去也！

## 2.第二部-源码部分精读
### 2.1.发币机制
- 发币挖掘新区块利用了POW(工作证明)，利用了SHA256编码的完全随机性来确保机器必须提供足够的算力才能获取新区块，这方式还真是浪费电啊···
- 初始的计算工作难度为，每10分钟会多开放一个新区块
``` cpp
(uint256)~0 >> 32 = 0x00000000FFFFFFFF......(总共24个F)
``` 
- SHA256计算出来的256位的随机码要小于这个工作难度值时才证明用户算出来了这个区块并通知其他节点
- 即初始计算难度为:1/2^32，这个概率低得可以啊，所以多个节点同时算出区块值的可能性本就趋近于0，就算出现这种情况，在大算力的情况下，有一个节点会巨大的扩散而淘汰其他节点，因为每个节点始终只把最长的链当作主链
- 源码为，从中可以看出，最近两周比特币出产速度与工作难度成正比，但工作难度不能比初始难度简单
``` cpp
static CBigNum bnProofOfWorkLimit = (uint256)~0 >> 32;
// 根据前一个block对应的工作量获取下一个block获取需要的工作量
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast/*区块链尾指针*/)
{
    const unsigned int nTargetTimespan = 14 * 24 * 60 * 60; // 每两周，工作量重新计算一次
    const unsigned int nTargetSpacing = 10 * 60; // 10分钟产生一个block
    // 每隔2016个块对应的工作量难度就需要重新计算一次
    const unsigned int nInterval = nTargetTimespan / nTargetSpacing; // 中间隔了多少个block 2016个块

    // 说明当前块是一个创世区块，因为当前块对应的前一个区块为空 直接返回初始值
    if (pindexLast == NULL)
        return bnProofOfWorkLimit.GetCompact();

    // 如果不等于0不进行工作量难度改变 即没有过两周 工作难度不会重新计算
    if ((pindexLast->nHeight+1) % nInterval != 0)
        return pindexLast->nBits;//上一个区块的工作难度

    // 往前推2016个区块
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < nInterval-1; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // 当前区块的前一个区块创建时间 减去 从当前区块向前推2016个区块得到区块创建时间
    unsigned int nActualTimespan = pindexLast->nTime - pindexFirst->nTime;

    // 控制目标难度调整的跨度不能太大 也不能太小 
    if (nActualTimespan < nTargetTimespan/4)
        nActualTimespan = nTargetTimespan/4;
    if (nActualTimespan > nTargetTimespan*4)
        nActualTimespan = nTargetTimespan*4;

    // 重新目标计算难度：当前区块对应的前一个区块对应的目标难度 * 实际2016区块对应的创建时间间隔 / 目标时间跨度14天
    CBigNum bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

	// 如果计算的工作量难度（值越大对应的工作难度越小）小于当前对应的工作量难度
    if (bnNew > bnProofOfWorkLimit)
        bnNew = bnProofOfWorkLimit;

    return bnNew.GetCompact();
}
```
- 每一个区块所拥有的比特币值不同
``` cpp
// 获取这个区块对应的价值（奖励+交易手续费）
int64 CBlock::GetBlockValue(int64 nFees) const
{
    // 补贴;津贴，初始奖励是50个比特币
    int64 nSubsidy = 50 * COIN;

    // 奖励是每4年减一半，总量是2100万
    // nBestHeight 总区块数量，可以这样理解每产出210000块block则对应的奖励减半，而产生一个block需要10分钟
    // 则产生210000个block需要的时间是 210000*10/(60*24*360)=4.0509...（年） 将近于每4年减一半
    // Subsidy is cut in half every 4 years
    nSubsidy >>= (nBestHeight/*总区块数量*/ / 210000);

    return nSubsidy + nFees;
}
```

### 2.2.交易链结构
- 基本的交易链结构
``` cpp
class CTransaction
{
public:
    vector<CTxIn> vin; // 交易对应的输入 即这个交易的比特币来自于哪些交易？
    vector<CTxOut> vout; // 交易对应的输出 即输入里的交易中拥有的比特币要给谁？
    ... ...
}
class CTxIn
{
public:
    class COutPoint
    {
    public:
        uint256 hash; // 交易对应的hash
        unsigned int n; // 交易对应的第几个输出
    };
    COutPoint prevout; // 前一个交易对应的输出(比特币来源)
    CScript scriptSig; // 输入脚本对应的签名(操作的用户用自己的私钥进行的签名)
    unsigned int nSequence;// 主要是用于判断相同输入的交易哪一个更新，值越大越新
    ... ...
}
class CTxOut
{
public:
    int64 nValue; // 交易输出对应的金额
    CScript scriptPubKey; // 交易对应的公钥(这个比特币要给谁，就用谁的公钥，可以校验下一个输入里的签名是不是这个用户的私有签的)
    ... ...
}
```
- 默克尔树分析
```
阅读中，待更新
```

### 2.3.网络协议

- [跳转](https://github.com/tockjyy/CS_note/bitcoin/Route.md)