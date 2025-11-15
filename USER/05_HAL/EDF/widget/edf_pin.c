/**
  ******************************************************************************
  *
  * @file    edf_pin.c
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
#include "edf_pin.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

#define PIN_MODE_SET(_pin, _number, _mode)                                              \
    do                                                                                  \
    {                                                                                   \
        EDF_BIT_CLR((_pin)->pins[(_number) / 8], (0xf << (((_number) % 8) * 4)));        \
        EDF_BIT_SET((_pin)->pins[(_number) / 8], ((_mode) << (((_number) % 8) * 4)));    \
    } while (0)
#define PIN_MODE_GET(_pin, _number)                                                     \
        ((int)(((_pin)->pins[(_number) / 8] >> (((_number) % 8) * 4)) & 0xf))           \
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
EDF_INLINE int pin_set_mode(edf_pin_t *pin, int number, edf_pin_config_t config);
EDF_INLINE int pin_get_mode(edf_pin_t *pin, int number, edf_pin_config_t *config);
static int pin_close(edf_device_t *dev);
static ssize_t pin_read(edf_device_t *dev, void *buf, size_t count);
static ssize_t pin_write(edf_device_t *dev, const void *buf, size_t count);
static int pin_ioctl(edf_device_t *dev, int cmd, void *args);
static ssize_t pin_isr(edf_device_t *dev, int event, void *args);
EDF_INLINE edf_device_t **_fast_pin_dev_get(uint32_t magic);
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
int edf_pin_register(edf_pin_t *pin, const char *path, edf_drv_t *drv)
{
    static edf_dev_ops_t ops = {EDF_NULL,
                                pin_close,
                                pin_read,
                                pin_write,
                                pin_ioctl,
                                pin_isr};
    EDF_ASSERT(pin != EDF_NULL);
    EDF_ASSERT(path != EDF_NULL);
    EDF_ASSERT(drv != EDF_NULL);
    EDF_ASSERT(drv->ops != EDF_NULL);

    /* Register the pin */
    int ret = edf_dev_register(&pin->dev, path, EDF_DEV_TYPE_PIN, EDF_O_RDWR, &ops, drv);
    if (ret == EDF_EOK) {
        /* Initialize the fast pin */
        _edf_fast_pin_init(&pin->dev);
    }
    return ret;
}

/**
 * @brief This function initialize the fast pin device.
 *
 * @param dev The pin device.
 *
 * @note Please do not appear the code in this file in the application layer.
 */
void _edf_fast_pin_init(edf_device_t *dev)
{
    if (_fast_pin_dev_get(dev->magic) != EDF_NULL) {
        *_fast_pin_dev_get(dev->magic) = dev;
    }
}

/**
 * @brief This function set the pin mode.
 *
 * @param number The pin number.
 * @param mode The pin mode.
 *
 * @return 0 on success, otherwise an error code.
 *
 * @note Please do not appear the code in this file in the application layer.
 */
int _edf_fast_pin_mode(int number, int mode)
{
    edf_device_t *dev = *_fast_pin_dev_get(EDF_MAGIC_NUMBER);

    if (dev == EDF_NULL) {
        return EDF_ENOTFOUND;
    }
    return ((edf_pin_ops_t *)dev->drv->ops)->configure((edf_pin_t *)dev, number, mode);
}

/**
 * @brief This function read the pin value.
 *
 * @param number The pin number.
 *
 * @return The pin value.
 *
 * @note Please do not appear the code in this file in the application layer.
 */
uint8_t _edf_fast_pin_read(int number)
{
    edf_device_t *dev = *_fast_pin_dev_get(EDF_MAGIC_NUMBER);
    uint8_t value = 0;

    ((edf_pin_ops_t *)dev->drv->ops)->read((edf_pin_t *)dev, number, &value);
    return value;
}

/**
 * @brief This function write the pin value.
 *
 * @param number The pin number.
 * @param value The pin value.
 *
 * @note Please do not appear the code in this file in the application layer.
 */
void _edf_fast_pin_write(int number, int value)
{
    edf_device_t *dev = *_fast_pin_dev_get(EDF_MAGIC_NUMBER);

    ((edf_pin_ops_t *)dev->drv->ops)->write((edf_pin_t *)dev, number, value);
}
/** @} */

/**********************
 *   STATIC FUNCTIONS
 **********************/
EDF_INLINE int pin_set_mode(edf_pin_t *pin, int number, edf_pin_config_t config)
{
    edf_pin_ops_t *ops = (edf_pin_ops_t *)pin->dev.drv->ops;

    if ((number < 0) || (number >= (sizeof(pin->pins) * 2))) {
        return EDF_EINVAL;
    }

    int ret = ops->configure(pin, number, config.mode);
    if (ret < 0) {
        return ret;
    }

    PIN_MODE_SET(pin, number, config.mode);
    return EDF_EOK;
}

