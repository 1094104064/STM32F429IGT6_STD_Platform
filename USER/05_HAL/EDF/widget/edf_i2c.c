/**
  ******************************************************************************
  *
  * @file    edf_i2c.c
  * @author  
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
#include "edf_i2c.h"
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
static int i2c_bus_open(edf_device_t *dev);
static int i2c_bus_close(edf_device_t *dev);
static ssize_t i2c_bus_read(edf_device_t *dev, void *buf, size_t count);
static ssize_t i2c_bus_write(edf_device_t *dev, const void *buf, size_t count);
static ssize_t i2c_bus_isr(edf_device_t *dev, int event, void *args);

EDF_INLINE int i2c_dev_take_bus(edf_i2c_dev_t *i2c_dev);
EDF_INLINE int i2c_dev_release_bus(edf_i2c_dev_t *i2c_dev);
EDF_INLINE int i2c_dev_send_addr(edf_i2c_dev_t *i2c_dev, int rdwr);
EDF_INLINE void i2c_dev_send_stop(edf_i2c_dev_t *i2c_dev);
EDF_INLINE ssize_t i2c_dev_read_handler(edf_i2c_dev_t *i2c_dev, uint8_t *buf, size_t count);
EDF_INLINE ssize_t i2c_dev_write_handler(edf_i2c_dev_t *i2c_dev, const uint8_t *buf, size_t count);
static int i2c_dev_open(edf_device_t *dev);
static int i2c_dev_close(edf_device_t *dev);
static ssize_t i2c_dev_read(edf_device_t *dev, void *buf, size_t count);
static ssize_t i2c_dev_write(edf_device_t *dev, const void *buf, size_t count);
static int i2c_dev_ioctl(edf_device_t *dev, int cmd, void *args);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
 * @brief This function registers a i2c-bus.
 *
 * @param i2c_bus The i2c-bus.
 * @param path The path of the i2c-bus.
 * @param drv The driver of the i2c-bus.
 *
 * @return 0 on success, otherwise an error code.
 */
int edf_i2c_bus_register(edf_i2c_bus_t *i2c_bus, const char *path, edf_drv_t *drv)
{
    static edf_dev_ops_t ops = {i2c_bus_open,
                                i2c_bus_close,
                                i2c_bus_read,
                                i2c_bus_write,
                                EDF_NULL,
                                i2c_bus_isr};
    struct edf_i2c_config default_config = EDF_I2C_CONFIG_DEFAULT;

    EDF_ASSERT(i2c_bus != EDF_NULL);
    EDF_ASSERT(path != EDF_NULL);
    EDF_ASSERT(drv != EDF_NULL);
    EDF_ASSERT(drv->ops != EDF_NULL);

    /* Initialize the fields */
    i2c_bus->config = default_config;
    i2c_bus->owner = EDF_NULL;
    i2c_bus->hold = EDF_FALSE;

    /* Register the i2c-bus */
    return edf_dev_register(&i2c_bus->dev, path, EDF_DEV_TYPE_I2C, EDF_O_RDWR, &ops, drv);
}

/**
 * @brief This function registers a i2c-device.
 *
 * @param i2c_dev The i2c-device.
 * @param path The path of the i2c-device.
 * @param addr The address of the i2c-device.
 * @param addr_bits The number of address bits of the i2c-device.
 *
 * @return 0 on success, otherwise an error code.
 */
