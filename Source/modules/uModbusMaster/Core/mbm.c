/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbm.c
 *  Copyright (C) <2018>  <FlandreUNX>
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

/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
/* ----------------------- Modbus core include files -----------------------------*/
#include "../mbm.h"
#include "./mbmPort.h"
#include "./mbmCRC16.h"
#include "../mbmConfig.h"


/**
 * @addtogroup modbus 方法集合
 */

/*@{*/

extern uMBM_Exception_t mbm_InputReg_Read_Func(void *mbm);

extern uMBM_Exception_t mbm_HoldingReg_Read_Func(void *mbm);
extern uMBM_Exception_t mbm_HoldingReg_SingleWrite_Func(void *mbm);
extern uMBM_Exception_t mbm_HoldingReg_MultiWrite_Func(void *mbm);
extern uMBM_Exception_t mbm_HoldingReg_MultiReadWrite_Func(void *mbm);

extern uMBM_Exception_t mbm_Coils_Read_Func(void *mbm);
extern uMBM_Exception_t mbm_Coils_SingleWrite_Func(void *mbm);
extern uMBM_Exception_t mbm_Coils_MultiWrite_Func(void *mbm);

extern uMBM_Exception_t mbm_DiscreteInput_Read_Func(void *mbm);

static const uMBM_FunctionHandler_t mbm_FunctionsHandlers[MBM_FUNC_HANDLERS_MAX] = {
#if MBM_FUNC_INPUT_ENABLED == 1
  {MBM_FUNC_READ_INPUT_REGISTER, mbm_InputReg_Read_Func},
#endif
  
#if MBM_FUNC_HOLDING_ENABLED == 1
  {MBM_FUNC_READ_HOLDING_REGISTER, mbm_HoldingReg_Read_Func},
  {MBM_FUNC_WRITE_REGISTER, mbm_HoldingReg_SingleWrite_Func},
  {MBM_FUNC_WRITE_MULTIPLE_REGISTERS, mbm_HoldingReg_MultiWrite_Func},
  {MBM_FUNC_READWRITE_MULTIPLE_REGISTERS, mbm_HoldingReg_MultiReadWrite_Func},
#endif
  
#if MBM_FUNC_COILS_ENABLED == 1
  {MBM_FUNC_READ_COILS, mbm_Coils_Read_Func},
  {MBM_FUNC_WRITE_SINGLE_COIL, mbm_Coils_SingleWrite_Func},
  {MBM_FUNC_WRITE_MULTIPLE_COILS, mbm_Coils_MultiWrite_Func},
#endif
  
#if MBM_FUNC_DISC_ENABLED == 1
  {MBM_FUNC_READ_DISCRETE_INPUTS, mbm_DiscreteInput_Read_Func}
#endif
};

/*@}*/

/**
 * @addtogroup mbm context functions
 */

/*@{*/

#if MBM_USE_CONTEXT == 1
  extern void pMBM_EnterCriticalSection(void);
  extern void pMBM_ExitCriticalSection(void);
#else 
  #define pMBM_EnterCriticalSection() 
  #define pMBM_ExitCriticalSection() 
#endif

/*@}*/

/**
 * @addtogroup 控制类方法
 */

/*@{*/

/**
 * 初始化modbus master 协议栈
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 * @param *event 事件方法集合
 * @param *serial 串口方法集合
 * @param *timer 定时器方法集合
 *
 * @return uMBM_ErrCode_t 
 */
