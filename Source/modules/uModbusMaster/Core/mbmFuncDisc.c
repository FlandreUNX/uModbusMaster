/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmFuncDisc.c
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


#if MBM_FUNC_DISC_ENABLED == 1

/**
 * @addtogroup 相关定义
 * @note Private
 */

/*@{*/

/**
 * Read Disc Input Req
 * @note none
 */
#define MBM_PDU_REQ_READ_ADDR_OFFSET       (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_READ_DISCCNT_OFFSET    (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_READ_SIZE              (4)

/**
 * Read Disc Input Func
 * @note none
 */
#define MBM_PDU_FUNC_READ_DISCCNT_OFFSET   (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_READ_VALUES_OFFSET    (MBM_PDU_DATA_OFFSET + 1)
#define MBM_PDU_FUNC_READ_SIZE_MIN         (1)

/*@}*/

/**
 * @addtogroup 用户方法
 * @note Public
 */

/*@{*/

/**
 * 读取从机DiscreteInput寄存器数据
 * @note [Rev.1] pack.data.length为待读取数量
 *
 * @param *dev, 主机
 * @param *pack, 目标请求包
 * @param timout, 超时值
 *
 * @return uMBM_ErrCode_t, 操作结果
 */
uMBM_ErrCode_t uMBM_DiscreteInput_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  /* 查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /* 获取缓冲器,填充数据 */
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /* 设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /* 1 byte 请求执行方法 */
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_READ_DISCRETE_INPUTS;
  
  /* 2 bytes 访问寄存器地址 */
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1] = pack->regAddr;
             
  /* 2 bytes 访问寄存器数量 */
  PDUFrame[MBM_PDU_REQ_READ_DISCCNT_OFFSET] = pack->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_READ_DISCCNT_OFFSET + 1] = pack->data.length;
             
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
 * @addtogroup mbm Discrete Input callback
 * @note Private
 */

/*@{*/

uMBM_ErrCode_t mbm_DiscreteInput_Read_Calllback(uMBM_Device_t *dev, uint8_t *src, int16_t address, uint16_t nDiscrete) {
  uint16_t nReg = nDiscrete / 8 + 1;
  
  uint16_t regIndex;
  uint16_t regBitIndex;
  
  uint8_t *regBuffer = dev->v8Buffer;
  dev->valueBufferCount = nDiscrete;
  
  address -= 1;
  
  if ((address >= MBM_DISCRETE_INPUT_START) && (address + nDiscrete <= MBM_DISCRETE_INPUT_START + MBM_DISCRETE_INPUT_NDISCRETES)) {
    regIndex = (uint16_t) (address - MBM_DISCRETE_INPUT_START) / 8;
    regBitIndex = (uint16_t) (address - MBM_DISCRETE_INPUT_START) % 8;
    
    while (nReg > 1) {
      uMBM_Util_SetBits(&regBuffer[regIndex++], regBitIndex, 8, *src++);
      nReg -= 1;
    }
    
    /* last discrete */
    nDiscrete = nDiscrete % 8;
    
    /* xMBUtilSetBits has bug when ucNBits is zero */
    if (nDiscrete != 0) {
      uMBM_Util_SetBits(&regBuffer[regIndex++], regBitIndex, nDiscrete, *src++);
    }
  }
  else {
    return MBM_ERR_NOREG;
  }
  
  return MBM_ERR_OK;
}

/*@}*/

/**
 * @addtogroup mbm Discrete Input Func
 * @note Private
 */

/*@{*/

/**
 * 主机调用该函数分离接收到的Discrete寄存器
 * @note [Rev.1]
 *
 * @param *mbm, 主机
 *
 * @return uMBM_Exception_t,
 */
uMBM_Exception_t mbm_DiscreteInput_Read_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *) mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regAddress;
  uint16_t discreteCount;
  uint8_t byteCount;
  
  if (rxPDUFramelength >= MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_READ_SIZE_MIN) {
    regAddress = (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1]);
    regAddress++;
    
    discreteCount = (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_DISCCNT_OFFSET] << 8);
    discreteCount |= (uint16_t) (txPDUFrame[MBM_PDU_REQ_READ_DISCCNT_OFFSET + 1]);
    
    /**
     * Test if the quantity of coils is a multiple of 8. If not last
     * byte is only partially field with unused coils set to zero. 
     */
    if((discreteCount & 0x0007 ) != 0) {
      byteCount = (uint8_t) (discreteCount / 8 + 1);
    }
    else {
      byteCount = (uint8_t) (discreteCount / 8);
    }
    
    /**
     * Check if the number of registers to read is valid. If not
     * return Modbus illegal data value exception. 
     */
    if ((discreteCount >= 1) && byteCount == rxPDUFrame[MBM_PDU_FUNC_READ_DISCCNT_OFFSET]) {
      /* Make callback to fill the buffer. */
			uMBM_ErrCode_t errCode = mbm_DiscreteInput_Read_Calllback(dev, &rxPDUFrame[MBM_PDU_FUNC_READ_VALUES_OFFSET], regAddress, discreteCount);
      
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
