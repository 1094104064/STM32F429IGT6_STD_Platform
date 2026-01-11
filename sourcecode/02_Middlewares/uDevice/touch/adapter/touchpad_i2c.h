/**
  ******************************************************************************
  *
  * @file    touchpad_i2c.h
  * @author  
  * @brief   Header file of touchpad_i2c module.
  *
  ******************************************************************************
  **/

#ifndef _TOUCHPAD_I2C_H
#define _TOUCHPAD_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define TOUCHPAD_I2C_WR                0x0000
#define TOUCHPAD_I2C_RD               (1u << 0)

struct touchpad_i2c_msg
{
    uint16_t addr;
    uint16_t flags;
    uint16_t len;
    uint8_t  *buf;
};

struct touchpad_i2c_ops
{
    bool (* transfer)     (const struct touchpad_i2c_ops ** ops, struct touchpad_i2c_msg * msgs, size_t num);
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void touchpad_i2c_start(const struct touchpad_i2c_ops ** ops)
{
    // Implementation for starting I2C communication
}

static inline void touchpad_i2c_stop(const struct touchpad_i2c_ops ** ops)
{
    // Implementation for stopping I2C communication
}

static inline bool touchpad_i2c_transfer(const struct touchpad_i2c_ops ** ops, struct touchpad_i2c_msg * msgs, size_t num)
{
    return (*ops)->transfer(ops, msgs, num);
}

/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_TOUCHPAD_I2C_H*/


/******************************* (END OF FILE) *********************************/


