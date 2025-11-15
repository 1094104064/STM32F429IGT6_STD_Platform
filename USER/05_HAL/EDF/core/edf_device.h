/**
  ******************************************************************************
  *
  * @file    edf_device.h
  * @author  
  * @brief   Header file of edf_device module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_DEVICE_H
#define _EDF_DEVICE_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "edf_list.h"
#include "edf_port.h"
/*********************
 *      DEFINES
 *********************/
/**
 * @brief Device types.
 */
enum edf_dev_type
{
    EDF_DEV_TYPE_ROOT = 0,                                           /**< Root device */
    EDF_DEV_TYPE_ADC,                                                /**< ADC device */
    EDF_DEV_TYPE_CAN,                                                /**< CAN device */
    EDF_DEV_TYPE_DAC,                                                /**< DAC device */
    EDF_DEV_TYPE_I2C,                                                /**< I2C device */
    EDF_DEV_TYPE_PIN,                                                /**< PIN device */
    EDF_DEV_TYPE_SERIAL,                                             /**< Serial device */
    EDF_DEV_TYPE_SPI,                                                /**< SPI device */
    EDF_DEV_TYPE_TIMER,                                              /**< Timer device */
    EDF_DEV_TYPE_PWM,                                                /**< PWM device */
    EDF_DEV_TYPE_COMPONENT,                                          /**< Component device */
};

/**********************
 *      TYPEDEFS
 **********************/
typedef struct edf_dev_ops edf_dev_ops_t;
typedef struct edf_drv edf_drv_t;
typedef struct edf_dev_call edf_dev_call_t;
typedef struct edf_dev_desc edf_dev_desc_t;
typedef struct edf_device edf_device_t;


/**
 * @brief Driver structure.
 */
struct edf_drv
{
    void *ops;                                                      /**< Operations */
    void *data;                                                     /**< Data */
};


/**
 * @brief Device operations structure.
 */
struct edf_dev_ops
{
    int     (* open)    (edf_device_t *dev);
    int     (* close)   (edf_device_t *dev);
    ssize_t (* read)    (edf_device_t *dev, void *buf, size_t count);
    ssize_t (* write)   (edf_device_t *dev, const void *buf, size_t count);
    int     (* ioctl)   (edf_device_t *dev, int cmd, void *args);
    ssize_t (* isr)     (edf_device_t *dev, int event, void *args);
};

/**
 * @brief Device callback structure.
 */
struct edf_dev_call
{
    edf_list_t list;                                                /**< List of callbacks */

    void (*fn)(int desc, void *args);                               /**< Callback function */
};


/**
 * @brief Device descriptor structure.
 */
struct edf_dev_desc
{
    edf_device_t *dev;                                              /**< Device */
    int flags;                                                      /**< Open flags */
    int position;                                                   /**< Current position */
    edf_dev_call_t rd_call;                                         /**< Read callback */
    edf_dev_call_t wr_call;                                         /**< Write callback */
};

/**
 * @brief Device structure.
 */
struct edf_device
{
    uint32_t magic;                                                 /**< Magic number */
#ifndef EDF_CFG_DEV_NAME_LEN
#define EDF_CFG_DEV_NAME_LEN             (8)
#endif /* EDF_CFG_DEV_NAME_LEN */
    char name[EDF_CFG_DEV_NAME_LEN];                                /**< Name */
    uint32_t type;                                                  /**< Type */
    int flags;                                                      /**< Flags */
    void *parent;                                                   /**< Parent device */
    edf_list_t list;                                                /**< Same level device list */
    edf_list_t clist;                                               /**< Child device list */

    size_t ref_count;                                               /**< Reference count */
#ifdef EDF_USING_RDWR_CTL
    volatile uint32_t lock;                                         /**< Lock flags */
#endif /* EDF_USING_RDWR_CTL */
    int sync;                                                       /**< Sync flag */
    int position;                                                   /**< Position */

    edf_list_t rd_call_list;                                        /**< Read callback list */
    edf_list_t wr_call_list;                                        /**< Write callback list */

    const edf_dev_ops_t *ops;                                       /**< Operations */
    const edf_drv_t *drv;                                           /**< Driver */
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/




/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_EDF_DEVICE_H*/


/******************************* (END OF FILE) *********************************/


