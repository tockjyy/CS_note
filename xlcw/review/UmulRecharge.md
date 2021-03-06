# 开服累充review

## 1.针对需求关键点分析
### 1.1.玩家身上活动数据赋值时机
- 只在充值事件时获取活动轮次id和封印等级赋值在玩家身上

### 1.2.活动强制结算时机

- 1.2.1.该活动触发强制结算的时机：
- A.活动事件START（只在begintime时触发）
- B.活动事件END（只在endtime时触发）
- C.玩家登录事件发现无该类型的活动开启
- D.玩家登录事件发现有该类型的活动开启，但是轮次不同

- 1.2.2.该活动触发强制结算的前提条件：
- A.玩家身上活动数据有赋值（重点数据：当前活动的ID和封印等级）

- 1.2.3.该活动触发强制结算所进行的操作：
- A.查看玩家有哪些奖励没有领的，以邮件的形式发送至背包
- B.清空玩家身上活动数据

## 2.代码业务点分析
### 2.1.同服封印等级变化
- （安全）不存在这种情况，活动的封印等级都是服务器加载配置的时候根据活动开启的日期算出该时刻的封印等级写在活动容器中的
- 所以在同服的状态下，封印等级是不会随时间的流逝而变化的（详情参考新的运营活动触发，作者：陈启祥）

### 2.2.合服情况下，母服没开，子服开启（安全）
- 合服后，配置用的是母服，子服活动自行结算，走的流程是上述(1.2.1.C)
  
### 2.3.合服情况下，母服与子服开启，不同轮次ID（安全）
- 合服后，配置用的是母服，子服活动自行结算，走的流程是上述(1.2.1.D)

### 2.4.合服情况下，母服与子服开启，同轮次ID（安全）
- 2.4.1.子服和母服活动开启时间的封印等级都是相同（参考：2.1），子服和母服的玩家都正常活动即可，不影响
- 2.4.2.子服和母服活动开启时间的封印等级不相同（参考：2.1），母服的玩家都正常活动即可，不影响，子服玩家又分以下两种状态：
- A.合服前，若玩家没有参与过活动（即充值），没有触发(1.1)，这个子服的玩家会使用母服的封印等级配置参与活动
- B.合服前，若玩家参与过活动（即充值），触发了(1.1)，这个子服的玩家会使用子服事先存在玩家身上的封印等级配置参与活动

### 2.2.合服情况下，母服开启，子服没开（安全）
- 合服后，母服玩家正常以前的活动，子服玩家新参与活动


## 3.代码逻辑检查
- 1.设置玩家身上封印等级的时候可以考虑打个日志，既触发(1.1)时，虽然从充值记录能推算出来，加一个方便查找