uMBM_ErrCode_t uMBM_CoreInit(uMBM_Device_t *dev, 
                              pMBM_Event_t *event,
                              pMBM_Serial_t *serial,
                              pMBM_Timer_t *timer) {
  uMBM_ErrCode_t errCode = MBM_ERR_OK;
  
  dev->event = event;
  dev->serial = serial;
  dev->timer = timer;
  
  /*初始化Event引擎*/
  dev->event->pMBM_EventInit();
  if (errCode != MBM_ERR_OK) {
    return errCode;
  }
  dev->event->pMBM_SemaphoreInit();
  if (errCode != MBM_ERR_OK) {
    return errCode;
  }
  
  /*初始化Serial引擎*/
  errCode = dev->serial->pMBM_SerialInit();
  if (errCode != MBM_ERR_OK) {
    return errCode;
  }
  dev->serial->mbm_Device = dev;
                                
  /*初始化Timer引擎*/
  /*T35需要N次的50uS*/
  uint16_t t35_Per50us = 0;
  if (dev->serial->baudRate >= 19200) {
    /* If baudrate > 19200 then we should use the fixed timer values
     * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
     */
    t35_Per50us = 35;
  }
  else {
    /* The timer reload value for a character is given by:
     *
     * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
     *             = 11 * Ticks_per_1s / Baudrate
     *             = 220000 / Baudrate
     * The reload for t3.5 is 1.5 times this value and similary
     * for t3.5.
     */
    t35_Per50us = (7 * 220000) / (2 * dev->serial->baudRate);
  }
  dev->timer->t35_Value = t35_Per50us;
  dev->timer->respondTime_Value = (RESPOND_TIMEOUT_MS * 1000) / 50;
  dev->timer->mbm_Device = dev;
  errCode = dev->timer->pMBM_TimerInit();
  if (errCode != MBM_ERR_OK) {
    return errCode;
  }
  
  /*复位mbm*/
  uMBM_Disable(dev);
  
  return errCode;
}
                              

/**
 * 使能modbus master 协议栈
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 *
 * @return uMBM_ErrCode_t 
 */
uMBM_ErrCode_t uMBM_Enable(uMBM_Device_t *dev) {
  pMBM_EnterCriticalSection();
  
  dev->serial->pMBM_SerialEnable(false, true);
  dev->rxState = MBM_RX_IDLE;
  dev->txState = MBM_TX_IDLE;
  
  pMBM_ExitCriticalSection();
  
  return MBM_ERR_OK;
}


/**
 * 失能modbus master 协议栈
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 *
 * @return uMBM_ErrCode_t 
 */
uMBM_ErrCode_t uMBM_Disable(uMBM_Device_t *dev) {
  pMBM_EnterCriticalSection();
  
  dev->serial->pMBM_SerialDisable();
  dev->timer->pMBM_TimerDisable();
  
  dev->rxState = MBM_RX_IDLE;
  dev->txState = MBM_TX_IDLE;
  
  dev->timeMode = MBM_TMODE_IDLE;
  
  dev->destAddress = 0x00;
  dev->rcvAddress = 0x00;
  
  dev->rxPDULength = 0;
  dev->rxBufferPos = 0;
  
  dev->txBufferLength = 0;
  dev->txBufferPos = NULL;
  dev->txPDULength = 0;
  
  dev->currentErrorCode = MBM_ERR_OK;
  dev->currentException = MBM_EX_NONE;
  
  dev->valueBufferCount = 0;
  
  pMBM_ExitCriticalSection();
  
  return MBM_ERR_OK;
}

/*@}*/

/**
 * @addtogroup 设置类方法
 */

/*@{*/

/**
 * 获取mbm发缓冲中的PDU起止
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 *
 * @return uint8_t*
 */
uint8_t* mbm_GetTxPDUBuffer(uMBM_Device_t *dev) {
  return &(dev->txBuffer[MBM_SL_PDU_PDU_OFFSET]);
}


/**
 * 设置mbm发送缓冲中PDU数据的长度
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 * @param length PDU长度
 *
 * @return none
 */
void mbm_SetTxPDULength(uMBM_Device_t *dev, uint16_t length) {
  dev->txPDULength = length;
}


/**
 * 获取mbm发送缓冲中PDU数据的长度
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 *
 * @return uint8_t
 */
uint8_t mbm_GetTxPDULength(uMBM_Device_t *dev) {
  return dev->txPDULength;
}


