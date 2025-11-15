/**
  ******************************************************************************
  *
  * @file    bsp_adapter_gt911.c
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
#include "bsp_adapter_gt911.h"
#include "bsp_driver_gt911.h"
#include "bsp_linker_gt911.h"
#include "delay.h"
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

/**********************
 *  STATIC VARIABLES
 **********************/
static gt911_driver_t gs_gt911_drv;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void ops_gt911_init(void)
{
    static const gt911_ctrl_t ctrl = {
        .pf_set_int_pin     = bsp_linker_gt911_set_int_pin,
        .pf_write_int_pin   = bsp_linker_gt911_write_int_pin,
        .pf_write_rst_pin   = bsp_linker_gt911_write_rst_pin,
    };

    static const gt911_timebase_t timebase = {
        .pf_delay_ms = delay_ms,
    };

    static const gt911_i2c_t i2c = {
        .type = GT911_I2C_SOFTWARE,
        .connect.si2c = {
            .pf_init            = bsp_linker_gt911_si2c_init,
            .pf_start           = bsp_linker_gt911_si2c_start,
            .pf_stop            = bsp_linker_gt911_si2c_stop,
            .pf_wait_ack        = bsp_linker_gt911_si2c_wait_ack,
            .pf_generate_ack    = bsp_linker_gt911_si2c_generate_ack,
            .pf_generate_nack   = bsp_linker_gt911_si2c_generate_nack,
            .pf_read_byte       = bsp_linker_gt911_si2c_read_byte,
            .pf_write_byte      = bsp_linker_gt911_si2c_write_byte,
        },
    };

    static const gt911_handle_t handle = {
        .ctrl       = &ctrl,
        .timebase   = &timebase,
        .i2c        = &i2c,
    };

    bsp_driver_gt911_link(&gs_gt911_drv, &handle);

    
}



/******************************* (END OF FILE) *********************************/



 







