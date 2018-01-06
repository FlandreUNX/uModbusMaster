/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbm.h
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
 
#ifndef MBM_H_
#define MBM_H_

/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>
/* ----------------------- Modbus core include files -----------------------------*/
#include "./Core/mbmType.h"
#include "./Core/mbmPort.h"
#include "./mbmConfig.h"

/**
 * @addtogroup Modbus master device define
 */

/*@{*/

typedef struct {
  pMBM_Event_t *event;
  pMBM_Serial_t *serial;
  pMBM_Timer_t *timer;
  
  uMBM_RxStatus_t rxState;
  uMBM_TxStatus_t txState;
  
  uint8_t txBuffer[MBM_SL_PDU_SIZE_MAX];
  uint8_t *txBufferPos;
  uint16_t txBufferLength;
  
  uint8_t rxBuffer[MBM_SL_PDU_SIZE_MAX];
  uint16_t rxBufferPos;
  
  uint16_t txPDULength;
  
  uint8_t *rxPDUFrame;     /**< 接收到数据PDU帧起点 */
  uint16_t rxPDULength;
  
  uint8_t destAddress;     /**< 目标地址 */
  uint8_t rcvAddress;      /**< 主机接收到的地址 */
  
  uMBM_TimeMode_t timeMode;
  
  uMBM_ErrCode_t currentErrorCode;
  uMBM_Exception_t currentException;
  
  uint16_t v16Buffer[MBM_BUFFER_MAX];
  uint8_t v8Buffer[MBM_BUFFER_MAX];
  uint16_t valueBufferCount;
} uMBM_Device_t;

/*@}*/

/**
 * @addtogroup Modbus master 控制类方法
 * @note Public
 */

/*@{*/

/**
 * 创建一个Modbus master主机
 * @note [Rev.1]
 *
 * @param name, 主机名称
 * @param number, 主机序号
 *
 * @return uMBM_Device_t
 */
#define uMBM_CreateDev(name, number) \
  uMBM_Device_t mbm_##name##_##number


/**
 * 声明一个Modbus master主机
 * @note [Rev.1]
 *
 * @param name, 主机名称
 * @param number, 主机序号
 *
 * @return uMBM_Device_t
 */
#define uMBM_ExternDev(name, number) \
  extern uMBM_Device_t mbm_##name##_##number
   
  
/**
 * 获取一个Modbus master主机
 * @note [Rev.1]
 *
 * @param name, 主机名称
 * @param number, 主机序号
 *
 * @return uMBM_Device_t
 */
#define uMBM_GetDev(name, number) \
  &mbm_##name##_##number

extern uMBM_ErrCode_t uMBM_CoreInit(uMBM_Device_t *dev, 
                                    pMBM_Event_t *event,
                                    pMBM_Serial_t *serial,
                                    pMBM_Timer_t *timer);

extern uMBM_ErrCode_t uMBM_Enable(uMBM_Device_t *dev);
extern uMBM_ErrCode_t uMBM_Disable(uMBM_Device_t *dev);

extern uMBM_ErrCode_t uMBM_Poll(uMBM_Device_t *dev);

/*@}*/

/**
 * @addtogroup Modbus master 方法类
 * @note Public
 */

/*@{*/

extern uint16_t uMBM_GetValueBufferCount(uMBM_Device_t *dev);

extern uint16_t uMBM_GetBuffer_16(uMBM_Device_t *dev, uint16_t *buffer, uint16_t count);
extern uint16_t uMBM_GetBuffer_8(uMBM_Device_t *dev, uint8_t *buffer, uint16_t count);

extern uMBM_ErrCode_t uMBM_UserEvent2ErrorCode(uMBM_Event_t event);
extern uMBM_Exception_t uMBM_GetCurrentException(uMBM_Device_t *dev);

extern void uMBM_Util_SetBits(uint8_t *dataArea, uint16_t bitOffset, uint8_t nBits, uint8_t value);
extern uint8_t uMBM_Util_GetBits(uint8_t *dataArea, uint16_t bitOffset, uint8_t nBits);

extern uMBM_ErrCode_t uMBM_InputReg_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);

extern uMBM_ErrCode_t uMBM_HoldingReg_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);
extern uMBM_ErrCode_t uMBM_HoldingReg_SingleWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);
extern uMBM_ErrCode_t uMBM_HoldingReg_MultiWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);
extern uMBM_ErrCode_t uMBM_HoldingReg_MultiReadWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack2Tx, uMBM_GeneralReqPack_t *pack2Rx, uint32_t timeout);

extern uMBM_ErrCode_t uMBM_Coils_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);
extern uMBM_ErrCode_t uMBM_Coils_SingleWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);
extern uMBM_ErrCode_t uMBM_Coils_MultiWrite(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);

extern uMBM_ErrCode_t uMBM_DiscreteInput_Read(uMBM_Device_t *dev, uMBM_GeneralReqPack_t *pack, uint32_t timeout);

/*@}*/

/**
 * @addtogroup Modbus master设置类方法
 * @note Private
 */

/*@{*/

extern uint8_t* mbm_GetTxPDUBuffer(uMBM_Device_t *dev);

extern void mbm_SetTxPDULength(uMBM_Device_t *dev, uint16_t length);
extern uint8_t mbm_GetTxPDULength(uMBM_Device_t *dev);

/*@}*/

#endif
