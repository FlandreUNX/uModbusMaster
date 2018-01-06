# uModbusMaster Rev.D 使用说明

----------
```C
/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
```
## 1. 简述

uModbusMaster基于Freemodubus协议栈以及[armink/FreeModbus_Slave-Master-RTT-STM32][1]重新编写的.源由是因为armink作者的modbus协议过于庞大,以及大部分可能不会用到master以及slave一起的,所以本版程序将两者进行分离,并只提取出master.对其代码进行美化以及基本全注释(供新手学习).此外对主机模块进行全隔离,能实现单个MCU系统里面实现多个Master主机同时运行,详细见下文所述.

Rev.D版本更新如下:

  - 修正uMBM_GetBuffer_16/uMBM_GetBuffer_8读取可能的错误,增加读取数量入口(PS:没有越界,请注意count的大小有没有越界)
  - 增加uMBM_GetValueBufferCount方法,获取当前valueBuffer存放了多少有效数据
  - 修改使用说明,如下文所示
  
Rev.C版本特征如下:

 - 主机接口与Freemodbus接口基本一致;
 - 采用通用请求包,简化入口变量的复杂;
 - 过程与对象全隔离,单系统多主机的实现;
 - 支持实时系统(本版本基于MDK RTX4)与裸机的移植;
 - 请求采用线程安全的堵塞方式,不会同时访问会出错的情况;
 - 仅支持RTU模式的Master
 - 几乎全中文注释,一行指令一个注释,非常适合新手学习

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

> 注：本程序的例程移植是以STM32F072C8t6+KIEL5+RTX_OS环境下的移植,裸奔请根据实际情况来配置

### 2.1. pmEvent

pmEvent是一个主机对事件进行发送或者接收的方法集合;

对于基于RTOS的移植请务必查看系统是否支持Message和Semaphore,如果支持请手动用别的方法进行模拟或者代替~,可以参考[armink/FreeModbus_Slave-Master-RTT-STM32][1]该作者在RTT下的移植;

详细的文件移植方法本人已经将注释写在了pmEvent.c里面~

### 2.2. pmContext

pmContext是一个主机调用上下文保护的方法集合;

基于RTOS的事务,可以将RTOS里面的保护区进出代码放进去,如果不需要可以在mbmConfig.h里面关闭该保护;

详细的文件移植方法本人已经将注释写在了pmContext.c里面~

### 2.3. pmSerial

pmSerial是一个主机发送接收来自串口数据的底层方法集合;涉及串口所有的方法都在改文件中;用户只需要在该文件下按照例程来进行修改即可.
因为uModbusMaster的串口发送时依靠USART_TC的中断发生的;如果其他MCU没有这功能,请另外将TC中断中的调用方法找另外处理;

> 注：需要在该文档的最后添加串口IRQ服务,详细方法参考例程;

### 2.4. pmTimer

pmTimer是主机收发超时需要的底层方法集合;本例程采用一个硬件定时来完成,如果需要用到RTOS的定时器可以一参考armink原作者的做法;

pmTimer依赖一个硬件定时器.该定时器的周期必须要50us增加一次ARR值,其余频率的MCU请根据该值来修改;

> 注：发送回应超时值在mbmConfig.h里面可以配置;

> 注：定时器不推荐开启ARR自动装载功能,因为该功能可能会打乱或者延长相当长的时间来触发Respond Timout或者T3.5超时,严重影响性能;

## 3. API

uModbusMaster 采用一个专用守护线程来维持数据事件的处理;里面所有设计传输的方法都是**线程安全|堵塞模式**的.传输函数都能设定相关的超市值,若未能得到相应的结果会返回错误供用户处理.

传输函数返回的各种错误信息都在mbmType.h中有注释,以下是简单的阐述一下:

|返回值                                |描述|
|:-----                               |:----|
|MBM_ERR_OK                           |正常，没错误|
|MBM_ERR_BUSY                         |主机忙,在设定的时间内，没有请求没有被发送|
|MBM_ERR_TIMEOUT                      |响应超时,主机在设定的时间内未收到从机响应|
|MBM_ERR_RECVIVE                      |接收数据出错|
|MBM_ERR_NOREG                        |寄存器、线圈或离散输入地址出错|
|MBM_ERR_EXECUTE                      |主机收到响应后，执行Modbus方法（功能）出错|
|MBM_ERR_ILL_ARG                      |入参格式有误|

----------

本程序额外设计了一个通用请求包,用于简化操作,该包说明下:

|uMBM_GeneralReqPack_t                |描述|
|:-----                               |:----|
|destAddr          |请求目标地址|
|regAddr           |请求目标的寄存器地址|
|data.length       |请求目标数据长度|
|data.value        |目标数据,用于发送一个数据给从机|
|multiData         |型数据指针,用于一次性发送大量数据给从机|

----------

> 注：其中data为一个union类型的值,详细的值请根据不同的交互API来选择;

### 3.1. 保持寄存器

以下是保存寄存相关的API,以例子的形势来说明,如果需要知道入口变量的情况,请在相应的代码中查看,都有详细的中文注释

***uMBM_HoldingReg_Read***
```C
/* ----------------------- uMBM_HoldingReg_Read -----------------------------*/
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.data.length = 10;
  pack.regAddr = 0;
  uMBM_ErrCode_t err = uMBM_HoldingReg_Read(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (err != MBM_ERR_OK) {
    for (;;);
  }
  else {
    uint16_t rcvHoldingReg[10];
    /* 采用获取缓存数量读取 */
    uint16_t valueBufferCount = uMBM_GetValueBufferCount(uMBM_GetDev(SensorHub, 0);
    uint16_t rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvHoldingReg, valueBufferCount));
    /* 采用固定数量获取 */
    uint16_t rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvHoldingReg, 10));
  }
```

***uMBM_HoldingReg_SingleWrite***
```C
/* ----------------------- uMBM_HoldingReg_SingleWrite -----------------------------*/
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.data.value = 0xFF;
  pack.regAddr = 0;
  uMBM_ErrCode_t err = uMBM_HoldingReg_SingleWrite(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (err != MBM_ERR_OK) {
    for (;;);
  }
```

***uMBM_HoldingReg_MultiWrite***
```C
/* ----------------------- uMBM_HoldingReg_MultiWrite -----------------------------*/
  uint16_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.data.length = 2;
  pack.regAddr = 0;
  pack.multiData = data;
  uMBM_ErrCode_t err = uMBM_HoldingReg_MultiWrite(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (err != MBM_ERR_OK) {
    for (;;);
  }
```


### 3.2. 输入寄存器

以下是输入寄存相关的API,以例子的形势来说明,如果需要知道入口变量的情况,请在相应的代码中查看,都有详细的中文注释

***uMBM_InputReg_Read***
```C
/* ----------------------- uMBM_InputReg_Read -----------------------------*/
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.data.length = 10;
  pack.regAddr = 0;
  uMBM_ErrCode_t err = uMBM_InputReg_Read(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (err != MBM_ERR_OK) {
    for (;;);
  }
  else {
    uint16_t rcvInputReg[10];
    uint16_t rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvInputReg, 10);
  }
```

### 3.3. 线圈寄存器

以下是线圈寄存相关的API,以例子的形势来说明,如果需要知道入口变量的情况,请在相应的代码中查看,都有详细的中文注释

***uMBM_Colis_Read***
```C
/* ----------------------- uMBM_Colis_Read -----------------------------*/
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.regAddr = 0;
  pack.data.length = 0x01;
  uint8_t coilDataRcv;
  uint8_t coilValue;
  uMBM_ErrCode_t errCode = uMBM_Coils_Read(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (errCode != MBM_ERR_OK) {
    for (;;);
  }
  else {
    uMBM_GetBuffer_8(uMBM_GetDev(SensorHub, 0), &coilDataRcv, 1);
    coilValue = uMBM_Util_GetBits(&coilDataRcv, 0, 1);
  }
```