/**
 * 将用户事件转换成错误代码
 * @note none
 *
 * @param event 事件
 *
 * @return uMBM_ErrCode_t
 */
uMBM_ErrCode_t uMBM_UserEvent2ErrorCode(uMBM_Event_t event) {
  switch (event) {
    case (MBM_USER_EV_ERROR_SENT) : {
      return MBM_ERR_IO;
    }
    
    case (MBM_USER_EV_RESPOND_TIMEOUT) : {
      return MBM_ERR_TIMEOUT;
    }
    
    case (MBM_USER_EV_ERROR_RECEIVE) : {
      return MBM_ERR_RECVIVE;
    }
    
    case (MBM_USER_EV_ERROR_FUNC_EXECUTE) : {
      return MBM_ERR_EXECUTE;
    }
    
    default : {
    }return MBM_ERR_OK;
  }
}


/**
 * 读取最近的func异常事件
 * @note none
 *
 * @param *dev 主机指针
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t uMBM_GetCurrentException(uMBM_Device_t *dev) {
  return dev->currentException;
}


/**
 * 读取mbm协议栈缓冲的数据数量
 * @note u16
 *
 * @param *dev 主机指针
 *
 * @return [uint8_t]-> 数量
 */
uint16_t uMBM_GetValueBufferCount(uMBM_Device_t *dev) {
  return dev->valueBufferCount;
}


/**
 * 读取mbm协议栈缓冲的数据
 * @note u16
 *
 * @param *dev 主机指针
 * @param *buffer 需要存放的缓冲器
 * @param count 需要获取的数量
 *
 * @return [uint8_t]-> 读取数量
 */
uint16_t uMBM_GetBuffer_16(uMBM_Device_t *dev, uint16_t *buffer, uint16_t count) {
  memcpy(buffer, dev->v16Buffer, count * 2);
  
  return count;
}


/**
 * 读取mbm协议栈缓冲的数据
 * @note u8
 *
 * @param *dev 主机指针
 * @param *buffer 需要存放的缓冲器
 * @param count 需要获取的数量
 *
 * @return [uint16_t]-> 读取数量
 */
uint16_t uMBM_GetBuffer_8(uMBM_Device_t *dev, uint8_t *buffer, uint16_t count) {
  memcpy(buffer, dev->v8Buffer, count);
  
  return count;
}

/*@}*/

/**
 * @addtogroup RTU类方法
 */

/*@{*/

/**
 * 使能mbm发送一个数据帧
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 *
 * @return uMBM_ErrCode_t
 */
uMBM_ErrCode_t mbm_RTUSendStartup(uMBM_Device_t *dev) {
  /*检查mbm接手状态*/
  if (dev->rxState != MBM_RX_IDLE) {
    return MBM_ERR_IO;
  }
  
  pMBM_EnterCriticalSection();
  
  /**
   * 发送总长=destAddr(1) + PDU(dev->txPDULength) + CRC16(2)
   */
  dev->txBufferLength = 0;
  dev->txBufferPos = &dev->txBuffer[MBM_SL_PDU_ADDR_OFFSET];
  
  /*发送首字节为目标地址*/
  dev->txBufferPos[MBM_SL_PDU_ADDR_OFFSET] = dev->destAddress;
  dev->txBufferLength += 1;
  
  dev->txBufferLength += dev->txPDULength;
  
  /*CRC16校验*/
  uint16_t crc16 = mbm_CRC16Calc(dev->txBufferPos, dev->txBufferLength);
  dev->txBufferPos[dev->txBufferLength++] = (uint8_t)(crc16 & 0xFF);
  dev->txBufferPos[dev->txBufferLength++] = (uint8_t)(crc16 >> 8);
  
  /**
   * 标记dev发送状态, 启动tx,关闭rx
   * 打开串口会使能IT_TC,传输数据都在IT_TC中完成
   */
  dev->txState = MBM_TX_XMIT;
  dev->serial->pMBM_SerialEnable(true, false);
  
  pMBM_ExitCriticalSection();
  
  return MBM_ERR_OK;
}


