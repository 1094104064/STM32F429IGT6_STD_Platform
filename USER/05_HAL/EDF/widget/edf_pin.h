/**
  ******************************************************************************
  *
  * @file    edf_pin.h
  * @author  
  * @brief   Header file of edf_pin module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_PIN_H
#define _EDF_PIN_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "edf_device.h"
/*********************
 *      DEFINES
 *********************/
/**
 * @brief PIN mode.
 */
#define EDF_PIN_MODE_NONE                (0)                         /**< No mode */
#define EDF_PIN_MODE_OUTPUT              (1)                         /**< Output push-pull mode */
#define EDF_PIN_MODE_OUTPUT_OD           (2)                         /**< Output open-drain mode */
#define EDF_PIN_MODE_INPUT               (3)                         /**< Input mode */
#define EDF_PIN_MODE_INPUT_DOWN          (4)                         /**< Input pull-down mode */
#define EDF_PIN_MODE_INPUT_UP            (5)                         /**< Input pull-up mode */

/**
 * @brief PIN mode-interrupt.
 */
#define EDF_PIN_MODE_IRQ_RISING          (6)                         /**< Interrupt rising edge mode */
#define EDF_PIN_MODE_IRQ_FALLING         (7)                         /**< Interrupt falling edge mode */
#define EDF_PIN_MODE_IRQ_EDGE            (8)                         /**< Interrupt edge mode */
#define EDF_PIN_MODE_IRQ_LOW             (9)                         /**< Interrupt low level mode */
#define EDF_PIN_MODE_IRQ_HIGH            (10)                        /**< Interrupt high level mode */
/**
 * @brief PIN control command.
 */
#define EDF_IOC_PIN_SET_NUMBER           EDF_IOC_SPOS                 /**< Set pin number command */
#define EDF_IOC_PIN_SET_MODE             EDF_IOC_SCFG                 /**< Set pin mode command */
#define EDF_IOC_PIN_SET_EXTI_CALL        EDF_IOC_SRCB                 /**< Set pin exti callback command */

#define EDF_IOC_PIN_GET_NUMBER           EDF_IOC_GPOS                 /**< Get pin number command */
#define EDF_IOC_PIN_GET_MODE             EDF_IOC_GCFG                 /**< Get pin mode command */
#define EDF_IOC_PIN_GET_EXTI_CALL        EDF_IOC_GRCB                 /**< Get pin exti callback command */

/**
 * @brief PIN ISR events.
 */
#define EDF_ISR_PIN_EXTI_INT             (EDF_ISR_RD | (0x01))        /**< Exti interrupt event */
/**********************
 *      TYPEDEFS
 **********************/
typedef struct edf_pin edf_pin_t;
typedef struct edf_pin_config edf_pin_config_t;
typedef struct edf_pin_ops edf_pin_ops_t;
/**
 * @brief PIN data type.
 */
typedef uint8_t edf_pin_data_t;                                      /**< PIN read/write data type */

 /**
 * @brief PIN configuration structure.
 */
struct edf_pin_config
{
    int mode;                                                       /**< Mode */
};

/**
 * @brief PIN structure.
 */
struct edf_pin
{
    edf_device_t dev;                                              /**< Device */

    uint32_t pins[32];                                              /**< Pins */
};

/**
 * @brief PIN operations structure.
 */
struct edf_pin_ops
{
    int (* configure)(edf_pin_t *pin, int number, int mode);
    int (* read)     (edf_pin_t *pin, int number, uint8_t *value);
    int (* write)    (edf_pin_t *pin, int number, uint8_t value);
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/
int edf_pin_register(edf_pin_t *pin, const char *path, edf_drv_t *drv);
void _edf_fast_pin_init(edf_device_t *dev);
int _edf_fast_pin_mode(int number, int mode);
uint8_t _edf_fast_pin_read(int number);
void _edf_fast_pin_write(int number, int value);













/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_EDF_PIN_H*/


/******************************* (END OF FILE) *********************************/


