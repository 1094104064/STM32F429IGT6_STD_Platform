/**
  ******************************************************************************
  *
  * @file    bsp_driver_i2c.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_i2c module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_DRIVER_I2C_H
#define _BSP_DRIVER_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct i2c_oper {
    void (* init)(void);
    void (* sda_high)(void);
    void (* sda_low)(void);
    void (* scl_high)(void);
    void (* scl_low)(void);
    uint8_t (* read_bit)(void);
    void (* delay_us)(uint8_t wait);
};


struct i2c_driver {
    struct i2c_oper * oper;

    void (* init)(void);
    void (* read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len);
    void (* write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len);
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

#endif /*_BSP_DRIVER_I2C_H*/


/******************************* (END OF FILE) *********************************/

