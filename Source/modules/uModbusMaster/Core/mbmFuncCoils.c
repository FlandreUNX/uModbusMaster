/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmFuncCoils.c
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
#include "./mbmUtils.h"
#include "../mbmConfig.h"

#if MBM_FUNC_COILS_ENABLED == 1

/**
 * @addtogroup 使用例子
 * @note none
 */

/*@{*/

/* ----------------------- uMBM_Colis_Read -----------------------------*/
/**
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
    uMBM_GetBuffer_8(uMBM_GetDev(SensorHub, 0), &coilDataRcv);
    coilValue = uMBM_Util_GetBits(&coilDataRcv, 0, 1);
  }
 */
 
/* ----------------------- uMBM_Coils_SingleWrite -----------------------------*/
/**
  uMBM_GeneralReqPack_t pack;
  uMBM_ErrCode_t errCode;
  pack.destAddr = 0x01;
  pack.regAddr = 0;
  pack.data.value = 0xFF00;

  errCode = uMBM_Coils_SingleWrite(uMBM_GetDev(SensorHub, 0), &pack, osWaitForever);
  if (errCode != MBM_ERR_OK) {
    for (;;);
  }
 */
 
/* ----------------------- uMBM_Coils_MultiWrite -----------------------------*/
/**
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
 */

/*@}*/

/**
 * @addtogroup 相关定义
 */

/*@{*/

/**
 * Read Coils Req
 * @note none
 */
#define MBM_PDU_REQ_READ_ADDR_OFFSET            (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_REQ_READ_COILCNT_OFFSET         (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_READ_SIZE                   (4)

/**
 * Read Coils Func
 * @note none
 */
#define MBM_PDU_FUNC_READ_COILCNT_OFFSET        (MBM_PDU_DATA_OFFSET + 0)
#define MBM_PDU_FUNC_READ_VALUES_OFFSET         (MBM_PDU_DATA_OFFSET + 1)
#define MBM_PDU_FUNC_READ_SIZE_MIN               (1)
       
/**
 * Write Coils Req
 * @note none
 */       
#define MBM_PDU_REQ_WRITE_ADDR_OFFSET           (MBM_PDU_DATA_OFFSET)
#define MBM_PDU_REQ_WRITE_VALUE_OFFSET          (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_WRITE_SIZE                  (4)

/**
 * Write Coils Func
 * @note none
 */
#define MBM_PDU_FUNC_WRITE_ADDR_OFFSET          (MBM_PDU_DATA_OFFSET)
#define MBM_PDU_FUNC_WRITE_VALUE_OFFSET         (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_FUNC_WRITE_SIZE                  (4)
      
/**
 * Multi Write Coils Req
 * @note none
 */      
#define MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET       (MBM_PDU_DATA_OFFSET)
#define MBM_PDU_REQ_WRITE_MUL_COILCNT_OFFSET    (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_REQ_WRITE_MUL_BYTECNT_OFFSET    (MBM_PDU_DATA_OFFSET + 4)
#define MBM_PDU_REQ_WRITE_MUL_VALUES_OFFSET     (MBM_PDU_DATA_OFFSET + 5)
#define MBM_PDU_REQ_WRITE_MUL_SIZE_MIN          (5)
#define MBM_PDU_REQ_WRITE_MUL_COILCNT_MAX       (0x07B0)

/**
 * Multi Write Coils Func
 * @note none
 */    
#define MBM_PDU_FUNC_WRITE_MUL_ADDR_OFFSET      (MBM_PDU_DATA_OFFSET)
#define MBM_PDU_FUNC_WRITE_MUL_COILCNT_OFFSET   (MBM_PDU_DATA_OFFSET + 2)
#define MBM_PDU_FUNC_WRITE_MUL_SIZE             (5)

/*@}*/

/**
 * @addtogroup 用户方法
 * @note 用户使用
 */

/*@{*/

/**
 * 读取从机Coils寄存器数据
 * @note pack.data.length为待读取数量
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_Coils_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /*1 byte 请求执行方法*/
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_READ_COILS;
  
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1] = pack->regAddr;
             
  PDUFrame[MBM_PDU_REQ_READ_COILCNT_OFFSET] = pack->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_READ_COILCNT_OFFSET + 1] = pack->data.length;
             
  mbm_SetTxPDULength(dev, MBM_PDU_SIZE_MIN + MBM_PDU_REQ_READ_SIZE);
  
  /*标记发送事件,请求协议栈处理事务*/
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /*等待协议栈发送处理结果*/
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}


