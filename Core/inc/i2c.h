/**
  ******************************************************************************
  *
  * @file    i2c.h
  * @author  Jamin
  * @brief   Header file of i2c module.
  *
  ******************************************************************************
  **/

#ifndef _I2C_H
#define _I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "stm32f4xx.h"
/*********************
 *      DEFINES
 *********************/
#define BMP180_SDA_PORT    GPIOB
#define BMP180_SDA_PIN     GPIO_Pin_7

#define BMP180_SCL_PORT    GPIOB
#define BMP180_SCL_PIN     GPIO_Pin_6
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

#endif /*_I2C_H*/


/******************************* (END OF FILE) *********************************/

