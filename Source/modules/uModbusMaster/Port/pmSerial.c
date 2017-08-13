/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : pmSerial.c
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
 * 该文件用于定义mb主机串口数据传输的底层方法
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
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

/* ----------------------- Hardware support include files -----------------------------*/
#include "stm32f0xx.h"                  // Device header


/**
 * @addtogroup 串口方法集合
 */

/*@{*/

extern uMBM_ErrCode_t pMBM_SerialInit(void);
extern void pMBM_SerialEnable(bool txEnable, bool rxEnable);
extern void pMBM_SerialDisable(void);
extern void pMBM_SerialSendByte(uint8_t byte);
extern uint8_t pMBM_SerialReadByte(void);

pMBM_Serial_t mbm_Serial = {
  .baudRate = 9600,
  
  .pMBM_SerialInit = pMBM_SerialInit,
  .pMBM_SerialEnable = pMBM_SerialEnable,
  .pMBM_SerialDisable = pMBM_SerialDisable,
  .pMBM_SerialSendByte = pMBM_SerialSendByte,
  .pMBM_SerialReadByte = pMBM_SerialReadByte
};

/*@}*/

/**
 * @addtogroup 串口用户自定义方法
 */

/*@{*/

/**
 * 初始化串口
 * @note none
 *
 * @param none
 *
 * @return bool 
 */
uMBM_ErrCode_t pMBM_SerialInit(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpio);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

  USART_InitTypeDef usart;
	usart.USART_BaudRate = mbm_Serial.baudRate;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &usart);
  
  USART_ITConfig(USART1, USART_IT_ERR, ENABLE);
  USART_ClearFlag(USART1, USART_FLAG_RXNE | USART_FLAG_TC);
  
	USART_Cmd(USART1, ENABLE);
  
  NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelPriority = 4;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
  
  return MBM_ERR_OK;
}


/**
 * 使能串口收/发
 * @note none
 *
 * @param txEnable 串口发送使能/失能
 * @param rxEnable 串口接受使能/失能
 *
 * @return none 
 */
void pMBM_SerialEnable(bool txEnable, bool rxEnable) {
	if(txEnable) {
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	}
	else {
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	}  
  
  if (rxEnable) {
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
	else {
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	
	}
}


/**
 * 失能串口
 * @note none
 *
 * @param none
 *
 * @return none 
 */
void pMBM_SerialDisable(void) {
  
}


/**
 * 硬件发送一字节
 * @note none
 *
 * @param byte 
 *
 * @return none 
 */
void pMBM_SerialSendByte(uint8_t byte) {
  USART1->TDR = (char)byte;
}


/**
 * 硬件接收一字节
 * @note none
 *
 * @param none
 *
 * @return [uint8_t] -> one byte 
 */
uint8_t pMBM_SerialReadByte(void) {
  return (uint8_t)USART1->RDR;
}

/*@}*/

/**
 * @addtogroup 串口 ISR
 */

/*@{*/

void USART1_IRQHandler(void) {
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
    pMBM_SerialRXNE_ISR(&mbm_Serial);
  }
  else if(USART_GetITStatus(USART1, USART_IT_TC) == SET) {
    pMBM_SerialTC_ISR(&mbm_Serial);
  }
  else {
    USART1->ISR;
    USART1->ICR |= USART_ICR_PECF | USART_ICR_FECF | USART_ICR_NCF | USART_ICR_ORECF;
  }
}

/*@}*/
