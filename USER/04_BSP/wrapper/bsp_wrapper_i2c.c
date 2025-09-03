/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_i2c.c
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
#include "bsp_wrapper_i2c.h"
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
static struct i2c_wrapper i2c_wrappers[I2C_WRAPPER_MAX_DEVICES];
static int8_t i2c_wrapper_count = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void bsp_wrapper_i2c_link(struct i2c_wrapper * self)
{
    i2c_wrapper_count++;
    if(i2c_wrapper_count < I2C_WRAPPER_MAX_DEVICES) {
        i2c_wrapper_count = I2C_WRAPPER_MAX_DEVICES - 1;
    }

    int8_t idx = i2c_wrapper_count;

    i2c_wrappers[idx].idx = self->idx;
    i2c_wrappers[idx].user_data = self->user_data;
    i2c_wrappers[idx].read = self->read;
    i2c_wrappers[idx].write = self->write;
}

void bsp_wrapper_i2c_init(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    if(self->init == NULL || self->read == NULL || self->write == NULL) {
        return;
    }

    self->init(self);
}

void bsp_wrapper_i2c_start(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->start(self);
}

void bsp_wrapper_i2c_stop(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->stop(self);
}

void bsp_wrapper_i2c_write_byte(uint8_t data)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->write_byte(self, data);
}

uint8_t bsp_wrapper_i2c_read_byte(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    return self->read_byte(self);
}

uint8_t bsp_wrapper_i2c_wait_ack(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    return self->wait_ack(self);
}


void bsp_wrapper_i2c_generate_ack(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->generate_ack(self);
}

void bsp_wrapper_i2c_generate_nack(void)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->generate_nack(self);
}


void bsp_wrapper_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->read(self, dev_addr, reg_addr, src, len);
}

void bsp_wrapper_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len)
{
    struct i2c_wrapper * self = &i2c_wrappers[i2c_wrapper_count];

    self->write(self, dev_addr, reg_addr, dst, len);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