***uMBM_Coils_SingleWrite***
```C
/* ----------------------- uMBM_Coils_SingleWrite -----------------------------*/
  uMBM_GeneralReqPack_t pack;
  uMBM_ErrCode_t errCode;
  pack.destAddr = 0x01;
  pack.regAddr = 0;
  pack.data.value = 0xFF00;

  errCode = uMBM_Coils_SingleWrite(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (errCode != MBM_ERR_OK) {
    for (;;);
  }
```

***uMBM_Coils_MultiWrite***
```C
/* ----------------------- uMBM_Coils_MultiWrite -----------------------------*/
  uint8_t colisValue[1] = {0xFE};
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.regAddr = 0;
  pack.data.length = 8;
  pack.multiData = (uint16_t *)&colisValue;
  uMBM_ErrCode_t errCode = uMBM_Coils_MultiWrite(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (errCode != MBM_ERR_OK) {
    for (;;);
  }
```

### 3.4. 离散寄存器

以下是离散寄存相关的API,以例子的形势来说明,如果需要知道入口变量的情况,请在相应的代码中查看,都有详细的中文注释

***uMBM_DiscreteInput_Read***
```C
/* ----------------------- uMBM_DiscreteInput_Read -----------------------------*/
  uint8_t coilDataRcv;
  uint8_t coilValue;
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.regAddr = 0;
  pack.data.length = 1;
  uMBM_ErrCode_t errCode = uMBM_DiscreteInput_Read(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (errCode != MBM_ERR_OK) {
    for (;;);
  }
  else {
    uMBM_GetBuffer_8(uMBM_GetDev(SensorHub, 0), &coilDataRcv, 1);
    coilValue = uMBM_Util_GetBits(&coilDataRcv, 0, 1);
  }
```

## 4. modbus主机的使用

本程序是采用数据逻辑全隔离的方式编写的,所以能在多线程里面实现多主机的特点

### 4.1 单例主机生成

在一个全局变量里面申明一个主机,并将他放在一个线程中运行

**th_ModbusMaster_1.c**
```C
#include "../modules/uModbusMaster/mbm.h"
uMBM_CreateDev(SensorHub, 0);

osThreadId modbusMaster_ThreadID;

void modbusMaster_Thread(void const *arg) {
  for (;;) {
    uMBM_Poll(uMBM_GetDev(SensorHub, 0));
  }
}
osThreadDef(modbusMaster_Thread, osPriorityNormal , 1, 0);

void modbusMaster_ThreadStart(void) {
  extern pMBM_Event_t mbm_Event;
  extern pMBM_Serial_t mbm_Serial;
  extern pMBM_Timer_t mbm_Timer;
  uMBM_CoreInit(uMBM_GetDev(SensorHub, 0), &mbm_Event, &mbm_Serial, &mbm_Timer);
  uMBM_Enable(uMBM_GetDev(SensorHub, 0));
  
  modbusMaster_ThreadID = osThreadCreate(osThread(modbusMaster_Thread), NULL);
}
```

### 4.1 单例主机的使用

** 基于CMSIS-RTX5 **

在一个**th_ModbusMaster_1.c**中生成一个主机并且将他启动,接着我们可以在别的地方对他进行访问或者使用他来收发

**th_Main.c**
```C
#include "../modules/uModbusMaster/mbm.h"
uMBM_ExternDev(SensorHub, 0);

uint16_t rcvLength;

int main(void) {
  osThreadSetPriority(osThreadGetId(), osPriorityRealtime);

  extern void modbusMaster_ThreadStart(void);
  modbusMaster_ThreadStart();
  
  for (;;) {
    osDelay(500);
    
    uMBM_GeneralReqPack_t pack;
    pack.destAddr = 0x01;
    pack.data.length = 10;
    pack.regAddr = 0;
    
    uMBM_ErrCode_t err = uMBM_InputReg_Read(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
    if (err != MBM_ERR_OK) {
      for (;;);
    }
    else {
      
      rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvInputReg);
    }
  }
}

```