/**
 * mbm底层发送方法
 * @note 该方法被IRQ调用
 *
 * @param *dev mbm设备
 *
 * @return none
 */
void mbm_RTUTxProcess(uMBM_Device_t *dev) {
  switch (dev->txState) {
    /* 发送中 */
    case (MBM_TX_XMIT) : {
      /* IT_TC发生一次发送一字节 */
      if (dev->txBufferLength != 0) {
        dev->serial->pMBM_SerialSendByte(*(dev->txBufferPos));
        
        dev->txBufferPos += 1;
        dev->txBufferLength -= 1;
      }
      /* 发送完毕 */
      else {
        /* 切换状态 */
        dev->txState = MBM_TX_XFWR;
        
        /* 失能发送,使能接收 */
        dev->serial->pMBM_SerialEnable(false, true);

        /* 启动等待回文定时器 */
        dev->timeMode = MBM_TMODE_RESPOND_TIMEOUT;
        dev->timer->pMBM_TimerDisable();
        dev->timer->pMBM_RespondTimerEnable();
      }
    }break;
    
    case (MBM_TX_XFWR) :
    case (MBM_TX_IDLE) : {
      /* 失能发送,使能接收 */
      dev->serial->pMBM_SerialEnable(false, true);
    }break;
  }
}


/**
 * mbm底层接收方法
 * @note 该方法被IRQ调用
 *
 * @param *dev mbm设备
 *
 * @return none
 */
void mbm_RTURxProcess(uMBM_Device_t *dev) {
  uint8_t recByte;
  
  /*拉取数据*/
  recByte = dev->serial->pMBM_SerialReadByte();
  
  /*检查rx状态*/
  switch (dev->rxState) {
    /*接收态*/
    case (MBM_RX_RCV) : {
      dev->timer->pMBM_TimerDisable();
      
      /*防止溢出*/
      if (dev->rxBufferPos < MBM_SL_PDU_SIZE_MAX) {
        /*存放数据*/
        dev->rxBuffer[dev->rxBufferPos++] = recByte;
      }
      /*数据过大,溢出*/
      else {
        /*标记错误*/
        dev->rxState = MBM_RX_ERROR;
      }
      
      /*刷新T3.5*/
      dev->timer->pMBM_T35TimerEnable();
    }break;
    
    /*空闲状态接收到数据,相当于检测到起止帧,进入接收中状态*/
    case (MBM_RX_IDLE) : {
      dev->timer->pMBM_TimerDisable();
      
      /*存放数据*/
      dev->rxBufferPos = 0;
      dev->rxBuffer[dev->rxBufferPos++] = recByte;
      
      /*切换到接受中状态*/
      dev->rxState = MBM_RX_RCV;
      
      /*清空tx状态*/
      dev->txState = MBM_TX_IDLE;
      
      /*启动T3.5定时器*/
      dev->timeMode = MBM_TMODE_T35;
      dev->timer->pMBM_T35TimerEnable();
    }break;
    
    /*接受错误*/
    case (MBM_RX_ERROR) : {
      /*接收错误后依旧接收到数据,但不存储,继续启动T3.5,直到数据接收完*/
      dev->timer->pMBM_TimerDisable();
      dev->timer->pMBM_T35TimerEnable();
    }break;
  }
}


/**
 * mbm底层定时器溢出处理方法
 * @note 该方法被IRQ调用
 *
 * @param *dev mbm设备
 *
 * @return none
 */
