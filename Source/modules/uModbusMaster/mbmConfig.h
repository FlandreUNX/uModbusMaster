/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmConfig.h
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
 
#ifndef MBMCONFIG_H_
#define MBMCONFIG_H_

/**
 * @addtogroup modbus timer config
 */

/*@{*/

/**
 * 响应超时值
 * @note none
 */
#define RESPOND_TIMEOUT_MS   (1000)   

/*@}*/

/**
 * @addtogroup modbus general config
 */

/*@{*/

/**
 * Maximum number of Modbus functions codes the protocol stack should support.
 * @note none
 */
#define MBM_FUNC_HANDLERS_MAX    (16)

/**
 * mbm存放从机返回数据的缓冲器最大值
 * @note none
 */
#define MBM_BUFFER_MAX    (16)

/**
 * mbm是否使用上下文保护
 * @note none
 */
#define MBM_USE_CONTEXT    (0)


/*@}*/

/**
 * @addtogroup functions general config
 */

/*@{*/

/**
 * Report Slave ID
 * @note none
 */
#define MBM_FUNC_OTHER_REP_SLAVEID_ENABLED    (1)

/**
 * Input Registers
 * @note none
 */
#define MBM_FUNC_INPUT_ENABLED     (1)

/**
 * Holding Registers
 * @note none
 */
#define MBM_FUNC_HOLDING_ENABLED   (1)

/**
 * Colis Registers
 * @note none
 */
#define MBM_FUNC_COILS_ENABLED   (1)

/**
 * Colis Registers
 * @note none
 */
#define MBM_FUNC_DISC_ENABLED   (1)

/*@}*/

/**
 * @addtogroup Register functions general config
 */

/*@{*/

#define MBM_REG_INPUT_START    (0)
#define MBM_REG_INPUT_NREGS    (10)

#define MBM_REG_HOLDING_START     (0)
#define MBM_REG_HOLDING_NREGS     (10)

#define MBM_DISCRETE_INPUT_START        (0)
#define MBM_DISCRETE_INPUT_NDISCRETES   (8)
        
#define MBM_COIL_START      (0)
#define MBM_COIL_NCOILS     (8)

/*@}*/

#endif
