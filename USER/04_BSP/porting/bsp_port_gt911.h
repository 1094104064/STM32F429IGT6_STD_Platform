/**
  ******************************************************************************
  *
  * @file    bsp_port_gt911.h
  * @author  Jamin
  * @brief   Header file of bsp_port_gt911 module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_PORT_GT911_H
#define _BSP_PORT_GT911_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "i2c.h"
#include "gpio.h"
#include "systick.h"
#include "msp_cluster_i2c.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void bsp_port_gt911_write_reg(uint8_t dev_addr, uint16_t reg_addr, uint8_t * src, uint16_t len)
{
    GT911_I2C_START();

    GT911_I2C_WRITE_BYTE(dev_addr);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    GT911_I2C_WRITE_BYTE(reg_addr >> 8);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    GT911_I2C_WRITE_BYTE(reg_addr & 0xFF);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    for(uint16_t i = 0; i < len; i++) {
        GT911_I2C_WRITE_BYTE(src[i]);
        if(GT911_I2C_WAIT_ACK()) {
            GT911_I2C_STOP();
            pr_error("I2C Error!");
            return;
        }
    }

    GT911_I2C_STOP();
}

static inline void bsp_port_gt911_read_reg(uint8_t dev_addr, uint16_t reg_addr, uint8_t * dst, uint16_t len)
{
    GT911_I2C_START();

    GT911_I2C_WRITE_BYTE(dev_addr);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    GT911_I2C_WRITE_BYTE(reg_addr >> 8);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    GT911_I2C_WRITE_BYTE(reg_addr & 0xFF);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    GT911_I2C_START();
    GT911_I2C_WRITE_BYTE(dev_addr | 0x01);
    if(GT911_I2C_WAIT_ACK()) {
        GT911_I2C_STOP();
        pr_error("I2C Error!");
        return;
    }

    for(uint16_t i = 0; i < len; i++) {
        dst[i] = GT911_I2C_READ_BYTE();
        if(i == (len - 1)) {
            GT911_I2C_NACK();
        } else {
            GT911_I2C_ACK();
        }
    }

    GT911_I2C_STOP();
}

static inline void bsp_port_gt911_int_in(void)
{
    STD_GPIO_InputMode(GT911_INT_PORT, GT911_INT_PIN);
}

static inline void bsp_port_gt911_int_out(void)
{
    STD_GPIO_OutputMode(GT911_INT_PORT, GT911_INT_PIN);
}

static inline void bsp_port_gt911_int_high(void)
{
    GPIO_SetBits(GT911_INT_PORT, GT911_INT_PIN);
}

static inline void bsp_port_gt911_int_low(void)
{
    GPIO_ResetBits(GT911_INT_PORT, GT911_INT_PIN);
}

static inline void bsp_port_gt911_rst_high(void)
{
    GPIO_SetBits(GT911_RST_PORT, GT911_RST_PIN);
}

static inline void bsp_port_gt911_rst_low(void)
{
    GPIO_ResetBits(GT911_RST_PORT, GT911_RST_PIN);
}

static inline void bsp_port_gt911_delay_ms(uint32_t ms)
{
    STD_SYSTICK_Delay(ms);
}


/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_GT911_H*/


/******************************* (END OF FILE) *********************************/

