/**
  ******************************************************************************
  *
  * @file    msp_cluster_i2c.h
  * @author  Jamin
  * @brief   Header file of msp_cluster_i2c module.
  *
  ******************************************************************************
  **/

#ifndef _MSP_CLUSTER_I2C_H
#define _MSP_CLUSTER_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "msp_emul_i2c.h"
#include "user_conf.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

enum {
    I2C_SENSOR      = 0,
    I2C_GT911,
    I2C_MAX
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void msp_cluster_i2c_start(uint8_t cluster);
void msp_cluster_i2c_stop(uint8_t cluster);
void msp_cluster_i2c_write_byte(uint8_t cluster, uint8_t data);
uint8_t msp_cluster_i2c_read_byte(uint8_t cluster);
uint8_t msp_cluster_i2c_wait_ack(uint8_t cluster);
void msp_cluster_i2c_generate_ack(uint8_t cluster);
void msp_cluster_i2c_generate_nack(uint8_t cluster);
void msp_cluster_i2c_write(uint8_t cluster, uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len);
void msp_cluster_i2c_read(uint8_t cluster, uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len);
/**********************
 *      MACROS
 **********************/

#define SENSOR_I2C_WRITE   msp_cluster_i2c_write(I2C_SENSOR)
#define SENSOR_I2C_READ    msp_cluster_i2c_read(I2C_SENSOR)



#define GT911_I2C_START()               msp_cluster_i2c_start(I2C_GT911)
#define GT911_I2C_STOP()                msp_cluster_i2c_stop(I2C_GT911)
#define GT911_I2C_WRITE_BYTE(data)      msp_cluster_i2c_write_byte(I2C_GT911, data)
#define GT911_I2C_READ_BYTE()           msp_cluster_i2c_read_byte(I2C_GT911)
#define GT911_I2C_WAIT_ACK()            msp_cluster_i2c_wait_ack(I2C_GT911)
#define GT911_I2C_ACK()                 msp_cluster_i2c_generate_ack(I2C_GT911)
#define GT911_I2C_NACK()                msp_cluster_i2c_generate_nack(I2C_GT911)


#ifdef __cplusplus
}
#endif

#endif /*_MSP_CLUSTER_I2C_H*/


/******************************* (END OF FILE) *********************************/