EDF_INLINE int pin_get_mode(edf_pin_t *pin, int number, edf_pin_config_t *config)
{
    if ((number < 0) || (number >= (sizeof(pin->pins) * 2))) {
        return EDF_EINVAL;
    }

    config->mode = PIN_MODE_GET(pin, number);
    return EDF_EOK;
}

static int pin_close(edf_device_t *dev)
{
    edf_pin_t *pin = (edf_pin_t *)dev;
    edf_pin_ops_t *ops = (edf_pin_ops_t *)dev->drv->ops;

#ifdef EDF_USING_PIN_AUTO_DISABLE
    /* Disable all pins */
    for (size_t i = 0; i < (sizeof(pin->pins) * 2); i++)
    {
        if (PIN_MODE_GET(pin, i) != EDF_PIN_MODE_NONE)
        {
            ops->configure(pin, (int)i, EDF_PIN_MODE_NONE);
            PIN_MODE_SET(pin, i, EDF_PIN_MODE_NONE);
        }
    }
#endif /* EDF_USING_PIN_AUTO_DISABLE */
    return EDF_EOK;
}

static ssize_t pin_read(edf_device_t *dev, void *buf, size_t count)
{
    edf_pin_t *pin = (edf_pin_t *)dev;
    edf_pin_ops_t *ops = (edf_pin_ops_t *)dev->drv->ops;
    uint8_t *rd_buf = (uint8_t *)buf;
    ssize_t rd_size;

#ifdef EDF_USING_PIN_CHECK
    /* Check number is valid */
    if ((dev->position < 0) ||
        (dev->position >= (sizeof(pin->pins) * 2)) ||
        (PIN_MODE_GET(pin, dev->position) == EDF_PIN_MODE_NONE)) {
        return EDF_EINVAL;
    }
#endif /* EDF_USING_PIN_CHECK */

    for (rd_size = 0; rd_size < count; rd_size += sizeof(*rd_buf)) {
        int ret = (uint8_t)ops->read(pin, dev->position, rd_buf);
        if (ret < 0) {
            return (rd_size == 0) ? ret : rd_size;
        }
        rd_buf++;
    }
    return rd_size;
}

static ssize_t pin_write(edf_device_t *dev, const void *buf, size_t count)
{
    edf_pin_t *pin = (edf_pin_t *)dev;
    edf_pin_ops_t *ops = (edf_pin_ops_t *)dev->drv->ops;
    uint8_t *wr_buf = (uint8_t *)buf;
    ssize_t wr_size;

#ifdef EDF_USING_PIN_CHECK
    /* Check number is valid */
    if ((dev->position < 0) ||
        (dev->position >= (sizeof(pin->pins) * 2)) ||
        (PIN_MODE_GET(pin, dev->position) == EDF_PIN_MODE_NONE)) {
        return EDF_EINVAL;
    }
#endif /* EDF_USING_PIN_CHECK */

    for (wr_size = 0; wr_size < count; wr_size += sizeof(*wr_buf)) {
        int ret = ops->write(pin, dev->position, *wr_buf);
        if (ret < 0) {
            return (wr_size == 0) ? ret : wr_size;
        }
        wr_buf++;
    }
    return wr_size;
}

static int pin_ioctl(edf_device_t *dev, int cmd, void *args)
{
    edf_pin_t *pin = (edf_pin_t *)dev;

    switch (cmd) {
        case EDF_IOC_PIN_SET_MODE: {
            if (args != EDF_NULL) {
                edf_pin_config_t config = *((edf_pin_config_t *)args);

                int ret = pin_set_mode(pin, dev->position, config);
                if (ret < 0) {
                    return ret;
                }
                return sizeof(config);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_PIN_GET_MODE: {
            if (args != EDF_NULL) {
                edf_pin_config_t *config = (edf_pin_config_t *)args;
                int ret = pin_get_mode(pin, dev->position, config);
                if (ret < 0) {
                    return ret;
                }
                return sizeof(*config);
            }
        }
        default: {
            return EDF_ENOTSUP;
        }
    }
}

static ssize_t pin_isr(edf_device_t *dev, int event, void *args)
{
    switch (event) {
        case EDF_ISR_PIN_EXTI_INT: {
            ssize_t number = *(int *)args;
            return number;
        }
        default: {
            return EDF_ENOTSUP;
        }
    }
}

/**
 * @brief This function get the fast pin device pointer.
 *
 * @param magic The magic number.
 *
 * @return The fast pin device pointer.
 *
 * @note Please do not appear the code in this file in the application layer.
 */
EDF_INLINE edf_device_t **_fast_pin_dev_get(uint32_t magic)
{
    static edf_device_t *dev = EDF_NULL;

    /* If that doesn't stop you, feel free to use it */
    if (magic == EDF_MAGIC_NUMBER) {
        return &dev;
    }
    return EDF_NULL;
}

/******************************* (END OF FILE) *********************************/



 







