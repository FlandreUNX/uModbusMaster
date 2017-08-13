/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmFuncHolding.c
 * This file is part of "uModbusMaster"
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
 
/* ----------------------- Modbus core include files -----------------------------*/
#include "../mbm.h"
#include "./mbmType.h"
#include "../mbmConfig.h"

#if MBM_FUNC_HOLDING_ENABLED == 1

/**
 * @addtogroup 使用例子
 * @note none
 */

/*@{*/

/* ----------------------- uMBM_HoldingReg_Read -----------------------------*/
/**
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
    uint16_t rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvHoldingReg);
  }
 */
 
/* ----------------------- uMBM_HoldingReg_SingleWrite -----------------------------*/
/**
  uMBM_GeneralReqPack_t pack;
  pack.destAddr = 0x01;
  pack.data.value = 0xFF;
  pack.regAddr = 0;
  uMBM_ErrCode_t err = uMBM_HoldingReg_SingleWrite(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (err != MBM_ERR_OK) {
    for (;;);
  }
 */
 
/* ----------------------- uMBM_HoldingReg_MultiWrite -----------------------------*/
/**
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
 */
 
/* ----------------------- uMBM_HoldingReg_MultiReadWrite -----------------------------*/
/**
  uint16_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  
  uMBM_GeneralReqPack_t pack2Tx;
  pack2Tx.destAddr = 0x01;
  pack2Tx.data.length = 2;
  pack2Tx.regAddr = 0;
  pack2Tx.multiData = data;
  
  uMBM_GeneralReqPack_t pack2Rx;
  pack2Rx.data.length = 10;
  pack2Rx.regAddr = 0;
  
  uMBM_ErrCode_t err = uMBM_HoldingReg_MultiReadWrite(uMBM_GetDev(SensorHub, 0), &pack2Tx, &pack2Rx,osWaitForever);
  if (err != MBM_ERR_OK) {
    for (;;);
  }
  else {
    uint16_t rcvHoldingReg[10];
    uint16_t rcvLength = uMBM_GetBuffer_16(uMBM_GetDev(SensorHub, 0), rcvHoldingReg);
  }
 */

/*@}*/


/**
 * @addtogroup 相关定义
 */

/*@{*/

/**
 * Read Holding Req
 * @note none
 */
#define MBM_PDU_REQ_READ_ADDR_OFFSET          (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_READ_REGCNT_OFFSET        (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_READ_SIZE                 (4)

/**
 * Read Holding Func
 * @note none
 */
#define MBM_PDU_FUNC_READ_REGCNT_MAX          (0x007D)
#define MBM_PDU_FUNC_READ_BYTECNT_OFFSET      (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_READ_VALUES_OFFSET       (MBM_PDU_DATA_OFFSET + 1)
#define MBM_PDU_FUNC_READ_SIZE_MIN            (1)

/**
 * Write Holding Req
 * @note none
 */
#define MBM_PDU_REQ_WRITE_ADDR_OFFSET         (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_WRITE_VALUE_OFFSET        (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_WRITE_SIZE                (4)
     
/**
 * Write Holding Func
 * @note none
 */     
#define MBM_PDU_FUNC_WRITE_ADDR_OFFSET        (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_WRITE_VALUE_OFFSET       (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_FUNC_WRITE_SIZE               (4)


/**
 * Multi Write Holding Req
 * @note none
 */     
#define MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET        (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_WRITE_MUL_REGCNT_OFFSET      (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_WRITE_MUL_BYTECNT_OFFSET     (MBM_PDU_DATA_OFFSET + 4)
#define MBM_PDU_REQ_WRITE_MUL_VALUES_OFFSET      (MBM_PDU_DATA_OFFSET + 5)
#define MBM_PDU_REQ_WRITE_MUL_SIZE_MIN           (5)
#define MBM_PDU_REQ_WRITE_MUL_REGCNT_MAX         (0x0078)


/**
 * Multi Read Holding Func
 * @note none
 */     
#define MBM_PDU_FUNC_WRITE_MUL_ADDR_OFFSET       (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_WRITE_MUL_REGCNT_OFFSET     (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_FUNC_WRITE_MUL_SIZE              (4)

/**
 * Multi Read Write Holding Req
 * @note none
 */
