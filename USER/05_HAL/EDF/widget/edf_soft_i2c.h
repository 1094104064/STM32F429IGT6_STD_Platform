/**
  ******************************************************************************
  *
  * @file    edf_soft_i2c.h
  * @author  
  * @brief   Header file of edf_soft_i2c module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_SOFT_I2C_H
#define _EDF_SOFT_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "edf_pin.h"
#include "edf_i2c.h"
#include "edf_port.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct edf_soft_i2c_bus edf_soft_i2c_bus_t;
/**
* @brief Soft-I2C bus structure.
*/
struct edf_soft_i2c_bus
{
    edf_i2c_bus_t i2c_bus;                                      /**< I2C-bus device */

    uint32_t delay;                                                 /**< Speed delay */
    int scl_pin;                                                    /**< SCL pin */
    int sda_pin;                                                    /**< SDA pin */
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
int edf_soft_i2c_bus_register(edf_soft_i2c_bus_t *soft_i2c_bus,
                              const char *path,
                              int scl_pin,
                              int sda_pin);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_EDF_SOFT_I2C_H*/


/******************************* (END OF FILE) *********************************/


