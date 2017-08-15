/**
 *        __  __         _ _            __  __         _           
 *   _  _|  \/  |___  __| | |__ _  _ __|  \/  |__ _ __| |_ ___ _ _ 
 *  | || | |\/| / _ \/ _` | '_ \ || (_-< |\/| / _` (_-<  _/ -_) '_|
 *   \_,_|_|  |_\___/\__,_|_.__/\_,_/__/_|  |_\__,_/__/\__\___|_|  
 *                                                                
 * File      : mbmType.h
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
 
#ifndef MBMTYPE_H_
#define MBMTYPE_H_

/* ----------------------- ANSI-C include files -----------------------------*/
#include <stdbool.h>
#include <stdint.h>

/**
 * @addtogroup modbus PDU/Serial Lin PDU define
 */

/*@{*/

/**
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
 * <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MBM_SER_PDU_SIZE_MAX = 256
 * (2)  ... MBM_SER_PDU_ADDR_OFF = 0
 * (3)  ... MBM_SER_PDU_PDU_OFF  = 1
 * (4)  ... MBM_SER_PDU_SIZE_CRC = 2
 *
 * (1') ... MBM_PDU_SIZE_MAX     = 253
 * (2') ... MBM_PDU_FUNC_OFF     = 0
 * (3') ... MBM_PDU_DATA_OFF     = 1
 * </code>
 */

#define MBM_SL_PDU_SIZE_MIN      (4)    /**< Serial line PDU: Minimum size of a Modbus RTU frame. */
#define MBM_SL_PDU_SIZE_MAX      (256)  /**< Serial line PDU: Maximum size of a Modbus RTU frame. */
#define MBM_SL_PDU_CRC_SIZE      (2)    /**< Serial line PDU: Size of CRC field in PDU. */
#define MBM_SL_PDU_ADDR_OFFSET   (0)    /**< Serial line PDU: Offset of slave address in Ser-PDU. */
#define MBM_SL_PDU_PDU_OFFSET    (1)    /**< Serial line PDU: Offset of Modbus-PDU in Ser-PDU. */

#define MBM_PDU_SIZE_MAX     (253)  /**< Maximum size of a PDU. */
#define MBM_PDU_SIZE_MIN     (1)    /**< Function Code */
#define MBM_PDU_FUNC_OFFSET  (0)    /**< Offset of function code in PDU. */
#define MBM_PDU_DATA_OFFSET  (1)    /**< Offset for response data in PDU. */

/*@}*/

/**
 * @addtogroup modbus command define
 */

/*@{*/

#define MBM_ADDRESS_BROADCAST    (0)   /*! Modbus broadcast address. */
#define MBM_ADDRESS_MIN          (1)   /*! Smallest possible slave address. */
#define MBM_ADDRESS_MAX          (247) /*! Biggest possible slave address. */
          
#define MBM_FUNC_NONE                          (0)
#define MBM_FUNC_READ_COILS                    (1)
#define MBM_FUNC_READ_DISCRETE_INPUTS          (2)
#define MBM_FUNC_WRITE_SINGLE_COIL             (5)
#define MBM_FUNC_WRITE_MULTIPLE_COILS          (15)
#define MBM_FUNC_READ_HOLDING_REGISTER         (3)
#define MBM_FUNC_READ_INPUT_REGISTER           (4)
#define MBM_FUNC_WRITE_REGISTER                (6)
#define MBM_FUNC_WRITE_MULTIPLE_REGISTERS      (16)
#define MBM_FUNC_READWRITE_MULTIPLE_REGISTERS  (23)
#define MBM_FUNC_DIAG_READ_EXCEPTION           (7)
#define MBM_FUNC_DIAG_DIAGNOSTIC               (8)
#define MBM_FUNC_DIAG_GET_COM_EVENT_CNT        (11)
#define MBM_FUNC_DIAG_GET_COM_EVENT_LOG        (12)
#define MBM_FUNC_OTHER_REPORT_SLAVEID          (17)

#define MBM_FUNC_ERROR                         (128)

/*@}*/

/**
 * @addtogroup Error code Type define
 */

/*@{*/

/**
 * Modbus master error codes
 * @note Error codes used by all function in the Master request
 */
typedef enum {
  MBM_ERR_OK,             /**< no error. */
  
  /*mbm core error code*/
  MBM_ERR_EPORTERR,       /**< porting layer error. */
  MBM_ERR_IO,             /**< I/O error. */  
  MBM_ERR_DATA_OVERFLOW,  /**< 数据溢出 */
  MBM_ERR_CHEKSUM,        /**< 数据校验失败 */
  
  /*Req error code*/
  MBM_ERR_BUSY,           /**< master is busy now. */
  MBM_ERR_TIMEOUT,        /**< timeout error occurred. */
  MBM_ERR_RECVIVE,        /**< 接收错误. */
  MBM_ERR_NOREG,          /**< illegal register address. */
  MBM_ERR_EXECUTE,        /**< Function 操作失败 */
  MBM_ERR_ILL_ARG         /**< illegal argument. */
} uMBM_ErrCode_t;

