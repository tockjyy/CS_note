# 关于怎么一行写shell的疑问

## 例子1
- 有以下文件
``` shell
[2019-10-10 19:08:07]DEBUG|GetOrderInfoMsg|Ret:0 OrderInfo {
    RoleID:50
    OrderID:1234
}
... ...
[2019-10-10 20:08:08]DEBUG|GetOrderInfoMsg|Ret:0 OrderInfo {
    RoleID:50
    OrderID:12345
}
... ...
[2019-10-10 20:08:10]DEBUG|GetOrderInfoMsg|Ret:0 OrderInfo {
    RoleID:60
    OrderID:123456
}
... ...
[2019-10-10 20:10:07]DEBUG|GetOrderInfoMsg|Ret:0 OrderInfo {
    RoleID:50
    OrderID:1234567
}
```
- 想输出如下结果 用户50在2019-10-10 20:00:00之后所有的订单和时间
``` shell
12345   2019-10-10 20:08:08
1234567 2019-10-10 20:10:07
```

## 例子2
- 将目录中与子目录中所有的XX_ios.data重命名为XX.data，功能同下脚本
``` shell
#!/bin/sh

files=`find ./ -name *_ios.data | grep _ios.data`

for val in ${files}
do
	echo mv ${val} ${val/_ios/}
	mv ${val} ${val/ios/}
done

```