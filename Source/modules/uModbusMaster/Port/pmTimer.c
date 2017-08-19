/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : pmTimer.c
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
 * 该文件用于定义mb主机数据传输时超时判断的底层方法
 * @note 用户可在此自定义各种后续事件处理
 */
 
/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>
/* ----------------------- Modbus core include files -----------------------------*/
#include "../mbm.h"
#include "../Core/mbmType.h"
#include "../Core/mbmPort.h"
/* ----------------------- OS support include files -----------------------------*/
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
/* ----------------------- Hardware support include files -----------------------------*/
#include "stm32f0xx.h"                  // Device header

/**
 * @addtogroup 定时器方法集合
 */

/*@{*/

extern uMBM_ErrCode_t pMBM_TimerInit(void);
extern void pMBM_TimerDisable(void);

extern void pMBM_T35TimerEnable(void);

extern void pMBM_RespondTimerEnable(void);

pMBM_Timer_t mbm_Timer = {
  .pMBM_TimerInit = pMBM_TimerInit,
  .pMBM_TimerDisable = pMBM_TimerDisable,
  
  .pMBM_T35TimerEnable = pMBM_T35TimerEnable,
  
  .pMBM_RespondTimerEnable = pMBM_RespondTimerEnable
};

/*@}*/

/**
 * @addtogroup 定时器设置相关定义
 */

/*@{*/

/**
 * 定时器50us计数一次所需要的PSC值
 * @note none
 */
#define TIME_PER_50US_PSC  ((SystemCoreClock / 2400) - 1)

/*@}*/

/**
 * @addtogroup 定时器用户自定义方法
 */

/*@{*/

/**
 * 初始化定时器
 * @note none
 *
 * @param time_us T3.5的时间值(us) 
 *
 * @return uMBM_ErrCode_t 
 */
uMBM_ErrCode_t pMBM_TimerInit(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = mbm_Timer.t35_Value;
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)TIME_PER_50US_PSC;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
//  TIM_ARRPreloadConfig(TIM15, ENABLE);
	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
	TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM15, DISABLE);
  
  return MBM_ERR_OK;
}


/**
 * 失能定时器
 * @note none
 *
 * @param none
 *
 * @return none 
 */
void pMBM_TimerDisable(void) {
  TIM15->CR1 &= ~TIM_CR1_CEN;
  TIM15->CNT = 0;
  TIM15->SR &= ~TIM_IT_Update;
}

/*@}*/

/**
 * @addtogroup T3.5定时器方法
 */

/*@{*/

/**
 * 使能T3.5定时器
 * @note none
 *
 * @param none
 *
 * @return none 
 */
void pMBM_T35TimerEnable(void) {
  TIM15->ARR = mbm_Timer.t35_Value;
  TIM15->CR1 |= TIM_CR1_CEN;
}

/*@}*/

/**
 * @addtogroup T3.5定时器方法
 */

/*@{*/

/**
 * 使能延时定时器
 * @note none
 *
 * @param none
 *
 * @return none 
 */
void pMBM_RespondTimerEnable(void) {
  TIM15->ARR = mbm_Timer.respondTime_Value;
  TIM15->CNT = 0;
  TIM15->CR1 |= TIM_CR1_CEN;
}

/*@}*/

/**
 * @addtogroup Hardware timer ISR
 */

/*@{*/

void TIM15_IRQHandler(void) {
  pMBM_Timer_ISR(&mbm_Timer);
}

/*@}*/
