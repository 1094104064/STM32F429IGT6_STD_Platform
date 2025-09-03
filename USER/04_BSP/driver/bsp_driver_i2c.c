/**
  ******************************************************************************
  *
  * @file    bsp_driver_i2c.c
  * @author  Jamin
  * @brief   
  *
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  **/
  
  
/*********************
 *      INCLUDES
 *********************/
#include "bsp_driver_i2c.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void i2c_bus_init(struct i2c_driver * self)
{
    if (self == NULL || self->oper == NULL) {
        return;
    }

    if ((self->oper->init == NULL) ||
        (self->oper->sda_high == NULL) ||
        (self->oper->sda_low == NULL) ||
        (self->oper->scl_high == NULL) ||
        (self->oper->scl_low == NULL) ||
        (self->oper->read_bit == NULL) ||
        (self->oper->delay_us == NULL)) {
        return;
    }

    self->oper->init();

}

static void i2c_bus_start(struct i2c_driver * self)
{
    self->oper->sda_high();
    self->oper->scl_high();
    self->oper->delay_us(2);

    self->oper->sda_low();
    self->oper->delay_us(2);

    self->oper->scl_low();
    self->oper->delay_us(2);
}

static void i2c_bus_stop(struct i2c_driver * self)
{
    self->oper->scl_low();
    self->oper->delay_us(2);
    self->oper->sda_low();
    self->oper->delay_us(2);

    self->oper->scl_high();
    self->oper->delay_us(2);

    self->oper->sda_high();
    self->oper->delay_us(2);
}

static void i2c_bus_write_byte(struct i2c_driver * self, uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        if (data & 0x80) {
            self->oper->sda_high();
        } 
        else {
            self->oper->sda_low();
        }
        self->oper->delay_us(2);
        self->oper->scl_high();

        self->oper->delay_us(2);
        self->oper->scl_low();

        if (i == 7) {
            self->oper->sda_high();
        }

        data <<= 1;
        self->oper->delay_us(2);
    }
}

static uint8_t i2c_bus_read_byte(struct i2c_driver * self)
{
    uint8_t data = 0;

    for (int i = 0; i < 8; i++) {

        data <<= 1;

        self->oper->scl_high();
        self->oper->delay_us(2);

        if (self->oper->read_bit()) {
            data++;
        }

        self->oper->scl_low();
        self->oper->delay_us(2);
    }

    return data;
}

static uint8_t i2c_bus_wait_ack(struct i2c_driver * self)
{
    uint8_t ack = 0;

    self->oper->sda_high();
    self->oper->delay_us(2);

    self->oper->scl_high();
    self->oper->delay_us(2);

    if (self->oper->read_bit()) {
        ack = 1;
    }
    else {
        ack = 0;
    }

    self->oper->scl_low();
    self->oper->delay_us(2);

    return ack;
}

static void i2c_bus_generate_ack(struct i2c_driver * self)
{
    self->oper->sda_low();
    self->oper->delay_us(2);

    self->oper->scl_high();
    self->oper->delay_us(2);

    self->oper->sda_high();
    self->oper->scl_low();
    self->oper->delay_us(2);
}

static void i2c_bus_generate_nack(struct i2c_driver * self)
{
    self->oper->scl_low();
    self->oper->delay_us(2);

    self->oper->sda_high();
    self->oper->delay_us(2);

    self->oper->scl_high();
    self->oper->delay_us(2);

    self->oper->scl_low();
    self->oper->delay_us(2);
}

static void i2c_bus_write(struct i2c_driver * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len)
{
    i2c_bus_start(self);

    i2c_bus_write_byte(self, dev_addr);
    if (i2c_bus_wait_ack(self)) {
        i2c_bus_stop(self);
        return;
    }

    i2c_bus_write_byte(self, reg_addr);
    if (i2c_bus_wait_ack(self)) {
        i2c_bus_stop(self);
        return;
    }

    for (uint16_t i = 0; i < len; i++) {
        i2c_bus_write_byte(self, src[i]);
        if (i2c_bus_wait_ack(self)) {
            i2c_bus_stop(self);
            return;
        }
    }

    i2c_bus_stop(self);
}

static void i2c_bus_read(struct i2c_driver * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len)
{
    i2c_bus_start(self);

    i2c_bus_write_byte(self, dev_addr);
    if (i2c_bus_wait_ack(self)) {
        i2c_bus_stop(self);
        return;
    }

    i2c_bus_write_byte(self, reg_addr);
    if (i2c_bus_wait_ack(self)) {
        i2c_bus_stop(self);
        return;
    }

    i2c_bus_stop(self);

    i2c_bus_start(self);

    i2c_bus_write_byte(self, dev_addr | 0x01);
    if (i2c_bus_wait_ack(self)) {
        i2c_bus_stop(self);
        return;
    }

    for (uint16_t i = 0; i < len; i++) {
        dst[i] = i2c_bus_read_byte(self);
        if (i == (len - 1)) {
            i2c_bus_generate_nack(self);
        } else {
            i2c_bus_generate_ack(self);
        }
    }

    i2c_bus_stop(self);
}

/******************************* (END OF FILE) *********************************/