/*@}*/

/**
 * @addtogroup functions exception Type define
 */

/*@{*/

/**
 * Modbus master functions exception codes
 * @note none
 */
typedef enum {
  MBM_EX_NONE = 0x00,
  MBM_EX_ILLEGAL_FUNCTION = 0x01,
  MBM_EX_ILLEGAL_DATA_ADDRESS = 0x02,
  MBM_EX_ILLEGAL_DATA_VALUE = 0x03,
  MBM_EX_SLAVE_DEVICE_FAILURE = 0x04,
  MBM_EX_ACKNOWLEDGE = 0x05,
  MBM_EX_SLAVE_BUSY = 0x06,
  MBM_EX_MEMORY_PARITY_ERROR = 0x08,
  MBM_EX_GATEWAY_PATH_FAILED = 0x0A,
  MBM_EX_GATEWAY_TGT_FAILED = 0x0B
} uMBM_Exception_t;

/*@}*/

/**
 * @addtogroup Core Type define
 */

/*@{*/

/**
 * Modbus master timer moode
 * @note Timer mode is Master 3 kind of Timer modes.
 */
typedef enum {
  MBM_TMODE_IDLE,
  MBM_TMODE_T35,               /**< Master receive frame T3.5 timeout. */
  MBM_TMODE_RESPOND_TIMEOUT,   /**< Master wait respond for slave. */
} uMBM_TimeMode_t;


/**
 * Modbus master events
 * @note none
 */
typedef enum {
  /*内核态事件*/
  MBM_EV_READY,                 /**< 主机启动就绪 */
  MBM_EV_FRAME_SENT,            /**< 主机发送一帧数据*/
  MBM_EV_FRAME_RECEIVED,        /**< 主机接收到一帧数据 */
  MBM_EV_FUNC_EXECUTE,          /**< 主机执行一条指令 */

  MBM_EV_RESPOND_TIMEOUT,       /**< 主机请求超时 */
  MBM_EV_ERROR_SENT,            /**< 主机发送出现错误 */
  MBM_EV_ERROR_RECEIVE,         /**< 主机接受出现错误 */
  MBM_EV_ERROR_FUNC_EXECUTE,    /**< 主机处理命令有误 */
  
  /*用户态事件*/
  MBM_USER_EV_ERROR_SENT,           /**< 发送失败 */
  MBM_USER_EV_RESPOND_TIMEOUT,      /**< 请求超时 */
  MBM_USER_EV_ERROR_RECEIVE,        /**< 数据有错误 */
  MBM_USER_EV_ERROR_FUNC_EXECUTE,   /**< 请求执行有误 */
  MBM_USER_EV_OK                    /**< 请求成功 */
} uMBM_Event_t;


/**
 * Modbus master serial parity
 * @note none
 */
typedef enum {
  MBM_PAR_NONE,   /**< No parity. */
  MBM_PAR_ODD,    /**< Odd parity. */
  MBM_PAR_EVEN    /**< Even parity. */
} uMBM_PortParity_t;


/**
 * Modbus master Rx status
 * @note none
 */
typedef enum {
  MBM_RX_IDLE,        /**< Receiver is in idle state. */
  MBM_RX_RCV,         /**< Frame is beeing received. */
  MBM_RX_ERROR        /**< If the frame is invalid. */
} uMBM_RxStatus_t;


/**
 * Modbus master Tx status
 * @note none
 */
typedef enum {
  MBM_TX_IDLE,      /**< Transmitter is in idle state. */
  MBM_TX_XMIT,      /**< Transmitter is in transfer state. */
  MBM_TX_XFWR       /**< Transmitter is in transfer finish and wait receive state. */
} uMBM_TxStatus_t;


/**
 * Modbus master reg mode
 * @note none
 */
typedef enum {
  MBM_REG_READ,
  MBM_REG_WRITE
} uMBM_RegMode_t;

/*@}*/

/**
 * @addtogroup General data pack define
 */

/*@{*/

typedef struct {
  uint16_t destAddr;   /**< 目标地址 */
  uint16_t regAddr;    /**< 寄存器地址 */
  union {
    uint16_t length;  /**< 目标长度 */
    uint16_t value;   /**< 目标数据,用于发送一个数据给从机 */
  }data;
  uint16_t *multiData;  /**< 大型数据指针,用于一次性发送大量数据给从机 */
} uMBM_GeneralReqPack_t;

/*@}*/

/**
 * @addtogroup General modbus functions define
 */

/*@{*/

typedef uMBM_Exception_t (*functionHandler)(void *mbm);

typedef struct {
  uint8_t functionCode;
  functionHandler handler;
} uMBM_FunctionHandler_t;

/*@}*/

#endif
