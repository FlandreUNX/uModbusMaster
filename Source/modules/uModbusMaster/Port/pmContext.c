/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : pmContext.c
 *  Copyright (C) <2017>  <FlandreUNX>
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
 * 该文件用于定义mb主机进去不可打断的状态的底层方法
 * @note 用户可在此自定义各种后续事件处理
 */
 
/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>
 
/* ----------------------- Modbus core include files -----------------------------*/
#include "../core/mbmType.h"
#include "../core/mbmPort.h"

/* ----------------------- OS support include files -----------------------------*/
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

/* ----------------------- Hardware support include files -----------------------------*/

#if MBM_USE_CONTEXT == 1

/**
 * @addtogroup 方法定义
 */

/*@{*/

/**
 * 进入上下文保护
 * @note none
 *
 * @param none
 *
 * @return none 
 */
void pMBM_EnterCriticalSection(void) {
  
}


/**
 * 退出上下文保护
 * @note none
 *
 * @param none
 *
 * @return none 
 */
void pMBM_ExitCriticalSection(void) {
  
}

/*@}*/

#endif
