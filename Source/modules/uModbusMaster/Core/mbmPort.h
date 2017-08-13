/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmPort.h
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
 
#ifndef MBMPORT_H_
#define MBMPORT_H_

/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>

/* ----------------------- Modbus core include files -----------------------------*/
#include "../core/mbmType.h"

/**
 * @addtogroup Event port functions define
 */

/*@{*/

typedef struct {
  uMBM_ErrCode_t (*pMBM_EventInit)(void);
  void (*pMBM_EventPost)(uMBM_Event_t event);
  uMBM_Event_t (*pMBM_EventGet)(void);
  
  uMBM_ErrCode_t (*pMBM_SemaphoreInit)(void);
  bool (*pMBM_SemaphoreWait)(uint32_t timeout);
  void (*pMBM_SemaphoreRelease)(void);
  
  void (*pMBM_UserEventPost)(uMBM_Event_t event);
  uMBM_Event_t (*pMBM_UserEventGet)(void);
} const pMBM_Event_t;

/*@}*/

/**
 * @addtogroup Serial port functions define
 */

/*@{*/

typedef struct {
  void *mbm_Device;    /**< 该端口的宿主 */
  
  uint16_t baudRate;    /**< 串口波特率 */
  
  uMBM_ErrCode_t (*pMBM_SerialInit)(void);
  
  void (*pMBM_SerialEnable)(bool txEnable, bool rxEnable);
  void (*pMBM_SerialDisable)(void);
  
  void (*pMBM_SerialSendByte)(uint8_t byte);
  uint8_t (*pMBM_SerialReadByte)(void);
} pMBM_Serial_t;

/*@}*/

/**
 * @addtogroup Timer port functions define
 */

/*@{*/

typedef struct {
  void *mbm_Device;    /**< 该端口的宿主 */
  
  uint16_t t35_Value;           /**< T3.5数值 */
  uint16_t respondTime_Value;   /**< 响应超时值 */
  
  uMBM_ErrCode_t (*pMBM_TimerInit)(void);
  void (*pMBM_TimerDisable)(void);
  
  void (*pMBM_T35TimerEnable)(void);
  
  void (*pMBM_RespondTimerEnable)(void);
} pMBM_Timer_t;

/*@}*/

#endif