#define MBM_PDU_REQ_READWRITE_READ_ADDR_OFFSET      (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_READWRITE_READ_REGCNT_OFFSET    (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_READWRITE_WRITE_ADDR_OFFSET     (MBM_PDU_DATA_OFFSET + 4)
#define MBM_PDU_REQ_READWRITE_WRITE_REGCNT_OFFSET   (MBM_PDU_DATA_OFFSET + 6)
#define MBM_PDU_REQ_READWRITE_WRITE_BYTECNT_OFFSET  (MBM_PDU_DATA_OFFSET + 8)
#define MBM_PDU_REQ_READWRITE_WRITE_VALUES_OFFSET   (MBM_PDU_DATA_OFFSET + 9)
#define MBM_PDU_REQ_READWRITE_SIZE_MIN              (9)              
  
/**
 * Multi Read Write Holding Func
 * @note none
 */  
#define MBM_PDU_FUNC_READWRITE_READ_BYTECNT_OFFSET  (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_READWRITE_READ_VALUES_OFFSET   (MBM_PDU_DATA_OFFSET + 1)
#define MBM_PDU_FUNC_READWRITE_SIZE_MIN             (1)

/*@}*/

/**
 * @addtogroup 相关定义
 * @note 用户使用
 */

/*@{*/

/**
 * 读取从机Holding寄存器
 * @note pack.data.length为待读取数量
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_HoldingReg_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /*1 byte 请求执行方法*/
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_READ_HOLDING_REGISTER;
  
  /*2 bytes 访问寄存器地址*/
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1] = pack->regAddr;
             
  /*2 bytes 访问寄存器数量*/
  PDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET] = pack->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET + 1] = pack->data.length;
             
  /*1Byte (func code) + 2Bytes (dest reg addr) + 2Bytes (reg read length)*/
  mbm_SetTxPDULength(dev, 5);
  
  /*标记发送事件,请求协议栈处理事务*/
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /*等待协议栈发送处理结果*/
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}


/**
 * 往从机写入单个Holding寄存器数据
 * @note pack.data.value为待发送数据
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_HoldingReg_SingleWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /*1 byte 请求执行方法*/
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_WRITE_REGISTER;
  
  /*2 bytes 访问寄存器地址*/
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1] = pack->regAddr;
             
  /*2 bytes 寄存器值*/
  PDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET] = pack->data.value >> 8;
  PDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET + 1] = pack->data.value;
             
  /*1Byte (func code) + 2Bytes (dest reg addr) + 2Bytes (reg read length)*/
  mbm_SetTxPDULength(dev, 5);
  
  /*标记发送事件,请求协议栈处理事务*/
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /*等待协议栈发送处理结果*/
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}


/**
 * 往从机写入多个Holding寄存器数据
 * @note pack.data.length为待发送数据的长度,pack.multiData为待发送数据
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_HoldingReg_MultiWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /*请求执行方法*/
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_WRITE_MULTIPLE_REGISTERS;
  
  /*访问寄存器地址*/
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET + 1] = pack->regAddr;
             
  /*访问寄存器数量*/
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_REGCNT_OFFSET] = pack->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_REGCNT_OFFSET + 1] = pack->data.length;
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_BYTECNT_OFFSET] = pack->data.length * 2;
  
  /*填充待发送数据*/
  PDUFrame += MBM_PDU_REQ_WRITE_MUL_VALUES_OFFSET;
  
  uint16_t index = 0;
  while(pack->data.length > index) {
    *PDUFrame++ = pack->multiData[index] >> 8;
    *PDUFrame++ = pack->multiData[index];
    index += 1;
  }
  
  mbm_SetTxPDULength(dev, MBM_PDU_SIZE_MIN + MBM_PDU_REQ_WRITE_MUL_SIZE_MIN + pack->data.length * 2);
  
  /*标记发送事件,请求协议栈处理事务*/
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /*等待协议栈发送处理结果*/
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}


