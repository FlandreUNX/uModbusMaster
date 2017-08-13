# uModbusMaster Rev.C 使用说明

----------

## 1. 简述

uModbusMaster基于Freemodubus协议栈以及[armink/FreeModbus_Slave-Master-RTT-STM32][1]重新编写的.源由是因为armink作者的modbus协议过于庞大,以及大部分可能不会用到master以及slave一起的,所以本版程序将两者进行分离,并只提取出master.对其代码进行美化以及基本全注释(供新手学习).此外对主机模块进行全隔离,能实现单个MCU系统里面实现多个Master主机同时运行,详细见下文所述.

Rev.C版本特征如下:

 - 主机接口与Freemodbus接口基本一致;
 - 采用通用请求包,简化入口变量的复杂;
 - 过程与对象全隔离,单系统多主机的实现;
 - 支持实时系统(本版本基于MDK RTX4)与裸机的移植;
 - 请求采用线程安全的堵塞方式,不会同时访问会出错的情况;
 - 仅支持RTU模式的Master

### 1.1. 文件结构

|源文件                                        |描述   |
|:------------------------------              |:----- |
|Source\uModbusMaster\Core\mbm.c              |给应用层提供Modbus主机设置及轮询相关接口|
|Source\uModbusMaster\Core\mbmFuncColis.c     |主机线圈相关功能|
|Source\uModbusMaster\Core\mbmFuncDisc.c      |主机离散输入相关功能|
|Source\uModbusMaster\Core\mbmFuncHolding.c   |主机保持寄存器相关功能|
|Source\uModbusMaster\Core\mbmFuncInput.c     |主机输入寄存器相关功能|
|Source\uModbusMaster\Core\mbmUtils.c         |一些协议栈中需要用到的小工具|
|Source\uModbusMaster\Core\mbmCRC16.c         |CRC16校验工具|
|Source\uModbusMaster\Port\pmEvent.c          |实现主机事件移植接口|
|Source\uModbusMaster\Port\pmSerial.c         |主机串口移植|
|Source\uModbusMaster\Port\pmTimer.c          |主机定时器移植|
|Source\uModbusMaster\Port\pmContext.c        |主机上下文保护移植|

|源头文件                                       |描述   |
|:------------------------------               |:----- |
|Source\uModbusMaster\mbm.h                    |给应用层提供Modbus主机所有用到的函数|
|Source\uModbusMaster\mbmConfig.h              |用于设置modbus主机|

----------

## 2. 协议栈的移植

对协议栈的移植主要还是两个方面:软件的pmEvent以及pmContext,硬件的pmSerial以及pmTimer;

> 注：本文移植只要是以STM32F072C8t6+KIEL5+RTX_OS环境下的移植,裸奔请根据实际情况来配置;

### 2.1. pmEvent

pmEvent是一个主机对事件进行发送或者接收的方法集合;

对于基于RTOS的移植请务必查看系统是否支持Message和Semaphore,如果支持请手动用别的方法进行模拟或者代替~,可以参考[armink/FreeModbus_Slave-Master-RTT-STM32][1]该作者在RTT下的移植;

详细的文件移植方法本人已经将注释写在了pmEvent.c里面~

### 2.1. pmContext








  [1]: https://github.com/armink/FreeModbus_Slave-Master-RTT-STM32