int edf_i2c_dev_register(edf_i2c_dev_t *i2c_dev, const char *path, int addr, int addr_bits)
{
    static edf_dev_ops_t ops = {i2c_dev_open,
                                i2c_dev_close,
                                i2c_dev_read,
                                i2c_dev_write,
                                i2c_dev_ioctl,
                                EDF_NULL};
    edf_i2c_config_t default_config = EDF_I2C_CONFIG_DEFAULT;

    EDF_ASSERT(i2c_dev != EDF_NULL);
    EDF_ASSERT(path != EDF_NULL);
    EDF_ASSERT((addr_bits == EDF_I2C_ADDR_BITS_7) || (addr_bits == EDF_I2C_ADDR_BITS_10));
    EDF_ASSERT((addr_bits != EDF_I2C_ADDR_BITS_7) || (addr >= 0 && addr <= 0x7f));
    EDF_ASSERT((addr_bits != EDF_I2C_ADDR_BITS_10) || (addr >= 0 && addr <= 0x3ff));
    /* Initialize the fields */
    i2c_dev->config = default_config;
    edf_ringbuf_init(&i2c_dev->rd_fifo, EDF_NULL, 0);
#ifndef EDF_CFG_I2C_RD_BUFSZ
#define EDF_CFG_I2C_RD_BUFSZ             (0)
#endif /* EDF_CFG_I2C_RD_BUFSZ */
    i2c_dev->rd_bufsz = EDF_CFG_I2C_RD_BUFSZ;
    i2c_dev->addr = addr;
    i2c_dev->addr_bits = addr_bits;

    /* Register the i2c-device */
    return edf_dev_register(&i2c_dev->dev, path, EDF_DEV_TYPE_I2C, EDF_O_RDWR, &ops, EDF_NULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static int i2c_bus_open(edf_device_t *dev)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)dev;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)dev->drv->ops;

    /* Reset the hold */
    i2c_bus->hold = EDF_FALSE;

    /* Default address is 0x00 and 7-bit */
    return ops->configure(i2c_bus, &i2c_bus->config, 0x00, EDF_I2C_ADDR_BITS_7);
}

static int i2c_bus_close(edf_device_t *dev)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)dev;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)dev->drv->ops;
    edf_i2c_config_t close_config = {0};

    return ops->configure(i2c_bus, &close_config, 0x00, EDF_I2C_ADDR_BITS_7);
}

static ssize_t i2c_bus_read(edf_device_t *dev, void *buf, size_t count)
{
    return EDF_ENOTSUP;
}

static ssize_t i2c_bus_write(edf_device_t *dev, const void *buf, size_t count)
{
    return EDF_ENOTSUP;
}

static ssize_t i2c_bus_isr(edf_device_t *dev, int event, void *args)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)dev;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)dev->drv->ops;

    switch (event) {
        case EDF_ISR_I2C_RD_INT: {
            edf_i2c_dev_t *i2c_dev = (edf_i2c_dev_t *)i2c_bus->owner;
            uint8_t data;

            /* Read data to FIFO */
            int ret = ops->read(i2c_bus, &data, EDF_ENABLE);
            if (ret < 0) {
                return ret;
            }
            edf_ringbuf_push_force(&i2c_dev->rd_fifo, data);
            /* Call the i2c-dev ISR */
            return edf_dev_isr(&i2c_dev->dev, event, EDF_NULL);
        }
        default: {
            return EDF_ENOTSUP;
        }
    }
}



EDF_INLINE int i2c_dev_take_bus(edf_i2c_dev_t *i2c_dev)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)i2c_dev->dev.parent;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)i2c_bus->dev.drv->ops;

    /* Check if the bus is busy */
    if ((i2c_bus->hold == EDF_TRUE) && (i2c_dev != i2c_bus->owner)) {
        return EDF_EBUSY;
    }

    /* If the owner changes, recheck the configuration */
    if (i2c_dev != i2c_bus->owner) {
        if (i2c_dev->config.baud_rate != i2c_bus->config.baud_rate ||
            i2c_dev->config.host_slave != i2c_bus->config.host_slave) {
            int addr = (i2c_dev->config.host_slave == EDF_I2C_HOST) ? 0x00 : i2c_dev->addr;

            int ret = ops->configure(i2c_bus, &i2c_dev->config, addr, i2c_dev->addr_bits);
            if (ret < 0) {
                return ret;
            }
        }
        i2c_bus->config = i2c_dev->config;
        i2c_bus->owner = i2c_dev;
    }
    i2c_bus->hold = EDF_TRUE;
    return EDF_EOK;
}

EDF_INLINE int i2c_dev_release_bus(edf_i2c_dev_t *i2c_dev)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)i2c_dev->dev.parent;

    if (i2c_dev != i2c_bus->owner) {
        return EDF_EINVAL;
    }

    /* If it is a host, release the bus. The slave needs to hold the bus at all times */
    if (i2c_dev->config.host_slave == EDF_I2C_HOST) {
        i2c_bus->hold = EDF_FALSE;
    }
    return EDF_EOK;
}

