/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : pmEvent.c
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

/**
 * 该文件用于定义mb主机各种事件的前台以及后台处理
 * @note 用户可在此自定义各种后续事件处理
 */
 
/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>
/* ----------------------- Modbus core include files -----------------------------*/
#include "../Core/mbmType.h"
#include "../Core/mbmPort.h"
/* ----------------------- OS support include files -----------------------------*/
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
/* ----------------------- Hardware support include files -----------------------------*/
#include "stm32f0xx.h"                  // Device header

/**
 * @addtogroup OS support Events
 */

/*@{*/

osMessageQueueId_t mbm_CoreMsg_ID;

osSemaphoreId_t mbm_Sem_ID;

osMessageQueueId_t mbm_UserMsg_ID;

/*@}*/

/**
 * @addtogroup Event 方法集合
 */

/*@{*/

extern uMBM_ErrCode_t pMBM_EventInit(void);
extern void pMBM_EventPost(uMBM_Event_t event);
extern uMBM_Event_t pMBM_EventGet(void);

extern uMBM_ErrCode_t pMBM_SemaphoreInit(void);
extern bool pMBM_SemaphoreWait(uint32_t timeout);
extern void pMBM_SemaphoreRelease(void);

extern void pMBM_UserEventPost(uMBM_Event_t event);
extern uMBM_Event_t pMBM_UserEventGet(void);

pMBM_Event_t mbm_Event = {
  .pMBM_EventInit = pMBM_EventInit,
  .pMBM_EventPost = pMBM_EventPost,
  .pMBM_EventGet = pMBM_EventGet,
  
  .pMBM_SemaphoreInit = pMBM_SemaphoreInit,
  .pMBM_SemaphoreWait = pMBM_SemaphoreWait,
  .pMBM_SemaphoreRelease = pMBM_SemaphoreRelease,
  
  .pMBM_UserEventPost = pMBM_UserEventPost,
  .pMBM_UserEventGet = pMBM_UserEventGet
};

/*@}*/

/**
 * @addtogroup mbm内核事件
 */

/*@{*/

/**
 * 初始化事件引擎
 * @note none
 *
 * @param none
 *
 * @return uMBM_ErrCode_t 
 */
uMBM_ErrCode_t pMBM_EventInit(void) {
  mbm_CoreMsg_ID = osMessageQueueNew(8, sizeof(uint32_t), NULL);
  mbm_UserMsg_ID = osMessageQueueNew(8, sizeof(uint32_t), NULL);
  
  return MBM_ERR_OK;
}


/**
 * 事件发送
 * @note none
 *
 * @param event 待发送事件
 *
 * @return none 
 */
void pMBM_EventPost(uMBM_Event_t event) {
  /*该函数有可能在IRQ中执行,所以延时等待必须=0*/
  osMessageQueuePut(mbm_CoreMsg_ID, &event, 0, 0);
}


/**
 * 内核事件获取
 * @note none
 *
 * @param none
 *
 * @return uMBM_Event_t 
 */
uMBM_Event_t pMBM_EventGet(void) {
  uMBM_Event_t event;
  
  osMessageQueueGet(mbm_CoreMsg_ID, &event, NULL, osWaitForever);
  return (uMBM_Event_t)(event);
}

/*@}*/

/**
 * @addtogroup mbm线程同步锁
 */

/*@{*/

/**
 * 初始化mbm信号量
 * @note 用于线程同步锁
 *
 * @param none
 *
 * @return uMBM_ErrCode_t 
 */
uMBM_ErrCode_t pMBM_SemaphoreInit(void) {
  mbm_Sem_ID = osSemaphoreNew(1, 1, NULL);
  
  return MBM_ERR_OK;
}


/**
 * 等待mbm信号量
 * @note 用于线程同步锁
 *
 * @param timeout 超时值
 *
 * @return [bool] -> 获取结果 
 */
bool pMBM_SemaphoreWait(uint32_t timeout) {
  osStatus_t val =  osSemaphoreAcquire(mbm_Sem_ID, timeout);
  
  if (val == osOK) {
    return true;
  }
  else {
    return false;
  }
}


/**
 * 释放mbm信号量
 * @note 用于线程同步锁
 *
 * @param none
 *
 * @return none
 */
void pMBM_SemaphoreRelease(void) {
  osSemaphoreRelease(mbm_Sem_ID); 
}

/*@}*/

/**
 * @addtogroup mbm用户态事件
 */

/*@{*/

/**
 * 用户态事件发送
 * @note none
 *
 * @param event 待发送事件
 *
 * @return none 
 */
void pMBM_UserEventPost(uMBM_Event_t event) {
  /*该函数有可能在IRQ中执行,所以延时等待必须=0*/
  osMessageQueuePut(mbm_UserMsg_ID, &event, 0, 0);
}


/**
 * 用户态事件获取
 * @note none
 *
 * @param none
 *
 * @return uMBM_Event_t 
 */
uMBM_Event_t pMBM_UserEventGet(void) {
  uMBM_Event_t event;
  
  osMessageQueueGet(mbm_UserMsg_ID, &event, NULL, osWaitForever);
  return (uMBM_Event_t)(event);
}

/*@}*/
