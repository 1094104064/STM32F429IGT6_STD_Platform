/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_i2c.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_i2c module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_WRAPPER_I2C_H
#define _BSP_WRAPPER_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define I2C_WRAPPER_MAX_DEVICES  4
/**********************
 *      TYPEDEFS
 **********************/

struct i2c_wrapper {
    int8_t idx;
    void * user_data;

    void (* init)(struct i2c_wrapper * self);
    void (* start)(struct i2c_wrapper * self);
    void (* stop)(struct i2c_wrapper * self);
    void (* write_byte)(struct i2c_wrapper * self, uint8_t data);
    uint8_t (* read_byte)(struct i2c_wrapper * self);
    uint8_t (* wait_ack)(struct i2c_wrapper * self);
    void (* generate_ack)(struct i2c_wrapper * self);
    void (* generate_nack)(struct i2c_wrapper * self);
    void (* read)(struct i2c_wrapper * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len);
    void (* write)(struct i2c_wrapper * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_i2c_link(struct i2c_wrapper * self);
void bsp_wrapper_i2c_init(void);
void bsp_wrapper_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len);
void bsp_wrapper_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_I2C_H*/


/******************************* (END OF FILE) *********************************/

