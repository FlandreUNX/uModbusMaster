/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmUtils.h
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
 
#ifndef MBMUTILS_H_
#define MBMUTILS_H_

/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>

/**
 * @addtogroup 获取宿主结构体指针
 */

/*@{*/

/**
 * @note from linux "include/linux/kernel.h"
 */
#define offset_of(type, name) \
	((uint32_t) &((type *)0)->name)


/**
 * 获取当前元素所在的宿主结构项
 *
 * @param ptr 宿主结构体下的元素指针
 * @param type 宿主类型
 * @param name 元素在宿主结构体里面的名称
 * 
 * @retval 宿主结构体指针
 */
#define struct_Entry(ptr, type, name) \
    (type*)((uint8_t*)(ptr)-offset_of(type, name))

/*@}*/

/**
 * @addtogroup modbus 工具类方法
 */

/*@{*/

extern void uMBM_Util_SetBits(uint8_t *dataArea, uint16_t bitOffset, uint8_t nBits, uint8_t value);

extern uint8_t uMBM_Util_GetBits(uint8_t *dataArea, uint16_t bitOffset, uint8_t nBits);

/*@}*/

#endif