/**
 * 写从机Coils寄存器
 * @note pack.data.value为待写数据
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_Coils_SingleWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  if ((pack->data.value != 0xFF00) && (pack->data.value != 0x0000)) {
    return MBM_ERR_ILL_ARG;
  }
  
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /*1 byte 请求执行方法*/
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_WRITE_SINGLE_COIL;
  
  /*2 bytes 访问寄存器地址*/
  PDUFrame[MBM_PDU_REQ_WRITE_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_WRITE_ADDR_OFFSET + 1] = pack->regAddr;
             
  /*2 bytes 访问寄存器值*/
  PDUFrame[MBM_PDU_REQ_WRITE_VALUE_OFFSET] = pack->data.value >> 8;
  PDUFrame[MBM_PDU_REQ_WRITE_VALUE_OFFSET + 1] = pack->data.value;
             
  mbm_SetTxPDULength(dev, MBM_PDU_SIZE_MIN + MBM_PDU_REQ_WRITE_SIZE);
  
  /*标记发送事件,请求协议栈处理事务*/
  dev->event->pMBM_EventPost(MBM_EV_FRAME_SENT);
  
  /*等待协议栈发送处理结果*/
  uMBM_Event_t event = dev->event->pMBM_UserEventGet();

  dev->event->pMBM_SemaphoreRelease();
  
  return uMBM_UserEvent2ErrorCode(event);
}


/**
 * 多个数据写从机Coils寄存器
 * @note pack.data.length为待发送数据的长度,pack.multiData为待发送数据
 *
 * @param *dev 主机
 * @param *pack 目标请求包
 * @param timout 超时值
 *
 * @return [uMBM_ErrCode_t] -> 操作结果
 */
uMBM_ErrCode_t uMBM_Coils_MultiWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout) {
  if (pack->data.length > MBM_PDU_REQ_WRITE_MUL_COILCNT_MAX) {
    return MBM_ERR_ILL_ARG;
  }
  
  /*查询同步锁*/
  if (!dev->event->pMBM_SemaphoreWait(timeout)) {
    return MBM_ERR_BUSY;
  }
  
  /*获取缓冲器,填充数据*/
  uint8_t *PDUFrame = mbm_GetTxPDUBuffer(dev);
  
  /*设置目标地址*/
  dev->destAddress = pack->destAddr;
  
  /*1 byte 请求执行方法*/
  PDUFrame[MBM_PDU_FUNC_OFFSET] = MBM_FUNC_WRITE_MULTIPLE_COILS;
  
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET] = pack->regAddr >> 8;
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_ADDR_OFFSET + 1] = pack->regAddr;
             
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_COILCNT_OFFSET] = pack->data.length >> 8;
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_COILCNT_OFFSET + 1] = pack->data.length;
  
  uint8_t byteCount;
  if ((pack->data.length & 0x0007) != 0) {
    byteCount = (uint8_t)(pack->data.length / 8 + 1);
  }
  else {
    byteCount = (uint8_t)(pack->data.length / 8);
  }
  
  PDUFrame[MBM_PDU_REQ_WRITE_MUL_BYTECNT_OFFSET] = byteCount;
  PDUFrame += MBM_PDU_REQ_WRITE_MUL_VALUES_OFFSET;
  
  uint16_t regIndex = 0;
  while(byteCount > regIndex) {
    *PDUFrame++ = ((uint8_t *)(pack->multiData))[regIndex++];
  }
  
  mbm_SetTxPDULength(dev, MBM_PDU_SIZE_MIN + MBM_PDU_REQ_WRITE_MUL_SIZE_MIN + byteCount);
  
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

