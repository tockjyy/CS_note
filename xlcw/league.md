---
title: "test"
output: pdf_document
---

[TOC]

# 联盟系统

## 1.服务器框架

```mermaid
graph TD
Client1 --> xlgateway.zone_1
Client2 --> xlgateway.zone_1
Client3 --> xlgateway.zone_2_1
Client4 --> xlgateway.zone_2_1
xlgateway.zone_1 --> transmit_1
xlgateway.zone_2_1 --> transmit_1
Client5 --> xlgateway.zone_2
Client6 --> xlgateway.zone_2
Client7 --> xlgateway.zone_2_2
Client8 --> xlgateway.zone_2_2
xlgateway.zone_2 --> transmit_2
xlgateway.zone_2_2 --> transmit_2
transmit_2 --> zone_2
transmit_2 --> roomzone_2
transmit_1 --> zone_1
transmit_1 --> roomzone_1
zone_1 --> online_1
roomzone_1 --> online_1
battlezone --> online_1
transmit_1 --> battlezone
transmit_2 --> battlezone
battlezone --> online_2
zone_2 --> online_2
roomzone_2 --> online_2

online_1 --> mailsvr_1
online_1 --> cachesvr_1

online_2 --> mailsvr_2
online_2 --> cachesvr_2

online_1 --> gamedb_1
online_2 --> gamedb_2
mailsvr_1 --> gamedb_1
mailsvr_2 --> gamedb_2
cachesvr_1 --> gamerd_1
cachesvr_2 --> gamerd_2

online_1 --> proxysvr
online_1 --> globalsvr
online_1 --> miscsvr
online_2 --> proxysvr
online_2 --> globalsvr
online_2 --> miscsvr

globalsvr --> globaldb
miscsvr --> platsvr
proxysvr --> loginsvr

```
- xlgateway:网关服务，负责对外的网络交互，支持负载均衡
- transmit:网络包中转服务器，网络包转发，统计网络，排队逻辑
- zone:场景服务器，包含大部分游戏逻辑和静态地图
- roomzone:房间服务器，代码同zone，动态地图
- battlezone:跨服服务器，代码同zone，跨服地图
- online:游戏在线服，处理整个服中的一些全局状态
- mailsvr:邮件服务器，处理邮件的逻辑
- cachesvr:缓存服务器，为redis访问提供异步调用
- xlormsvr:数据库服务器，为mysql访问提供异步调用，图中未画出，每一个连接mysql的连线中间都有一个
- 

## 2.联盟相关

### 2.1 创建联盟

```mermaid
sequenceDiagram
    participant C1 as 队长
    participant C2 as 队员
    participant BZ as 战场服
    participant O as Online
    participant G as Global
    C1->>C2:两个帮主组成队伍
    C1->>BZ:请求创建联盟
    Note Over BZ:判断创建联盟的条件
    BZ-->>C1:返回判断结果
    Note Over BZ:如果满足条件
    BZ->>C1:询问是否确认创建联盟
    BZ->>C2:询问是否确认创建联盟
    C1->>BZ:同意创建联盟
    C2->>BZ:同意创建联盟
    BZ->>G:创建联盟请求
    Note Over G:这里不做配置判断
    G->>BZ:创建联盟成功通知
    BZ->>C1:创建联盟成功通知
    BZ->>C2:创建联盟成功通知
    G->>O:同步新联盟相关的信息
    O->>G:同步归属该联盟的帮会信息
    
```

### 2.2 联盟信息同步机制

```mermaid
sequenceDiagram
    participant BZ as 战场服
    participant O as Online
    participant G as Global
    BZ->>O:d
    
```