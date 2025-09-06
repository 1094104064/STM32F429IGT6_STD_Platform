/**
  ******************************************************************************
  *
  * @file    msp_emul_i2c.h
  * @author  Jamin
  * @brief   Header file of msp_emul_i2c module.
  *
  ******************************************************************************
  **/

#ifndef _MSP_EMUL_I2C_H
#define _MSP_EMUL_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "gpio.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct i2c_gpio {
    GPIO_TypeDef * scl_port;
    GPIO_TypeDef * sda_port;
    uint32_t scl_pin;
    uint32_t sda_pin;
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void msp_emul_i2c_start(struct i2c_gpio * self);
void msp_emul_i2c_stop(struct i2c_gpio * self);
void msp_emul_i2c_write_byte(struct i2c_gpio * self, uint8_t data);
uint8_t msp_emul_i2c_read_byte(struct i2c_gpio * self);
uint8_t msp_emul_i2c_wait_ack(struct i2c_gpio * self);
void msp_emul_i2c_generate_ack(struct i2c_gpio * self);
void msp_emul_i2c_generate_nack(struct i2c_gpio * self);
int msp_emul_i2c_write(struct i2c_gpio * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len);
int msp_emul_i2c_read(struct i2c_gpio * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_MSP_EMUL_I2C_H*/


/******************************* (END OF FILE) *********************************/