/**
 * 往从机写入多个Holding寄存器数据并读入多个Holding寄存器数据
 * @note pack.data.length为待发送数据的长度,pack.multiData为待发送数据
 *
 * @param *dev 主机
 * @param *pack2Tx 发送请求包
 * @param *pack2Rx 接收请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_HoldingReg_MultiReadWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack2Tx, uMBM_GeneralReqPack_t *pack2Rx, uint32_t timeout) {
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack2Tx->destAddr;
  
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_READWRITE_MULTIPLE_REGISTERS;
  
  PDUFrame[MBM_PDU_REQ_READWRITE_READ_ADDR_OFFSET] = pack2Rx->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READWRITE_READ_ADDR_OFFSET + 1] = pack2Rx->regAddr;
  
  PDUFrame[MBM_PDU_REQ_READWRITE_READ_REGCNT_OFFSET] = pack2Rx->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_READWRITE_READ_REGCNT_OFFSET + 1] = pack2Rx->data.length;
  
  PDUFrame[MBM_PDU_REQ_READWRITE_WRITE_ADDR_OFFSET] = pack2Tx->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READWRITE_WRITE_ADDR_OFFSET + 1] = pack2Tx->regAddr;
  
  PDUFrame[MBM_PDU_REQ_READWRITE_WRITE_REGCNT_OFFSET] = pack2Tx->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_READWRITE_WRITE_REGCNT_OFFSET + 1] = pack2Tx->data.length;
  PDUFrame[MBM_PDU_REQ_READWRITE_WRITE_REGCNT_OFFSET] = pack2Tx->data.length * 2;
  
  PDUFrame += MBM_PDU_REQ_READWRITE_WRITE_VALUES_OFFSET;
  
  uint16_t index = 0;
  while(pack2Tx->data.length > index) {
    *PDUFrame++ = pack2Tx->multiData[index] >> 8;
    *PDUFrame++ = pack2Tx->multiData[index];
    index += 1;
  }
  
  mbm_SetTxPDULength(dev, MBM_PDU_SIZE_MIN + MBM_PDU_REQ_READWRITE_SIZE_MIN + pack2Tx->data.length * 2);
  
  /*标记发送事件,请求协议栈处理事务*/
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /*等待协议栈发送处理结果*/
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}

/*@}*/

/**
 * @addtogroup mbm holding callback
 * @note 内核调用
 */

/*@{*/

uMBM_ErrCode_t mbm_HoldingReg_Calllback(uMBM_Device_t *dev, uint8_t *src, int16_t address, uint16_t nRegs, uMBM_RegMode_t mode) {
  uint16_t regIndex;
  
  uint16_t *holdingRegBuffer = dev->v16Buffer;
  dev->valueBufferCount = nRegs;
  
  /* it already plus one in modbus function method. */
  address -= 1;
  
  if ((address >= MBM_REG_HOLDING_START) && (address + nRegs <= MBM_REG_HOLDING_START + MBM_REG_HOLDING_NREGS)) {
    regIndex = address - MBM_REG_HOLDING_START;
    
    switch (mode) {
      case (MBM_REG_READ) : {
        while (nRegs > 0) {
          holdingRegBuffer[regIndex] = *src++ << 8;
          holdingRegBuffer[regIndex] |= *src++;
      
          regIndex += 1;
          nRegs -= 1;
        }
      }break;
      
      case (MBM_REG_WRITE) : {
        while (nRegs > 0) {
          holdingRegBuffer[regIndex] = *src++ << 8;
          holdingRegBuffer[regIndex] |= *src++;
      
          regIndex += 1;
          nRegs -= 1;
        }
      }break;
    }
  }
  else {
    return MBM_ERR_NOREG;
  }
  
  return MBM_ERR_OK;
}

/*@}*/


/**
 * @addtogroup mbm 内核调用方法
 * @note 内核调用
 */

/*@{*/

/**
 * 主机调用该函数将分离接收到的Holding寄存器数据到Buffer
 * @note none
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_HoldingReg_Read_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regAddress;
  uint16_t regCount;
  
  /*分割数据*/
  if (rxPDUFramelength >= MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_READ_SIZE_MIN) {
    regAddress = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1]);
    
    regAddress += 1;
    
    regCount = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET] << 8);
    regCount |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET + 1]);
    
    /* Check if the number of registers to read is valid. If not
     * return Modbus illegal data value exception.
     */
    if ((regCount >= 1) && (2 * regCount == rxPDUFrame[MBM_PDU_FUNC_READ_BYTECNT_OFFSET])) {
      /* Make callback to fill the buffer. */
      uMBM_ErrCode_t errCode = mbm_HoldingReg_Calllback(dev, &rxPDUFrame[MBM_PDU_FUNC_READ_VALUES_OFFSET], regAddress, regCount, MBM_REG_READ);
      
      /* If an error occured convert it into a Modbus exception. */
      if (errCode != MBM_ERR_OK) {
        exception = MBM_EX_ILLEGAL_DATA_ADDRESS;
      }
    }
    else {
      exception = MBM_EX_ILLEGAL_DATA_VALUE;
    }
  }
  else {
    exception = MBM_EX_ILLEGAL_DATA_VALUE;
  }
  
  return exception;
}


