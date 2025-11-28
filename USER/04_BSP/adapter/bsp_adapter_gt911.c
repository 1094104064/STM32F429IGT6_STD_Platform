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
#include "bsp_wrapper_touchpad.h"
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
static int ops_gt911_init(void)
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

    if (gs_gt911_drv.pf_init(&gs_gt911_drv) == false) {
        return 2;
    }
    
    return 0;
}

static bool ops_gt911_is_pressed(void)
{
    return gs_gt911_drv.pf_is_pressed(&gs_gt911_drv);
}

static void ops_gt911_control(int cmd, void * arg)
{
    // Implement control commands if needed
}

static void ops_gt911_get_xy(uint16_t * x, uint16_t * y, uint8_t read_num)
{
    gs_gt911_drv.pf_get_coordinates(&gs_gt911_drv, x, y, read_num);
}

static const touchpad_ops_t gt911_ops = {
    .pf_init           = ops_gt911_init,
    .pf_is_pressed     = ops_gt911_is_pressed,
    .pf_control        = ops_gt911_control,
    .pf_get_xy         = ops_gt911_get_xy,
};

void Gt911Register(void)
{
    touchpad_obj_t * gt911_obj = NULL;

    gt911_obj = wrp_touchpad.obj_create(&gt911_ops, "gt911", NULL);
    wrp_touchpad.init(gt911_obj);
}

/******************************* (END OF FILE) *********************************/



 