void mbm_RTUTimerExpired(uMBM_Device_t *dev) {
  /*关闭定时器*/
  dev->timer->pMBM_TimerDisable();
  dev->timeMode = MBM_TMODE_IDLE;
  
  /**
   * 检查timer超时时的rx状态
   */
  switch (dev->rxState) {
    /*接收完毕,T3.5超时*/
    case (MBM_RX_RCV) : {
      dev->rxState = MBM_RX_IDLE;
      
      dev->event->pMBM_EventPost(MBM_EV_FRAME_RECEIVED);
    }break;
    
    /*接收错误,T3.5超时*/
    case (MBM_RX_ERROR) : {
      dev->rxState = MBM_RX_IDLE;
      
      dev->currentErrorCode = MBM_ERR_RECVIVE;
      
      dev->event->pMBM_EventPost(MBM_EV_ERROR_RECEIVE);
    }break;
    
    case (MBM_RX_IDLE) : {
    }break;
  }
  
  /**
   * 检查timer超时时的tx状态
   */
  switch (dev->txState) {
    /*tx状态为发送完毕*/
    case (MBM_TX_XFWR) : {
      /*定时器模式为等待响应,并超时,说明自从发送后没有收到任何数据*/
      dev->txState = MBM_TX_IDLE;
      
      dev->currentErrorCode = MBM_ERR_TIMEOUT;
      
      /*向主机发送超时事件*/
      dev->event->pMBM_EventPost(MBM_EV_RESPOND_TIMEOUT);
    }break;
    
    case (MBM_TX_IDLE) : {
    }break;
    
    case (MBM_TX_XMIT) : {
    }break;
  }
}


/**
 * mbm 对接收到的数据帧进行预处理
 * @note 主机只能应用于RTU模式
 *
 * @param *dev mbm设备
 *
 * @return uMBM_ErrCode_t
 */
uMBM_ErrCode_t mbm_RTUFrameProcess(uMBM_Device_t *dev) {
  uMBM_ErrCode_t errCode = MBM_ERR_OK;
  
  pMBM_EnterCriticalSection();
  
  /*检查长度以及检查CRC16*/
  if ((dev->rxBufferPos >= MBM_SL_PDU_SIZE_MIN) && (mbm_CRC16Calc(dev->rxBuffer, dev->rxBufferPos) == 0)) {
    /*首字节为从机返回响应地址*/
    dev->rcvAddress = dev->rxBuffer[MBM_SL_PDU_ADDR_OFFSET];
    
    /*检查从机地址是否匹配*/
    if (dev->rcvAddress == dev->destAddress) {
      /*获取PDU接受长度,总长度 - (从机地址1Byte + CRC16校验值2Byte)*/
      dev->rxPDULength = dev->rxBufferPos - MBM_SL_PDU_PDU_OFFSET - MBM_SL_PDU_CRC_SIZE;
    
      /*设置PDU起点*/
      dev->rxPDUFrame = &dev->rxBuffer[MBM_SL_PDU_PDU_OFFSET];
    }
    else {
      errCode = MBM_ERR_CHEKSUM;
    }
  }
  else {
    errCode = MBM_ERR_CHEKSUM;
  }
  
  pMBM_ExitCriticalSection();
  
  return errCode;
}

/*@}*/

/**
 * @addtogroup mbm callback方法
 */

/*@{*/

/**
 * mbm Serial TC中断回调方法
 * @note 该方法被IRQ调用
 *
 * @param *serial mbm 的Serial端口
 *
 * @return none
 */
void pMBM_SerialTC_ISR(pMBM_Serial_t *serial) {
  mbm_RTUTxProcess(serial->mbm_Device);
}


/**
 * mbm Serial RXNE中断回调方法
 * @note 该方法被IRQ调用
 *
 * @param *serial mbm 的Serial端口
 *
 * @return none
 */
void pMBM_SerialRXNE_ISR(pMBM_Serial_t *serial) {
  mbm_RTURxProcess(serial->mbm_Device);
}


/**
 * mbm timer 溢出回调方法
 * @note 该方法被IRQ调用
 *
 * @param *serial mbm 的Serial端口
 *
 * @return none
 */
void pMBM_Timer_ISR(pMBM_Timer_t *timer) {
  mbm_RTUTimerExpired(timer->mbm_Device);
}

/*@}*/

/**
 * @addtogroup mbm core方法
 */

