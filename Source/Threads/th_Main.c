/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : th_Main.c
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
 * @addtogroup system
 */

/*@{*/

#include "stm32f0xx.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*@}*/ 

/**
 * @addtogroup thread
 */

/*@{*/

#include "./thread.h"

/*@}*/

/**
 * @addtogroup modules
 */
 
/*@{*/

#include "../modules/uModbusMaster/mbm.h"
uMBM_ExternDev(SensorHub, 0);

/*@}*/

/**
 * @addtogroup thread define
 */
 
/*@{*/

/*@}*/

/**
 * @addtogroup var define
 */

/*@{*/

uint16_t rcvInputReg[10];
uint16_t rcvLength;

/*@}*/

/**
 * @addtogroup thread
 */
 
/*@{*/



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

/*@}*/
