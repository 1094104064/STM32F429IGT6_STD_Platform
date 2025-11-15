/**
  ******************************************************************************
  *
  * @file    edf_soft_i2c.c
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
#include "edf_soft_i2c.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define SOFT_I2C_LOW                    0
#define SOFT_I2C_HIGH                   1
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int soft_i2c_bus_wait_ack(edf_i2c_bus_t *i2c_bus);
static void soft_i2c_bus_send_ack(edf_i2c_bus_t *i2c_bus, int ack);
static int soft_i2c_bus_configure(edf_i2c_bus_t *i2c_bus,
                                  edf_i2c_config_t *config,
                                  int addr,
                                  int addr_bits);
static void soft_i2c_bus_start(edf_i2c_bus_t *i2c_bus);
static int soft_i2c_bus_send_addr(edf_i2c_bus_t *i2c_bus, int addr, int addr_bits);
static void soft_i2c_bus_stop(edf_i2c_bus_t *i2c_bus);
static int soft_i2c_bus_read(edf_i2c_bus_t *i2c_bus, uint8_t *data, int ack_state);
static int soft_i2c_bus_write(edf_i2c_bus_t *i2c_bus, uint8_t data);                                  
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**
 * @brief This function registers a soft-i2c-bus.
 *
 * @param soft_i2c_bus The soft-i2c-bus.
 * @param path The path of the soft-i2c-bus.
 * @param scl_pin The scl pin of the soft-i2c-bus.
 * @param sda_pin The sda pin of the soft-i2c-bus.
 *
 * @return 0 on success, otherwise an error code.
 */
int edf_soft_i2c_bus_register(edf_soft_i2c_bus_t *soft_i2c_bus,
                              const char *path,
                              int scl_pin,
                              int sda_pin)
{
    static edf_i2c_bus_ops_t ops = {soft_i2c_bus_configure,
                                    soft_i2c_bus_start,
                                    soft_i2c_bus_send_addr,
                                    soft_i2c_bus_stop,
                                    soft_i2c_bus_read,
                                    soft_i2c_bus_write};
    static edf_drv_t drv = {&ops, EDF_NULL};

    EDF_ASSERT(soft_i2c_bus != EDF_NULL);
    EDF_ASSERT(path != EDF_NULL);

    /* Initialize the fields */
    soft_i2c_bus->delay = 0;
    soft_i2c_bus->scl_pin = scl_pin;
    soft_i2c_bus->sda_pin = sda_pin;

    /* Register the soft-i2c-bus */
    return edf_i2c_bus_register(&soft_i2c_bus->i2c_bus, path, &drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
EDF_INLINE void soft_i2c_scl_set(edf_soft_i2c_bus_t *soft_i2c_bus, uint8_t value)
{
    _edf_fast_pin_write(soft_i2c_bus->scl_pin, value);
}

EDF_INLINE void soft_i2c_bus_sda_set(edf_soft_i2c_bus_t *soft_i2c_bus, uint8_t value)
{
    _edf_fast_pin_write(soft_i2c_bus->sda_pin, value);
}

EDF_INLINE uint8_t soft_i2c_sda_get(edf_soft_i2c_bus_t *soft_i2c_bus)
{
    return (uint8_t)_edf_fast_pin_read(soft_i2c_bus->sda_pin);
}

static int soft_i2c_bus_wait_ack(edf_i2c_bus_t *i2c_bus)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;
    int ret = EDF_ETIMEOUT;

    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_HIGH);
    edf_delay_us(soft_i2c_bus->delay);

    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_HIGH);
    edf_delay_us(soft_i2c_bus->delay);

    if (soft_i2c_sda_get(soft_i2c_bus) == SOFT_I2C_LOW) {
        ret = EDF_EOK;
    }
    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    edf_delay_us(soft_i2c_bus->delay);
    return ret;
}

