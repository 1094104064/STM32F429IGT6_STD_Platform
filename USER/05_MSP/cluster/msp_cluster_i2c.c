/**
  ******************************************************************************
  *
  * @file    msp_cluster_i2c.c
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
#include "msp_cluster_i2c.h"
/**********************
 *      MACROS
 **********************/
#define I2C_LOCK()
#define I2C_UNLOCK()
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

static struct i2c_gpio i2c_clusters[I2C_MAX] = {
    [I2C_SENSOR] = {
        .sda_port = GPIOB,
        .sda_pin = GPIO_Pin_7,
        .scl_port = GPIOB,
        .scl_pin = GPIO_Pin_6
    },
    [I2C_GT911] = {
        .sda_port = GT911_SDA_PORT,
        .sda_pin = GT911_SDA_PIN,
        .scl_port = GT911_SCL_PORT,
        .scl_pin = GT911_SCL_PIN
    }
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void msp_cluster_i2c_start(uint8_t cluster)
{
    if (cluster >= I2C_MAX) {
        return;
    }
    I2C_LOCK();
    msp_emul_i2c_start(&i2c_clusters[cluster]);
}

void msp_cluster_i2c_stop(uint8_t cluster)
{
    if (cluster >= I2C_MAX) {
        return;
    }

    msp_emul_i2c_stop(&i2c_clusters[cluster]);
    I2C_UNLOCK();
}

void msp_cluster_i2c_write_byte(uint8_t cluster, uint8_t data)
{
    if (cluster >= I2C_MAX) {
        return;
    }

    msp_emul_i2c_write_byte(&i2c_clusters[cluster], data);
}

uint8_t msp_cluster_i2c_read_byte(uint8_t cluster)
{
    if (cluster >= I2C_MAX) {
        return 0;
    }

    return msp_emul_i2c_read_byte(&i2c_clusters[cluster]);
}

uint8_t msp_cluster_i2c_wait_ack(uint8_t cluster)
{
    if (cluster >= I2C_MAX) {
        return 1;
    }

    return msp_emul_i2c_wait_ack(&i2c_clusters[cluster]);
}

void msp_cluster_i2c_generate_ack(uint8_t cluster)
{
    if (cluster >= I2C_MAX) {
        return;
    }

    msp_emul_i2c_generate_ack(&i2c_clusters[cluster]);
}

void msp_cluster_i2c_generate_nack(uint8_t cluster)
{
    if (cluster >= I2C_MAX) {
        return;
    }

    msp_emul_i2c_generate_nack(&i2c_clusters[cluster]);
}

void msp_cluster_i2c_write(uint8_t cluster, uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len)
{
    if (cluster >= I2C_MAX) {
        return;
    }

    msp_emul_i2c_write(&i2c_clusters[cluster], dev_addr, reg_addr, src, len);
}

void msp_cluster_i2c_read(uint8_t cluster, uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len)
{
    if (cluster >= I2C_MAX) {
        return;
    }

    msp_emul_i2c_read(&i2c_clusters[cluster], dev_addr, reg_addr, dst, len);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

