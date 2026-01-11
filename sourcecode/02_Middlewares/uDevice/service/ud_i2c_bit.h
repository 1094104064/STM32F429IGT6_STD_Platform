/**
  ******************************************************************************
  *
  * @file    ud_i2c_bit.h
  * @author  
  * @brief   Header file of i2c bit-banging module.
  *
  ******************************************************************************
  **/
  
#ifndef _UD_I2C_BIT_H
#define _UD_I2C_BIT_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
/*********************
 *      DEFINES
 *********************/
struct ud_i2c_bit_ops
{
    void (* set_scl)(void *data, uint8_t level);
    void (* set_sda)(void *data, uint8_t level);
    uint32_t (* get_scl)(void *data);
    uint32_t (* get_sda)(void *data);
    void (* delay_us)(uint32_t us);
    uint32_t timeout_ms;
    void * data;
};

struct ud_i2c_bit
{
    const struct ud_i2c_bit_ops * ops;
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/



/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /* _UD_I2C_BIT_H */


/******************************* (END OF FILE) *********************************/