#define EDF_I2C_RD                       (0)
#define EDF_I2C_WR                       (1)

EDF_INLINE int i2c_dev_send_addr(edf_i2c_dev_t *i2c_dev, int rdwr)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)i2c_dev->dev.parent;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)i2c_bus->dev.drv->ops;
    int addr, addr_bits;

    /* Get the address, otherwise use the 0x00 */
    addr = i2c_dev->addr;
    addr_bits = i2c_dev->addr_bits;

    /* Set the read command */
    addr = (0xf000 | ((addr >> 8) & 0x03) << 9) | (addr & 0xff);
    if (rdwr == EDF_I2C_RD) {
        addr |= (addr_bits == EDF_I2C_ADDR_BITS_7) ? 0x01 : 0x10;
    }

    ops->start(i2c_bus);
    int ret = ops->send_addr(i2c_bus, addr, addr_bits);
    if (ret < 0) {
        ops->stop(i2c_bus);
    }
    return ret;
}

EDF_INLINE void i2c_dev_send_stop(edf_i2c_dev_t *i2c_dev)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)i2c_dev->dev.parent;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)i2c_bus->dev.drv->ops;

    ops->stop(i2c_bus);
}

EDF_INLINE ssize_t i2c_dev_read_handler(edf_i2c_dev_t *i2c_dev, uint8_t *buf, size_t count)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)i2c_dev->dev.parent;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)i2c_bus->dev.drv->ops;
    ssize_t rd_size;

    for (rd_size = 0; rd_size < count; rd_size += sizeof(*buf)) {
        int ack = ((count - rd_size) != sizeof(*buf));

        int ret = ops->read(i2c_bus, buf, ack);
        if (ret < 0) {
            return (rd_size == 0) ? ret : rd_size;
        }
        buf++;
    }
    return rd_size;
}

EDF_INLINE ssize_t i2c_dev_write_handler(edf_i2c_dev_t *i2c_dev, const uint8_t *buf, size_t count)
{
    edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)i2c_dev->dev.parent;
    edf_i2c_bus_ops_t *ops = (edf_i2c_bus_ops_t *)i2c_bus->dev.drv->ops;
    ssize_t wr_size;

    for (wr_size = 0; wr_size < count; wr_size += sizeof(*buf)) {
        int ret = ops->write(i2c_bus, *buf);
        if (ret < 0) {
            return (wr_size == 0) ? ret : wr_size;
        }
        buf++;
    }
    return wr_size;
}

static int i2c_dev_open(edf_device_t *dev)
{
    edf_i2c_dev_t *i2c_dev = (edf_i2c_dev_t *)dev;

    return edf_ringbuf_allocate(&i2c_dev->rd_fifo, i2c_dev->rd_bufsz);
}

static int i2c_dev_close(edf_device_t *dev)
{
    edf_i2c_dev_t *i2c_dev = (edf_i2c_dev_t *)dev;

    edf_ringbuf_free(&i2c_dev->rd_fifo);
    return EDF_EOK;
}

static ssize_t i2c_dev_read(edf_device_t *dev, void *buf, size_t count)
{
    edf_i2c_dev_t *i2c_dev = (edf_i2c_dev_t *)dev;

    ssize_t ret = i2c_dev_take_bus(i2c_dev);
    if (ret < 0) {
        return ret;
    }

    if (i2c_dev->config.host_slave == EDF_I2C_HOST) {
        /* Send the address of the register that needs to be read */
        if (dev->position >= 0) {
            ret = i2c_dev_send_addr(i2c_dev, EDF_I2C_WR);
            if (ret < 0) {
                goto release_bus;
            }

            ret = i2c_dev_write_handler(i2c_dev,
                                (uint8_t *)&dev->position,
                                (i2c_dev->config.reg_bits >> 3));
            if (ret < 0) {
                goto release_bus;
            }
        }

        ret = i2c_dev_send_addr(i2c_dev, EDF_I2C_RD);
        if (ret < 0) {
            goto release_bus;
        }
        ret = i2c_dev_read_handler(i2c_dev, (uint8_t *)buf, count);
        i2c_dev_send_stop(i2c_dev);
    } else {
        ret = (ssize_t)edf_ringbuf_read(&i2c_dev->rd_fifo, buf, count);
    }

    release_bus:
    i2c_dev_release_bus(i2c_dev);
    return ret;
}