static void soft_i2c_bus_send_ack(edf_i2c_bus_t *i2c_bus, int ack)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;

    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    if (ack == EDF_ENABLE) {
        soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_LOW);
    } else {
        soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_HIGH);
    }

    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_HIGH);
    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_HIGH);
}

static int soft_i2c_bus_configure(edf_i2c_bus_t *i2c_bus,
                                  edf_i2c_config_t *config,
                                  int addr,
                                  int addr_bits)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;
    int state = (config->baud_rate != 0) ? EDF_ENABLE : EDF_DISABLE;
    int mode = (state == EDF_ENABLE) ? EDF_PIN_MODE_OUTPUT_OD : EDF_PIN_MODE_NONE;

    if (state == EDF_ENABLE) {
        /* Soft I2C only support host mode */
        if (config->host_slave != EDF_I2C_HOST) {
            return EDF_ENOTSUP;
        }

        /* Calculate the delay time */
        soft_i2c_bus->delay = (1000000 / config->baud_rate) / 2;
    }

    /* Configure SCL and SDA */
    int ret = _edf_fast_pin_mode(soft_i2c_bus->scl_pin, mode);
    if (ret < 0) {
        return ret;
    }
    ret = _edf_fast_pin_mode(soft_i2c_bus->sda_pin, mode);
    if (ret < 0) {
        return ret;
    }
    return EDF_EOK;
}

static void soft_i2c_bus_start(edf_i2c_bus_t *i2c_bus)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;

    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_HIGH);
    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_HIGH);

    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_LOW);
    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
}

static int soft_i2c_bus_send_addr(edf_i2c_bus_t *i2c_bus, int addr, int addr_bits)
{
    if (addr_bits == EDF_I2C_ADDR_BITS_10) {
        int ret = soft_i2c_bus_write(i2c_bus, addr >> 8);
        if (ret < 0) {
            return ret;
        }
    }

    int ret = soft_i2c_bus_write(i2c_bus, addr);
    if (ret < 0) {
        return ret;
    }
    return EDF_EOK;
}

static void soft_i2c_bus_stop(edf_i2c_bus_t *i2c_bus)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;

    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_LOW);
    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);

    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_HIGH);
    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_HIGH);
    edf_delay_us(soft_i2c_bus->delay);
}

static int soft_i2c_bus_read(edf_i2c_bus_t *i2c_bus, uint8_t *data, int ack_state)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;

    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_bus_sda_set(soft_i2c_bus, SOFT_I2C_HIGH);

    for (size_t bits = 0; bits < (sizeof(*data) * 8); bits++) {
        soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
        edf_delay_us(soft_i2c_bus->delay);
        soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_HIGH);
        edf_delay_us(soft_i2c_bus->delay);
        *data <<= 1;
        if (soft_i2c_sda_get(soft_i2c_bus) == SOFT_I2C_HIGH) {
            *data |= 0x01;
        }
    }

    soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    edf_delay_us(soft_i2c_bus->delay);
    soft_i2c_bus_send_ack(i2c_bus, ack_state);
    return EDF_EOK;
}

static int soft_i2c_bus_write(edf_i2c_bus_t *i2c_bus, uint8_t data)
{
    edf_soft_i2c_bus_t *soft_i2c_bus = (edf_soft_i2c_bus_t *)i2c_bus;

    for (size_t bits = 0; bits < 8; bits++) {
        soft_i2c_bus_sda_set(soft_i2c_bus, (data & 0x80) ? SOFT_I2C_HIGH : SOFT_I2C_LOW);
        data <<= 1;

        edf_delay_us(soft_i2c_bus->delay);
        soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_HIGH);
        edf_delay_us(soft_i2c_bus->delay);
        soft_i2c_scl_set(soft_i2c_bus, SOFT_I2C_LOW);
    }
    return soft_i2c_bus_wait_ack(i2c_bus);
}



/******************************* (END OF FILE) *********************************/



 