/*@{*/

/**
 * mbm functions 数据分离
 * @note none
 *
 * @param *dev mbm设备
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_FunctionExecute(uMBM_Device_t *dev) {
  uint8_t funcCode = dev->rxPDUFrame[MBM_PDU_FUNC_OFFSET];
  uMBM_Exception_t exception = MBM_EX_ILLEGAL_FUNCTION;
  
   /* If receive frame has exception .The receive function code highest bit is 1.*/
  if (funcCode >> 7) {
    /*截取错误数据*/
    exception = (uMBM_Exception_t) dev->rxPDUFrame[MBM_PDU_DATA_OFFSET];
  }
  else {
    for (uint8_t i = 0; i < MBM_FUNC_HANDLERS_MAX; i++) {
      /*匹配到操作码*/
      if (mbm_FunctionsHandlers[i].functionCode == funcCode) {
        exception = mbm_FunctionsHandlers[i].handler(dev);
        
        break;
      }
      /*没有多余的操作*/
      else if (mbm_FunctionsHandlers[i].functionCode == 0) {
        break;
      }
    }
  }
  
  return exception;
}


/**
 * mbm 核心协议栈
 * @note none
 *
 * @param *dev mbm设备
 *
 * @return uMBM_ErrCode_t
 */
uMBM_ErrCode_t uMBM_Poll(uMBM_Device_t *dev) {
  uMBM_Event_t event = dev->event->pMBM_EventGet();
  switch (event) {
    /*数据请求发送事件*/
    case (MBM_EV_FRAME_SENT) : {
      /*启动发送*/
      dev->currentErrorCode = mbm_RTUSendStartup(dev);
      
      if (dev->currentErrorCode != MBM_ERR_OK) {
        dev->event->pMBM_EventPost(MBM_EV_ERROR_SENT);
      }
    }break;

    /*一帧Frame接收完毕,处理数据*/
    case (MBM_EV_FRAME_RECEIVED) : {
      dev->currentErrorCode = mbm_RTUFrameProcess(dev);
      
      /*校验失败*/
      if (dev->currentErrorCode != MBM_ERR_OK) {
        dev->event->pMBM_EventPost(MBM_EV_ERROR_RECEIVE);
      }
      /*向mbm发送执行命令事件*/
      else {
        dev->event->pMBM_EventPost(MBM_EV_FUNC_EXECUTE);
      }
    }break;
    
    /*处理Frame,对指令进行解析*/
    case (MBM_EV_FUNC_EXECUTE) : {
      dev->currentException = mbm_FunctionExecute(dev);
      
      /*检测到错误码,发送事件*/
      if (dev->currentException != MBM_EX_NONE) {
        dev->event->pMBM_EventPost(MBM_EV_ERROR_FUNC_EXECUTE);
      }
      /*一次请求处理完成*/
      else {
        dev->event->pMBM_UserEventPost(MBM_USER_EV_OK);
      }
    }break;
    
    /*等待从机响应超时*/
    case (MBM_EV_RESPOND_TIMEOUT) : {
      /*向请求者转发操作超时事件*/
      dev->event->pMBM_UserEventPost(MBM_USER_EV_RESPOND_TIMEOUT);
    }break;
    
    /*数据有误*/
    case (MBM_EV_ERROR_RECEIVE) : {
      /*向请求者转发事件*/
      dev->event->pMBM_UserEventPost(MBM_USER_EV_ERROR_RECEIVE);
    }break;
    
    case (MBM_EV_ERROR_FUNC_EXECUTE) : {
      dev->event->pMBM_UserEventPost(MBM_USER_EV_ERROR_FUNC_EXECUTE);
    }break;
    
    case (MBM_EV_ERROR_SENT) : {
      dev->event->pMBM_UserEventPost(MBM_USER_EV_ERROR_SENT);
    }break;
    
    default : {
    }break;
  }
  
  return dev->currentErrorCode;
}

/*@}*/
