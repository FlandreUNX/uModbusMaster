/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : th_ModbusMaster.c
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

//uMBM_CreateDev(SensorHub, 0);
uMBM_Device_t mbm_SensorHub_0;

/*@}*/


/**
 * @addtogroup thread
 */
 
/*@{*/

osThreadId modbusMaster_ThreadID;

void modbusMaster_Thread(void const *arg) {
  for (;;) {
    uMBM_Poll(uMBM_GetDev(SensorHub, 0));
  }
}
osThreadDef(modbusMaster_Thread, osPriorityNormal , 1, 0);


void modbusMaster_ThreadStart(void) {
  extern pMBM_Event_t mbm_Event;
  extern pMBM_Serial_t mbm_Serial;
  extern pMBM_Timer_t mbm_Timer;
  uMBM_CoreInit(uMBM_GetDev(SensorHub, 0), &mbm_Event, &mbm_Serial, &mbm_Timer);
  uMBM_Enable(uMBM_GetDev(SensorHub, 0));
  
  modbusMaster_ThreadID = osThreadCreate(osThread(modbusMaster_Thread), NULL);
}

/*@}*/
