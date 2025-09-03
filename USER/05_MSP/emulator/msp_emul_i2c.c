/**
  ******************************************************************************
  *
  * @file    msp_emul_i2c.c
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
#include "msp_emul_i2c.h"
/**********************
 *      MACROS
 **********************/
#define SDA_HIGH(x)     GPIO_SetBits((x)->sda_port, (x)->sda_pin)
#define SDA_LOW(x)      GPIO_ResetBits((x)->sda_port, (x)->sda_pin)
#define SCL_HIGH(x)     GPIO_SetBits((x)->scl_port, (x)->scl_pin)
#define SCL_LOW(x)      GPIO_ResetBits((x)->scl_port, (x)->scl_pin)
#define READ_SDA(x)     GPIO_ReadInputDataBit((x)->sda_port, (x)->sda_pin)
/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void delay_us(uint32_t us);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void msp_emul_i2c_start(struct i2c_gpio * self)
{
    SDA_HIGH(self);
    SCL_HIGH(self);
    delay_us(2);

    SDA_LOW(self);
    delay_us(2);

    SCL_LOW(self);
    delay_us(2);
}

void msp_emul_i2c_stop(struct i2c_gpio * self)
{
    SCL_LOW(self);
    delay_us(2);
    SDA_LOW(self);
    delay_us(2);

    SCL_HIGH(self);
    delay_us(2);

    SDA_HIGH(self);
    delay_us(2);
}

void msp_emul_i2c_write_byte(struct i2c_gpio * self, uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        if (data & 0x80) {
            SDA_HIGH(self);
        } 
        else {
            SDA_LOW(self);
        }
        delay_us(2);
        SCL_HIGH(self);

        delay_us(2);
        SCL_LOW(self);

        if (i == 7) {
            SDA_HIGH(self);
        }

        data <<= 1;
        delay_us(2);
    }
}

uint8_t msp_emul_i2c_read_byte(struct i2c_gpio * self)
{
    uint8_t data = 0;

    for (int i = 0; i < 8; i++) {

        data <<= 1;

        SCL_HIGH(self);
        delay_us(2);

        if (READ_SDA(self)) {
            data++;
        }

        SCL_LOW(self);
        delay_us(2);
    }

    return data;
}

uint8_t msp_emul_i2c_wait_ack(struct i2c_gpio * self)
{
    uint8_t ack;

    SDA_HIGH(self);
    delay_us(2);

    SCL_HIGH(self);
    delay_us(2);

    ack = READ_SDA(self);

    SCL_LOW(self);
    delay_us(2);

    return ack;
}

void msp_emul_i2c_generate_ack(struct i2c_gpio * self)
{
    SDA_LOW(self);
    delay_us(4);

    SCL_HIGH(self);
    delay_us(2);

    SCL_LOW(self);
    SDA_HIGH(self);
    delay_us(2);
}

void msp_emul_i2c_generate_nack(struct i2c_gpio * self)
{
    SCL_LOW(self);
    delay_us(2);

    SDA_HIGH(self);
    delay_us(2);

    SCL_HIGH(self);
    delay_us(2);

    SCL_LOW(self);
    delay_us(2);
}

void msp_emul_i2c_write(struct i2c_gpio * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len)
{
    msp_emul_i2c_start(self);

    msp_emul_i2c_write_byte(self, dev_addr);
    if (msp_emul_i2c_wait_ack(self)) {
        msp_emul_i2c_stop(self);
        pr_error("I2C write failed: no ACK received");
        return;
    }

    msp_emul_i2c_write_byte(self, reg_addr);
    if (msp_emul_i2c_wait_ack(self)) {
        msp_emul_i2c_stop(self);
        pr_error("I2C write failed: no ACK received");
        return;
    }

    for (uint16_t i = 0; i < len; i++) {
        msp_emul_i2c_write_byte(self, src[i]);
        if (msp_emul_i2c_wait_ack(self)) {
            msp_emul_i2c_stop(self);
            pr_error("I2C write failed: no ACK received");
            return;
        }
    }

    msp_emul_i2c_stop(self);
}

void msp_emul_i2c_read(struct i2c_gpio * self, uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len)
{
    msp_emul_i2c_start(self);

    msp_emul_i2c_write_byte(self, dev_addr);
    if (msp_emul_i2c_wait_ack(self)) {
        msp_emul_i2c_stop(self);
        pr_error("I2C read failed: no ACK received");
        return;
    }

    msp_emul_i2c_write_byte(self, reg_addr);
    if (msp_emul_i2c_wait_ack(self)) {
        msp_emul_i2c_stop(self);
        pr_error("I2C read failed: no ACK received");
        return;
    }

    msp_emul_i2c_start(self);
    msp_emul_i2c_write_byte(self, dev_addr | 0x01);
    if (msp_emul_i2c_wait_ack(self)) {
        msp_emul_i2c_stop(self);
        pr_error("I2C read failed: no ACK received");
        return;
    }

    for (uint16_t i = 0; i < len; i++) {
        dst[i] = msp_emul_i2c_read_byte(self);
        if (i == (len - 1)) {
            msp_emul_i2c_generate_nack(self);
        } else {
            msp_emul_i2c_generate_ack(self);
        }
    }

    msp_emul_i2c_stop(self);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void delay_us(uint32_t us)
{
    /* SystemCoreClock 为实际系统时钟频率 */
    /* 4 是典型值（1条nop≈4时钟周期），需根据具体MCU调整 */
    uint32_t cycles = us * (SystemCoreClock / 1000000UL) / 4;
    while(cycles--) {
        __asm volatile ("nop"); // 空操作指令
    }
}


/******************************* (END OF FILE) *********************************/

