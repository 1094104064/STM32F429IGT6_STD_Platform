/**
  ******************************************************************************
  *
  * @file    edf_i2c.h
  * @author  
  * @brief   Header file of edf_i2c module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_I2C_H
#define _EDF_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "edf_device.h"
#include "edf_ringbuf.h"
/*********************
 *      DEFINES
 *********************/
/**
 * @brief I2C host/slave.
 */
#define EDF_I2C_HOST                     (0)                         /**< I2C host */
#define EDF_I2C_SLAVE                    (1)                         /**< I2C slave */

/**
 * @brief I2C register bits.
 */
#define EDF_I2C_REG_BITS_8               (8)                         /**< 8 bits register */
#define EDF_I2C_REG_BITS_16              (16)                        /**< 16 bits register */
#define EDF_I2C_REG_BITS_32              (32)                        /**< 32 bits register */

/**
 * @brief I2C control command.
 */
#define EDF_IOC_I2C_SET_CONFIG           EDF_IOC_SCFG                 /**< Set configuration command */
#define EDF_IOC_I2C_SET_REG              EDF_IOC_SPOS                 /**< Set register command */
#define EDF_IOC_I2C_SET_RD_BUFSZ         EDF_IOC_SRBSZ                /**< Set read buffer size command */
#define EDF_IOC_I2C_CLR_RD_BUF           EDF_IOC_CRBD                 /**< Clear read buffer command */
#define EDF_IOC_I2C_SET_RD_CALL          EDF_IOC_SRCB                 /**< Set read callback command */
#define EDF_IOC_I2C_GET_CONFIG           EDF_IOC_GCFG                 /**< Get configuration command */
#define EDF_IOC_I2C_GET_REG              EDF_IOC_GPOS                 /**< Get register command */
#define EDF_IOC_I2C_GET_RD_BUFSZ         EDF_IOC_GRBSZ                /**< Get read buffer size command */
#define EDF_IOC_I2C_GET_RD_DATASZ        EDF_IOC_GRBDSZ               /**< Get read data size command */
#define EDF_IOC_I2C_GET_RD_CALL          EDF_IOC_GRCB                 /**< Get read callback command */

/**
 * @brief I2C ISR events.
 */
#define EDF_ISR_I2C_RD_INT               (EDF_ISR_RD | (0x01))        /**< Read interrupt event */

/**
 * @brief I2C device address bits.
 */
#define EDF_I2C_ADDR_BITS_7              (7)                         /**< 7 bit address */
#define EDF_I2C_ADDR_BITS_10             (10)                        /**< 10 bit address */

/**
 * @brief I2C default configuration.
 */
#define EDF_I2C_CONFIG_DEFAULT           \
{                                       \
    100000,                             \
    EDF_I2C_HOST,                        \
    EDF_I2C_REG_BITS_8,                  \
}
/**********************
 *      TYPEDEFS
 **********************/
typedef struct edf_i2c_bus edf_i2c_bus_t;
typedef struct edf_i2c_dev edf_i2c_dev_t;
typedef struct edf_i2c_config edf_i2c_config_t;
typedef struct edf_i2c_bus_ops edf_i2c_bus_ops_t;

/**
 * @brief I2C data type.
 */
typedef uint8_t edf_i2c_data_t;                                      /**< I2C read/write data type */

/**
 * @brief I2C configuration structure.
 */
struct edf_i2c_config
{
    uint32_t baud_rate;                                             /**< Baud rate */
    int host_slave;                                                 /**< Host/slave */
    int reg_bits;                                                   /**< Register bits */
};

/**
 * @brief I2C bus structure.
 */
struct edf_i2c_bus
{
    edf_device_t dev;                                              /**< Device */

    edf_i2c_config_t config;                                    /**< Configuration */
    volatile void *owner;                                           /**< Owner */
    volatile int hold;                                              /**< Owner hold */
};

/**
 * @brief I2C bus operations structure.
 */
struct edf_i2c_bus_ops
{
    int (*configure)(edf_i2c_bus_t *i2c_bus,
                     edf_i2c_config_t *config,
                     int addr,
                     int addr_bits);
    void (*start)(edf_i2c_bus_t *i2c_bus);
    int (*send_addr)(edf_i2c_bus_t *i2c_bus, int addr, int addr_bits);
    void (*stop)(edf_i2c_bus_t *i2c_bus);
    int (*read)(edf_i2c_bus_t *i2c_bus, uint8_t *data, int ack_state);
    int (*write)(edf_i2c_bus_t *i2c_bus, uint8_t data);
};

/**
 * @brief I2C device structure.
 */
struct edf_i2c_dev
{
    edf_device_t dev;                                              /**< Device */

    edf_i2c_config_t config;                                    /**< Configuration */
    edf_ringbuf_t rd_fifo;                                      /**< Read FIFO */
    size_t rd_bufsz;                                                /**< Read buffer size */
    int addr;                                                       /**< Address */
    int addr_bits;                                                  /**< Address bits */
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
int edf_i2c_bus_register(edf_i2c_bus_t *i2c_bus, const char *path, edf_drv_t *drv);
int edf_i2c_dev_register(edf_i2c_dev_t *i2c_dev, const char *path, int addr, int addr_bits);
   





/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_EDF_I2C_H*/


/******************************* (END OF FILE) *********************************/


