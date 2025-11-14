/**
  ******************************************************************************
  *
  * @file    dev_conf.h
  * @author  
  * @brief   Header file of dev_conf module.
  *
  ******************************************************************************
  **/
  
#ifndef _DEV_CONF_H
#define _DEV_CONF_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
/**
 * @brief Device types.
 */
enum dev_type
{
    DEV_TYPE_ROOT = 0,                                           /**< Root device */
    DEV_TYPE_ADC,                                                /**< ADC device */
    DEV_TYPE_CAN,                                                /**< CAN device */
    DEV_TYPE_DAC,                                                /**< DAC device */
    DEV_TYPE_I2C,                                                /**< I2C device */
    DEV_TYPE_PIN,                                                /**< PIN device */
    DEV_TYPE_SERIAL,                                             /**< Serial device */
    DEV_TYPE_SPI,                                                /**< SPI device */
    DEV_TYPE_TIMER,                                              /**< Timer device */
    DEV_TYPE_PWM,                                                /**< PWM device */
    DEV_TYPE_COMPONENT,                                          /**< Component device */
};

/**
 * @addtogroup Basic
 * @{
 */
#define DEV_NULL                         ((void *)0)                 /**< Null pointer */
#define DEV_DISABLE                      (0)                         /**< Disable */
#define DEV_ENABLE                       (1)                         /**< Enable */
#define DEV_FALSE                        (0)                         /**< False */
#define DEV_TRUE                         (1)                         /**< True */
/** @} */

/**
 * @addtogroup Error
 * @{
 */
#define DEV_EOK                          (0)                         /**< No error */
#define DEV_ENOMEM                       (-1)                        /**< No enough memory */
#define DEV_EIO                          (-2)                        /**< I/O error */
#define DEV_ENOTFOUND                    (-3)                        /**< Not found */
#define DEV_EBUSY                        (-4)                        /**< Resource busy */
#define DEV_EEXIST                       (-5)                        /**< Exists */
#define DEV_ENOTSUP                      (-6)                        /**< Operation not supported */
#define DEV_EINVAL                       (-7)                        /**< Invalid argument */
#define DEV_ETIMEOUT                     (-8)                        /**< Timeout */
/** @} */

#define DEV_MAGIC_NUMBER                 (0xdeadbeef)                /**< Magic number */

#define DEV_SYNC                         (0)                         /**< Synchronous */
#define DEV_ASYNC                        (1)                         /**< Asynchronous */

/* [31:24] are for lock, [23:0] reserved */
#define DEV_LOCK_RD                      (0x01 << 24)                /**< Read lock */
#define DEV_LOCK_WR                      (0x02 << 24)                /**< Write lock */
#define DEV_LOCK_RDWR                    (0x03 << 24)                /**< Read/write lock */
#define DEV_LOCK_NONBLOCK                (0x04 << 24)                /**< Non-blocking lock */
#define DEV_LOCK_SLEEP                   (0x08 << 24)                /**< Sleep lock */

/* [31:24] are for basic flags, [23:0] can define user flags */
#define DEV_O_CLOSED                     (0)                         /**< Closed flag */
#define DEV_O_QUERY                      (0)                         /**< Query flag */
#define DEV_O_RDONLY                     (0x01 << 24)                /**< Read only flag */
#define DEV_O_WRONLY                     (0x02 << 24)                /**< Write only flag */
#define DEV_O_RDWR                       (0x03 << 24)                /**< Read/write flag */
#define DEV_O_NONBLOCK                   (0x04 << 24)                /**< Non-blocking flag */

/* [31:24] are for basic commands, [23:0] can define user commands. (>0): user -> device, (<0): user <- device */
#define DEV_IOC_SPOS                     (0x01 << 24)                /**< Set position command */
#define DEV_IOC_SRCB                     (0x02 << 24)                /**< Set read callback command */
#define DEV_IOC_SWCB                     (0x03 << 24)                /**< Set write callback command */
#define DEV_IOC_SCFG                     (0x04 << 24)                /**< Set configuration command */
#define DEV_IOC_SRBSZ                    (0x05 << 24)                /**< Set read buffer size command */
#define DEV_IOC_SWBSZ                    (0x06 << 24)                /**< Set write buffer size command */
#define DEV_IOC_CRBD                     (0x07 << 24)                /**< Clear read buffer data command */
#define DEV_IOC_CWBD                     (0x08 << 24)                /**< Clear write buffer data command */

#define DEV_IOC_GPOS                     (-(0x01 << 24))             /**< Get position command */
#define DEV_IOC_GRCB                     (-(0x02 << 24))             /**< Get read callback command */
#define DEV_IOC_GWCB                     (-(0x03 << 24))             /**< Get write callback command */
#define DEV_IOC_GCFG                     (-(0x04 << 24))             /**< Get configuration command */
#define DEV_IOC_GRBSZ                    (-(0x05 << 24))             /**< Get read buffer size command */
#define DEV_IOC_GWBSZ                    (-(0x06 << 24))             /**< Get write buffer size command */
#define DEV_IOC_GRBDSZ                   (-(0x07 << 24))             /**< Get read buffer data size command */
#define DEV_IOC_GWBDSZ                   (-(0x08 << 24))             /**< Get write buffer data size command */

/* [31:24] are for interrupt flags, [23:0] can define user flags */
#define DEV_ISR_RD                       (0x01 << 24)                /**< Read interrupt event */
#define DEV_ISR_WR                       (0x02 << 24)                /**< Write interrupt event */
#define DEV_ISR_MASK                     (0x7f << 24)                /**< Interrupt event mask */

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

/** 
* @brief   fuction
*/ 


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_DEV_CONF_H*/


/******************************* (END OF FILE) *********************************/