/**
 * 主机调用该函数将发送后返回的数据分离到Buffer
 * @note HoldingReg SingleWrite
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_HoldingReg_SingleWrite_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regAddress;
  
  if (rxPDUFramelength == (MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_WRITE_SIZE)) {
    regAddress = (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_ADDR_OFFSET + 1]);
    regAddress += 1;
    
    /* Make callback to update the value. */
    uMBM_ErrCode_t errCode = mbm_HoldingReg_Calllback(dev, &rxPDUFrame[MBM_PDU_FUNC_WRITE_VALUE_OFFSET], regAddress, 1, MBM_REG_WRITE);
    
    if (errCode != MBM_ERR_OK) {
      exception = MBM_EX_ILLEGAL_DATA_ADDRESS;
    }
  }
  else {
    exception = MBM_EX_ILLEGAL_DATA_VALUE;
  }
  
  return exception;
}


/**
 * 主机调用该函数将发送后返回的数据分离到Buffer
 * @note HoldingReg MultiWrite
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_HoldingReg_MultiWrite_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regAddress;
  uint16_t regCount;
  uint16_t regByteCount;
  
  if (rxPDUFramelength == (MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_WRITE_MUL_SIZE)) {
    /*获取发送地址*/
    regAddress = (uint16_t)(txPDUFrame[MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET + 1]);
    regAddress += 1;
    
    /*获取返回数据中的寄存器数量*/
    regCount = (uint16_t)(rxPDUFrame[MBM_PDU_REQ_WRITE_MUL_REGCNT_OFFSET] << 8);
    regCount |= (uint16_t)(rxPDUFrame[MBM_PDU_REQ_WRITE_MUL_REGCNT_OFFSET + 1]);
    
    /*获取发送数据中的寄存器数量*/
    regByteCount = txPDUFrame[MBM_PDU_REQ_WRITE_MUL_BYTECNT_OFFSET];
    
    if ((regCount * 2) == regByteCount) {
      uMBM_ErrCode_t errCode = mbm_HoldingReg_Calllback(dev, &txPDUFrame[MBM_PDU_REQ_WRITE_MUL_VALUES_OFFSET], regAddress, regCount, MBM_REG_WRITE);
      
      if (errCode != MBM_ERR_OK) {
        exception = MBM_EX_ILLEGAL_DATA_ADDRESS;
      }
    }
    else {
      exception = MBM_EX_ILLEGAL_DATA_VALUE;
    }
  }
  else {
    exception = MBM_EX_ILLEGAL_DATA_VALUE;
  }
  
  return exception;
}


/**
 * 主机调用该函数将发送后返回的数据分离到Buffer
 * @note HoldingReg MultiReadWrite
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_HoldingReg_MultiReadWrite_Func(void *mbm) {
 uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regReadAddress;
  uint16_t regReadCount;
  
  uint16_t regWriteAddress;
  uint16_t regWriteCount;
  
  if (rxPDUFramelength >= MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_READWRITE_SIZE_MIN) {
    regReadAddress = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_READ_ADDR_OFFSET] << 8);
    regReadAddress |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_READ_ADDR_OFFSET + 1]);
    regReadAddress += 1;
    
    regReadCount = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_READ_REGCNT_OFFSET] << 8);
    regReadCount |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_READ_REGCNT_OFFSET + 1]);
    
    regWriteAddress = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_WRITE_ADDR_OFFSET] << 8);
    regWriteAddress |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_WRITE_ADDR_OFFSET + 1]);
    regWriteAddress++;
    
    regWriteCount = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_WRITE_REGCNT_OFFSET] << 8);
    regWriteCount |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READWRITE_WRITE_REGCNT_OFFSET + 1]);
    
    if ((regReadCount * 2) == rxPDUFrame[MBM_PDU_FUNC_READWRITE_READ_BYTECNT_OFFSET]) {
      uMBM_ErrCode_t errCode = mbm_HoldingReg_Calllback(dev, &txPDUFrame[MBM_PDU_REQ_READWRITE_WRITE_VALUES_OFFSET], regWriteAddress, regWriteCount, MBM_REG_WRITE);
      
      if (errCode == MBM_ERR_OK) {
        errCode = mbm_HoldingReg_Calllback(dev, &rxPDUFrame[MBM_PDU_FUNC_READWRITE_READ_VALUES_OFFSET], regReadAddress, regReadCount, MBM_REG_READ);
      }
      else {
        exception = MBM_EX_ILLEGAL_DATA_ADDRESS;
      }
      
      if (errCode != MBM_ERR_OK) {
        exception = MBM_EX_ILLEGAL_DATA_ADDRESS;
      }
    }
  }
  else {
    exception = MBM_EX_ILLEGAL_DATA_VALUE;
  }
  
  return exception;
}

/*@}*/

#endif
