/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmFuncInput.c
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
/* ----------------------- Modbus core include files -----------------------------*/
#include "../mbm.h"
#include "./mbmType.h"
#include "../mbmConfig.h"

#if MBM_FUNC_INPUT_ENABLED == 1


/**
 * @addtogroup 使用例子
 * @note none
 */

/*@{*/

/* ----------------------- uMBM_InputReg_Read -----------------------------*/
/**
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
 */

/*@}*/


/**
 * @addtogroup input相关定义
 */

/*@{*/

/**
 * Read Input Func
 * @note none
 */
#define MBM_PDU_REQ_READ_ADDR_OFFSET       (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_READ_REGCNT_OFFSET     (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_READ_SIZE              (4)

/**
 * Read Input Func
 * @note none
 */
#define MBM_PDU_FUNC_READ_BYTECNT_OFFSET        (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_READ_VALUES_OFFSET         (MBM_PDU_DATA_OFFSET + 1)
#define MBM_PDU_FUNC_READ_SIZE_MIN              (1)
#define MBM_PDU_FUNC_READ_RSP_BYTECNT_OFFSET    (MBM_PDU_DATA_OFFSET)

/*@}*/

/**
 * @addtogroup 用户方法
 * @note 用户使用
 */

/*@{*/

/**
 * 读取从机Input寄存器
 * @note none
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t], 操作结果
 */
uMBM_ErrCode_t uMBM_InputReg_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  /* 查询同步锁 */
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /* 获取缓冲器,填充数据 */
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /* 设置目标地址 */
  dev->destAddress = pack->destAddr;
  
  /* 1 byte 请求执行方法 */
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_READ_INPUT_REGISTER;
  
  /* 2 bytes 访问寄存器地址 */
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1] = pack->regAddr;
             
  /* 2 bytes 访问寄存器数量 */
  PDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET] = pack->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET + 1] = pack->data.length;
             
  /* 1Byte (func code) + 2Bytes (dest reg addr) + 2Bytes (reg read length) */
  mbm_SetTxPDULength(dev, MBM_PDU_SIZE_MIN + MBM_PDU_REQ_READ_SIZE);
  
  /* 标记发送事件,请求协议栈处理事务 */
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /* 等待协议栈发送处理结果 */
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}

/*@}*/

/**
 * @addtogroup mbm input callback
 * @note 内核调用
 */

/*@{*/

uMBM_ErrCode_t mbm_InputReg_Read_Calllback(uMBM_Device_t *dev, uint8_t *src, int16_t address, uint16_t nRegs) {
  uint16_t regIndex;
  
  uint16_t *regInputBuffer = dev->v16Buffer;
  dev->valueBufferCount = nRegs;
  
  address -= 1;
  
  if ((address >= MBM_REG_INPUT_START) && (nRegs <= MBM_REG_INPUT_NREGS)) {
    if (address + nRegs <= MBM_REG_INPUT_START + MBM_REG_INPUT_NREGS) {
      regIndex = address - MBM_REG_INPUT_START;
    }
    else {
      regIndex = 0;
    }
    
    while (nRegs > 0) {
      regInputBuffer[regIndex] = *src++ << 8;
      regInputBuffer[regIndex] |= *src++;
      
      regIndex += 1;
      nRegs -= 1;
    }
  }
  else {
    return MBM_ERR_NOREG;
  }
  
  return MBM_ERR_OK;
}

/*@}*/

/**
 * @addtogroup mbm方法
 * @note 内核调用
 */

/*@{*/

/**
 * 主机调用该函数分离接收到的Input寄存器数据
 * @note none
 *
 * @param *mbm, 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_InputReg_Read_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regAddress;
  uint16_t regCount;
  
  
  /* 分割数据 */
  if (rxPDUFramelength >= MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_READ_SIZE_MIN) {
    regAddress = (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1]);
    
    regAddress += 1;
    
    regCount = (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET] << 8);
    regCount |= (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_REGCNT_OFFSET + 1]);
    
    /* Check if the number of registers to read is valid. If not
     * return Modbus illegal data value exception.
     */
    if ((regCount >= 1) && (2 * regCount == rxPDUFrame[MBM_PDU_FUNC_READ_BYTECNT_OFFSET])) {
      /* Make callback to fill the buffer. */
      uMBM_ErrCode_t errCode = mbm_InputReg_Read_Calllback(dev, &rxPDUFrame[MBM_PDU_FUNC_READ_VALUES_OFFSET], regAddress, regCount);
      
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

/*@}*/

#endif
