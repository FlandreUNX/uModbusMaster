/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmUtils.c
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
 
/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>

/* ----------------------- Modbus core include files -----------------------------*/
#include "../mbm.h"
#include "./mbmPort.h"
#include "./mbmCRC16.h"
#include "../mbmConfig.h"

/**
 * @addtogroup 使用例子
 * @note none
 */

/*@{*/

/**
 * http://www.freemodbus.org/api/group__modbus__utils.html#ga1
 */

/* ----------------------- uMBM_Util_SetBits -----------------------------*/
/**
  ucBits[2] = {0, 0};

  // Set bit 4 to 1 (read: set 1 bit starting at bit offset 4 to value 1)
  uMBM_Util_SetBits(ucBits, 4, 1, 1 ;

  // Set bit 7 to 1 and bit 8 to 0.
  uMBM_Util_SetBits(ucBits, 7, 2, 0x01);

  // Set bits 8 - 11 to 0x05 and bits 12 - 15 to 0x0A;
  uMBM_Util_SetBits(ucBits, 8, 8, 0x5A);
 */
 
/* ----------------------- uMBM_Util_GetBits -----------------------------*/
/**
  uint8_t ucBits[2] = {0, 0};
  uint8_t ucResult;

  // Extract the bits 3 - 10.
  ucResult = uMBM_Util_GetBits(ucBits, 3, 8);
 */

/*@}*/

/**
 * @addtogroup modbus 工具类define
 */

/*@{*/

#define BITS_UINT8    8

/*@}*/

/**
 * @addtogroup modbus 工具类方法
 */

/*@{*/

/**
 * 设置一个字节缓冲的位
 * @note 这个函数可以实现有效处理位域的功能;存储位域的数值必须是两个字节;一次操作最多可处理8位
 *
 * @param *dataArea 位存储的缓冲区: 必须是2个字节的倍数
 * @param bitOffset 位设置的起始地址: 第一个位的偏移为0
 * @param nBits 需要修改的位的数量: 该值必须小于8 串口方法集合
 * @param value 位的新值: 在bitOffset中的第一位的值是newValue的最低有效位
 *
 * @return none 
 */
void uMBM_Util_SetBits(uint8_t *dataArea, uint16_t bitOffset, uint8_t nBits, uint8_t value) {
  int16_t wordBuf;
  uint16_t mask;
  uint16_t byteOffset;
  uint16_t nPreBits;
  uint16_t newValue = value;
  
  /**
   * Calculate byte offset for first byte containing the bit values starting at usBitOffset.
   */
  byteOffset = (uint16_t)((bitOffset) / BITS_UINT8);
  
  /*How many bits precede our bits to set.*/
  nPreBits = (uint16_t)(bitOffset - byteOffset * BITS_UINT8);
  
  /*Move bit field into position over bits to set*/
  newValue <<= nPreBits;
  
  /*Prepare a mask for setting the new bits.*/
  mask = (uint16_t)((1 << (uint16_t)nBits) - 1);
  mask <<= bitOffset - byteOffset * BITS_UINT8;
  
  /*copy bits into temporary storage.*/
  wordBuf = dataArea[byteOffset];
  wordBuf |= dataArea[byteOffset + 1] << BITS_UINT8;
  
  /*Zero out bit field bits and then or value bits into them.*/
  wordBuf = (uint16_t)((wordBuf & (~mask)) | newValue);
  
  /*move bits back into storage*/
  dataArea[byteOffset] = (uint8_t)(wordBuf & 0xFF);
  dataArea[byteOffset + 1] = (uint8_t)(wordBuf >> BITS_UINT8);
}


/**
 * 在字节缓冲中读取位
 * @note 这个函数从一个字节中来获取一个位的值;一步最多可以获取8个位的值
 *
 * @param *dataArea 位存储的缓冲区: 必须是2个字节的倍数
 * @param bitOffset 位设置的起始地址: 第一个位的偏移为0
 * @param nBits 需要读取的位的数量: 该值必须小于8
 *
 * @return [uint8_t] -> 该位状态
 */
uint8_t uMBM_Util_GetBits(uint8_t *dataArea, uint16_t bitOffset, uint8_t nBits) {
  int16_t wordBuf;
  uint16_t mask;
  uint16_t byteOffset;
  uint16_t nPreBits;
  
  /*Calculate byte offset for first byte containing the bit values starting at usBitOffset. */
  byteOffset = (uint16_t)((bitOffset) / BITS_UINT8);
  
  /*How many bits precede our bits to set.*/
  nPreBits = (uint16_t)(bitOffset - byteOffset * BITS_UINT8);
  
  /*Prepare a mask for setting the new bits.*/
  mask = (uint16_t)((1 << (uint16_t)nBits) - 1);
  
  /*copy bits into temporary storage.*/
  wordBuf = dataArea[byteOffset];
  wordBuf |= dataArea[byteOffset + 1] << BITS_UINT8;
  
  /*throw away unneeded bits.*/
  wordBuf >>= nPreBits;

  /*mask away bits above the requested bitfield.*/
  wordBuf &= mask;

  return (uint8_t)wordBuf;
}

/*@}*/
