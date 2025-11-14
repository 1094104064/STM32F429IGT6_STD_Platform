/**
  ******************************************************************************
  *
  * @file    dev_pin.c
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
#include "dev_pin.h"
#include "dev_service.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define PIN_MODE_SET(_pin, _number, _mode)                                              \
    do                                                                                  \
    {                                                                                   \
        DEV_BIT_CLR((_pin)->pins[(_number) / 8], (0xf << (((_number) % 8) * 4)));        \
        DEV_BIT_SET((_pin)->pins[(_number) / 8], ((_mode) << (((_number) % 8) * 4)));    \
    } while (0)
#define PIN_MODE_GET(_pin, _number)                                                     \
        ((int)(((_pin)->pins[(_number) / 8] >> (((_number) % 8) * 4)) & 0xf))           \
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int pin_close(plaf_dev_t * dev);
static int pin_read(plaf_dev_t * dev, void * buf, size_t count);
static int pin_write(plaf_dev_t * dev, const void * buf, size_t count);
static int pin_ioctl(plaf_dev_t * dev, int cmd, void * args);
static int pin_isr(plaf_dev_t * dev, int event, void * args);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
 * @brief This function registers a pin.
 *
 * @param pin The pin.
 * @param path The path of the pin.
 * @param drv The driver of the pin.
 *
 * @return 0 on success, otherwise an error code.
 */
int dev_pin_register(dev_pin_t * pin, const char * path, plaf_dev_drv_t * drv)
{
    static plaf_dev_ops_t ops = {DEV_NULL,
                                 pin_close,
                                 pin_read,
                                 pin_write,
                                 pin_ioctl,
                                 pin_isr};

    DEV_ASSERT(pin != DEV_NULL);
    DEV_ASSERT(path != DEV_NULL);
    DEV_ASSERT(drv != DEV_NULL);
    DEV_ASSERT(drv->ops != DEV_NULL);

    /* Register the pin */
    int ret = platform_device_register(&pin->dev, path, DEV_TYPE_PIN, DEV_O_RDWR, &ops, drv);
    if (ret == DEV_EOK) {
        /* Initialize the fast pin */
        // _DEV_fast_pin_init(&pin->dev);
    }
    return ret;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static inline int pin_set_mode(dev_pin_t * pin, int number, dev_pin_conf_t config)
{
    dev_pin_ops_t * ops = (dev_pin_ops_t *)pin->dev.drv->ops;

    if ((number < 0) || (number >= (sizeof(pin->pins) * 2))) {
        return DEV_EINVAL;
    }

    int ret = ops->configure(pin, number, config.mode);
    if (ret < 0) {
        return ret;
    }

    PIN_MODE_SET(pin, number, config.mode);
    return DEV_EOK;
}

static inline int pin_get_mode(dev_pin_t * pin, int number, dev_pin_conf_t * config)
{
    if ((number < 0) || (number >= (sizeof(pin->pins) * 2))) {
        return DEV_EINVAL;
    }

    config->mode = PIN_MODE_GET(pin, number);
    return DEV_EOK;
}

static int pin_close(plaf_dev_t * dev)
{
#ifdef DEV_USING_PIN_AUTO_DISABLE
    dev_pin_t * pin = (dev_pin_t *)dev;
    dev_pin_ops_t * ops = (dev_pin_ops_t *)dev->drv->ops;

    /* Disable all pins */
    for (size_t i = 0; i < (sizeof(pin->pins) * 2); i++)
    {
        if (PIN_MODE_GET(pin, i) != _PIN_MODE_NONE)
        {
            ops->configure(pin, (int)i, DEV_PIN_MODE_NONE);
            PIN_MODE_SET(pin, i, DEV_PIN_MODE_NONE);
        }
    }
#endif /* DEV_USING_PIN_AUTO_DISABLE */
    return DEV_EOK;
}

static int pin_read(plaf_dev_t * dev, void * buf, size_t count)
{
    dev_pin_t * pin = (dev_pin_t *)dev;
    dev_pin_ops_t * ops = (dev_pin_ops_t *)dev->drv->ops;
    uint8_t * rd_buf = (uint8_t *)buf;
    int rd_size;

#ifdef DEV_USING_PIN_CHECK
    /* Check number is valid */
    if ((dev->position < 0) ||
        (dev->position >= (sizeof(pin->pins) * 2)) ||
        (PIN_MODE_GET(pin, dev->position) == DEV_PIN_MODE_NONE)) {
        return DEV_EINVAL;
    }
#endif /* DEV_USING_PIN_CHECK */

    for (rd_size = 0; rd_size < count; rd_size += sizeof(*rd_buf)) {
        int ret = (uint8_t)ops->read(pin, dev->position, rd_buf);
        if (ret < 0) {
            return (rd_size == 0) ? ret : rd_size;
        }
        rd_buf++;
    }
    return rd_size;
}

static int pin_write(plaf_dev_t * dev, const void * buf, size_t count)
{
    dev_pin_t * pin = (dev_pin_t *)dev;
    dev_pin_ops_t * ops = (dev_pin_ops_t *)dev->drv->ops;
    uint8_t * wr_buf = (uint8_t *)buf;
    int wr_size;

#ifdef DEV_USING_PIN_CHECK
    /* Check number is valid */
    if ((dev->position < 0) ||
        (dev->position >= (sizeof(pin->pins) * 2)) ||
        (PIN_MODE_GET(pin, dev->position) == DEV_PIN_MODE_NONE)) {
        return DEV_EINVAL;
    }
#endif /* DEV_USING_PIN_CHECK */

    for (wr_size = 0; wr_size < count; wr_size += sizeof(*wr_buf)) {
        int ret = ops->write(pin, dev->position, *wr_buf);
        if (ret < 0) {
            return (wr_size == 0) ? ret : wr_size;
        }
        wr_buf++;
    }
    return wr_size;
}

static int pin_ioctl(plaf_dev_t * dev, int cmd, void * args)
{
    dev_pin_t * pin = (dev_pin_t *)dev;

    switch (cmd) {
        case DEV_IOC_PIN_SET_MODE: {
            if (args != DEV_NULL) {
                dev_pin_conf_t config = *((dev_pin_conf_t *)args);

                int ret = pin_set_mode(pin, dev->position, config);
                if (ret < 0) {
                    return ret;
                }
                return sizeof(config);
            }
            return DEV_EINVAL;
        }
        case DEV_IOC_PIN_GET_MODE: {
            if (args != DEV_NULL) {
                dev_pin_conf_t * config = (dev_pin_conf_t *)args;

                int ret = pin_get_mode(pin, dev->position, config);
                if (ret < 0) {
                    return ret;
                }
                return sizeof(*config);
            }
        }
        default: {
            return DEV_ENOTSUP;
        }
    }
}

static int pin_isr(plaf_dev_t * dev, int event, void * args)
{
    switch (event) {
        case DEV_ISR_PIN_EXTI_INT: {
            int number = *(int *)args;
            return number;
        }
        default: {
            return DEV_ENOTSUP;
        }
    }
}




/******************************* (END OF FILE) *********************************/