### 4.2 多例主机的使用

** 基于CMSIS-RTX5 **

如果我们使用多个主机可以按照以下的方式来操作
**th_ModbusMaster.c**
```C
#include "../modules/uModbusMaster/mbm.h"

/**
 * SensorHub_0主机守护线程
 */

uMBM_CreateDev(SensorHub, 0);

osThreadId SensorHub_0_ThreadID;
void SensorHub_0_Thread(void const *arg) {
  for (;;) {
    uMBM_Poll(uMBM_GetDev(SensorHub, 0));
  }
}
osThreadDef(SensorHub_0_Thread, osPriorityNormal , 1, 0);

void SensorHub_0_ThreadStart(void) {
  extern pMBM_Event_t mbm_Event;
  extern pMBM_Serial_t mbm_Serial;
  extern pMBM_Timer_t mbm_Timer;
  uMBM_CoreInit(uMBM_GetDev(SensorHub, 0), &mbm_Event, &mbm_Serial, &mbm_Timer);
  uMBM_Enable(uMBM_GetDev(SensorHub, 0));
  
  SensorHub_0_ThreadID = osThreadCreate(osThread(SensorHub_0_Thread), NULL);
}

/**
 * SensorHub_1主机守护线程
 */

uMBM_CreateDev(SensorHub, 1);

osThreadId SensorHub_1_ThreadID;
void SensorHub_1_Thread(void const *arg) {
  for (;;) {
    uMBM_Poll(uMBM_GetDev(SensorHub, 1));
  }
}
osThreadDef(SensorHub_1_Thread, osPriorityNormal , 1, 0);

void SensorHub_1_ThreadStart(void) {
  extern pMBM_Event_t mbm_Event;
  extern pMBM_Serial_t mbm_Serial;
  extern pMBM_Timer_t mbm_Timer;
  uMBM_CoreInit(uMBM_GetDev(SensorHub, 1), &mbm_Event, &mbm_Serial, &mbm_Timer);
  uMBM_Enable(uMBM_GetDev(SensorHub, 1));
  
  SensorHub_1_ThreadID = osThreadCreate(osThread(SensorHub_1_Thread), NULL);
}

/**
 * 主线程
 */

uint16_t rcvLength;

int main(void) {
  osThreadSetPriority(osThreadGetId(), osPriorityRealtime);

  extern void SensorHub_0_ThreadStart(void);
  SensorHub_0_ThreadStart();
  
  extern void SensorHub_1_ThreadStart(void);
  SensorHub_1_ThreadStart();
  
  for (;;) {
    osDelay(500);
    
     uMBM_GeneralReqPack_t pack;
    pack.destAddr = 0x01;
    pack.data.length = 10;
    pack.regAddr = 0;
    
    /**
     * 对SensorHub_0主机进行操作
     */
    uMBM_ErrCode_t err = uMBM_InputReg_Read(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
    if (err != MBM_ERR_OK) {
      for (;;);
    }
    else {
      rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvInputReg);
    }
    
    
    /**
     * 对SensorHub_1主机进行操作
     */
    uMBM_ErrCode_t err = uMBM_InputReg_Read(uMBM_GetDev(SensorHub, 1), &pack, osWaitForever);
    if (err != MBM_ERR_OK) {
      for (;;);
    }
    else {
      rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 1), rcvInputReg);
    }
  }
}

```



# 其他

如果各位看官喜欢该程序,可以给我点个赞star或者Fork;本程序无偿使用,但是**请标明程序出处**,谢谢合作;感谢[armink][2]作者的源码的贡献!












  [1]: https://github.com/armink/FreeModbus_Slave-Master-RTT-STM32
  [2]: https://github.com/armink