uMBM_ErrCode_t mbm_Coils_Calllback(uMBM_Device_t *dev, uint8_t *value, int16_t address, uint16_t nCoils, uMBM_RegMode_t mode) {
  uint16_t regIndex;
  uint16_t regBitIndex;
  uint16_t nReg = nCoils / 8 + 1;

  uint8_t *coilsBuffer = dev->v8Buffer;
  dev->valueBufferCount = nCoils;
  
  address -= 1;
  
  if ((address >= MBM_COIL_START) && (address + nCoils <= MBM_COIL_START + MBM_COIL_NCOILS)) {
    regIndex = (uint16_t)(address - MBM_COIL_START) / 8;
    regBitIndex = (uint16_t)(address - MBM_COIL_START) % 8;
    
    switch (mode) {
      /*将接收数据写进缓冲器*/
      case (MBM_REG_READ) :
      /*将发送数据回写进缓冲器*/
      case (MBM_REG_WRITE) : {
        while (nReg > 1) {
          uMBM_Util_SetBits(&coilsBuffer[regIndex++], regBitIndex, 8, *value++);
          nReg -= 1;
        }
        
        /* last coils */
        nCoils = nCoils % 8;
        /* xMBUtilSetBits has bug when ucNBits is zero */
        if (nCoils != 0) {
          uMBM_Util_SetBits(&coilsBuffer[regIndex++], regBitIndex, nCoils, *value++);
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
 * 主机调用该函数将分离接收到的Coils寄存器数据到Buffer
 * @note none
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_Coils_Read_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint16_t regAddress;
  uint16_t colisCount;
  
  uint16_t byteCount;
  
  if (rxPDUFramelength >= MBM_PDU_SIZE_MIN + MBM_PDU_FUNC_READ_SIZE_MIN) {
    regAddress = (uint16_t )(txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t )(txPDUFrame[MBM_PDU_REQ_READ_ADDR_OFFSET + 1]);
    regAddress++;

    colisCount = (uint16_t)(txPDUFrame[MBM_PDU_REQ_READ_COILCNT_OFFSET] << 8);
    colisCount |= (uint16_t)(txPDUFrame[MBM_PDU_REQ_READ_COILCNT_OFFSET + 1]);
    
    /**
     * Test if the quantity of coils is a multiple of 8. If not last
     * byte is only partially field with unused coils set to zero. 
     */
    if((colisCount & 0x0007 ) != 0) {
      byteCount = (uint8_t)(colisCount / 8 + 1);
    }
    else {
      byteCount = (uint8_t)(colisCount / 8);
    }
    
    /**
     * Check if the number of registers to read is valid. If not
     * return Modbus illegal data value exception. 
     */
    if ((colisCount >= 1) && (byteCount == rxPDUFrame[MBM_PDU_FUNC_READ_COILCNT_OFFSET])) {
      uMBM_ErrCode_t errCode = mbm_Coils_Calllback(dev, &rxPDUFrame[MBM_PDU_FUNC_READ_VALUES_OFFSET], regAddress, colisCount, MBM_REG_READ);
      
      if (errCode != MBM_ERR_OK) {
        exception = MBM_EX_ILLEGAL_DATA_ADDRESS;
      }
    }
    else {
      exception = MBM_EX_ILLEGAL_DATA_VALUE;
    }
  }
  else {
    /** 
     * Can't be a valid read coil register request because the length
     * is incorrect. 
     */
    exception = MBM_EX_ILLEGAL_DATA_VALUE;
  }
  
  return exception;
}


/**
 * 主机调用该函数将接收到的报文分析,并将发送的数据回写进缓冲器
 * @note none
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_Coils_SingleWrite_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uint16_t regAddress;
  uint8_t buffer[2];
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  if (rxPDUFramelength == (MBM_PDU_FUNC_WRITE_SIZE + MBM_PDU_SIZE_MIN)) {
    regAddress = (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_ADDR_OFFSET] << 8 );
    regAddress |= (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_ADDR_OFFSET + 1] );
    regAddress++;
    
    if ((rxPDUFrame[MBM_PDU_FUNC_WRITE_VALUE_OFFSET + 1] == 0x00) && 
      ((rxPDUFrame[MBM_PDU_FUNC_WRITE_VALUE_OFFSET] == 0xFF) || 
    (rxPDUFrame[MBM_PDU_FUNC_WRITE_VALUE_OFFSET] == 0x00))) {
      buffer[1] = 0;
          
      if(rxPDUFrame[MBM_PDU_FUNC_WRITE_VALUE_OFFSET] == 0xFF) {
          buffer[0] = 1;
      }
      else {
          buffer[0] = 0;
      }
      
      uMBM_ErrCode_t errCode = mbm_Coils_Calllback(dev, &buffer[0], regAddress, 1, MBM_REG_WRITE);
      
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
 * 主机调用该函数将接收到的报文分析,并将发送的数据回写进缓冲器
 * @note none
 *
 * @param *mbm 主机
 *
 * @return uMBM_Exception_t
 */
uMBM_Exception_t mbm_Coils_MultiWrite_Func(void *mbm) {
  uMBM_Exception_t exception = MBM_EX_NONE;
  
  uint16_t regAddress;
  uint16_t coilsCount;
  uint8_t byteCount;
  uint8_t byteVerify;
  
  uMBM_Device_t *dev = (uMBM_Device_t *)mbm;
  
  uint16_t rxPDUFramelength = dev->rxPDULength;
  uint8_t *rxPDUFrame = dev->rxPDUFrame;
  
  uint8_t *txPDUFrame = mbm_GetTxPDUBuffer(dev);
  
  if (rxPDUFramelength == MBM_PDU_FUNC_WRITE_MUL_SIZE) {
    regAddress = (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_MUL_ADDR_OFFSET] << 8);
    regAddress |= (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_MUL_ADDR_OFFSET + 1]);
    regAddress++;
    
    coilsCount = (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_MUL_COILCNT_OFFSET] << 8);
    coilsCount |= (uint16_t)(rxPDUFrame[MBM_PDU_FUNC_WRITE_MUL_COILCNT_OFFSET + 1]);
    
    byteCount = txPDUFrame[MBM_PDU_REQ_WRITE_MUL_BYTECNT_OFFSET];
    
    /*Compute the number of expected bytes in the request.*/
    if((coilsCount & 0x0007) != 0) {
      byteVerify = (uint8_t)(coilsCount / 8 + 1);
    }
    else {
      byteVerify = (uint8_t)(coilsCount / 8);
    }
    
    if ((coilsCount >= 1) && (byteVerify == byteCount)) {
      uMBM_ErrCode_t errCode = mbm_Coils_Calllback(dev, &txPDUFrame[MBM_PDU_REQ_WRITE_MUL_VALUES_OFFSET], regAddress, byteCount, MBM_REG_WRITE);
      
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
