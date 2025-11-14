/**
  ******************************************************************************
  *
  * @file    dev_pin.h
  * @author  
  * @brief   Header file of dev_pin module.
  *
  ******************************************************************************
  **/
  
#ifndef _DEV_PIN_H
#define _DEV_PIN_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "platform_device.h"
/*********************
 *      DEFINES
 *********************/
typedef struct dev_pin_config dev_pin_conf_t;
typedef struct dev_pin dev_pin_t;
typedef struct dev_pin_ops dev_pin_ops_t;

/**
 * @brief PIN mode.
 */
#define DEV_PIN_MODE_NONE                (0)                         /**< No mode */
#define DEV_PIN_MODE_OUTPUT              (1)                         /**< Output push-pull mode */
#define DEV_PIN_MODE_OUTPUT_OD           (2)                         /**< Output open-drain mode */
#define DEV_PIN_MODE_INPUT               (3)                         /**< Input mode */
#define DEV_PIN_MODE_INPUT_DOWN          (4)                         /**< Input pull-down mode */
#define DEV_PIN_MODE_INPUT_UP            (5)  

/**
 * @brief PIN mode-interrupt.
 */
#define DEV_PIN_MODE_IRQ_RISING          (6)                         /**< Interrupt rising edge mode */
#define DEV_PIN_MODE_IRQ_FALLING         (7)                         /**< Interrupt falling edge mode */
#define DEV_PIN_MODE_IRQ_EDGE            (8)                         /**< Interrupt edge mode */
#define DEV_PIN_MODE_IRQ_LOW             (9)                         /**< Interrupt low level mode */
#define DEV_PIN_MODE_IRQ_HIGH            (10)  

/**
 * @brief PIN configuration structure.
 */
struct dev_pin_config
{
    int mode;                                                       /**< Mode */
};

/**
 * @brief PIN control command.
 */
#define DEV_IOC_PIN_SET_NUMBER           DEV_IOC_SPOS                 /**< Set pin number command */
#define DEV_IOC_PIN_SET_MODE             DEV_IOC_SCFG                 /**< Set pin mode command */
#define DEV_IOC_PIN_SET_EXTI_CALL        DEV_IOC_SRCB                 /**< Set pin exti callback command */

#define DEV_IOC_PIN_GET_NUMBER           DEV_IOC_GPOS                 /**< Get pin number command */
#define DEV_IOC_PIN_GET_MODE             DEV_IOC_GCFG                 /**< Get pin mode command */
#define DEV_IOC_PIN_GET_EXTI_CALL        DEV_IOC_GRCB  

/**
 * @brief PIN data type.
 */
typedef uint8_t DEV_pin_data_t;                                      /**< PIN read/write data type */

/**
 * @brief PIN ISR events.
 */
#define DEV_ISR_PIN_EXTI_INT             (DEV_ISR_RD | (0x01))        /**< Exti interrupt event */

/**
 * @brief PIN structure.
 */
struct dev_pin
{
    plaf_dev_t dev;                                              /**< Device */

    uint32_t pins[32];                                              /**< Pins */
};

/**
 * @brief PIN operations structure.
 */
struct dev_pin_ops
{
    int (* configure)(dev_pin_t * pin, int number, int mode);
    int (* read)     (dev_pin_t * pin, int number, uint8_t * value);
    int (* write)    (dev_pin_t * pin, int number, uint8_t value);
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_DEV_PIN_H*/


/******************************* (END OF FILE) *********************************/