static ssize_t i2c_dev_write(edf_device_t *dev, const void *buf, size_t count)
{
    edf_i2c_dev_t *i2c_dev = (edf_i2c_dev_t *)dev;

    ssize_t ret = i2c_dev_take_bus(i2c_dev);
    if (ret < 0) {
        return ret;
    }

    if (i2c_dev->config.host_slave == EDF_I2C_HOST) {
        ret = i2c_dev_send_addr(i2c_dev, EDF_I2C_WR);
        if (ret < 0) {
            goto release_bus;
        }

        /* Send the address of the register that needs to be written */
        if (dev->position >= 0) {
            ret = i2c_dev_write_handler(i2c_dev,
                                (uint8_t *)&dev->position,
                                (i2c_dev->config.reg_bits >> 3));
            if (ret < 0) {
                goto release_bus;
            }
        }

        ret = i2c_dev_write_handler(i2c_dev, (uint8_t *)buf, count);
        i2c_dev_send_stop(i2c_dev);
    } else {
        ret = i2c_dev_write_handler(i2c_dev, (uint8_t *)buf, count);
    }

    release_bus:
    i2c_dev_release_bus(i2c_dev);
    return ret;
}

static int i2c_dev_ioctl(edf_device_t *dev, int cmd, void *args)
{
    edf_i2c_dev_t *i2c_dev = (edf_i2c_dev_t *)dev;

    switch (cmd) {
        case EDF_IOC_I2C_SET_CONFIG: {
            if (args != EDF_NULL) {
                edf_i2c_bus_t *i2c_bus = (edf_i2c_bus_t *)dev->parent;
                edf_i2c_config_t config = *(edf_i2c_config_t *)args;

                /* If holding the bus, release it */
                if (i2c_dev == i2c_bus->owner) {
                    i2c_bus->hold = EDF_FALSE;
                    i2c_bus->owner = EDF_NULL;
                }

                /* Update the configuration and try again to get the bus */
                i2c_dev->config = config;
                if (config.host_slave == EDF_I2C_SLAVE) {
                    int ret = i2c_dev_take_bus(i2c_dev);
                    if (ret < 0) {
                        return ret;
                    }
                }
                return sizeof(config);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_I2C_SET_RD_BUFSZ: {
            if (args != EDF_NULL) {
                size_t bufsz = *(size_t *)args;

                int ret = edf_ringbuf_allocate(&i2c_dev->rd_fifo, bufsz);
                i2c_dev->rd_bufsz = 0;
                if (ret < 0) {
                    return ret;
                }
                i2c_dev->rd_bufsz = bufsz;
                return sizeof(bufsz);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_I2C_CLR_RD_BUF: {
            edf_ringbuf_reset(&i2c_dev->rd_fifo);
            return EDF_EOK;
        }
        case EDF_IOC_I2C_GET_CONFIG: {
            if (args != EDF_NULL) {
                struct edf_i2c_config *config = (struct edf_i2c_config *)args;

                *config = i2c_dev->config;
                return sizeof(*config);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_I2C_GET_RD_BUFSZ: {
            if (args != EDF_NULL) {
                size_t *bufsz = (size_t *)args;

                *bufsz = i2c_dev->rd_bufsz;
                return sizeof(*bufsz);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_I2C_GET_RD_DATASZ: {
            if (args != EDF_NULL) {
                size_t *datasz = (size_t *)args;

                *datasz = edf_ringbuf_get_bufsz(&i2c_dev->rd_fifo);
                return sizeof(*datasz);
            }
            return EDF_EINVAL;
        }
        default: {
            return EDF_ENOTSUP;
        }
    }
}

/******************************* (END OF FILE) *********************************/



 







