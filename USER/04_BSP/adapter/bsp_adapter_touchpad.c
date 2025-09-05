/**
  ******************************************************************************
  *
  * @file    bsp_adapter_touchpad.c
  * @author  Jamin
  * @brief   Touchpad adapter implementation
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
#include "bsp_adapter_touchpad.h"
#include "bsp_wrapper_touchpad.h"
#include "bsp_driver_gt911.h"
#include "bsp_port_gt911.h"
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
static int bsp_adapter_touchpad_init(struct touchpad_wrapper * self);
static void bsp_adapter_touchpad_reset(struct touchpad_wrapper * self);
static void bsp_adapter_touchpad_scan(struct touchpad_wrapper * self);
static uint8_t bsp_adapter_touchpad_is_pressed(struct touchpad_wrapper * self);
static void bsp_adapter_touchpad_get_coordinates(struct touchpad_wrapper * self, uint16_t * x, uint16_t * y, uint8_t num);
/**********************
 *  STATIC VARIABLES
 **********************/
static struct gt911_driver gt911_drv;
static struct gt911_oper_i2c gt911_i2c = {
    .pf_read_reg    = bsp_port_gt911_read_reg,
    .pf_write_reg   = bsp_port_gt911_write_reg,
};
static struct gt911_oper_ctrl gt911_ctrl = {
    .pf_int_in      = bsp_port_gt911_int_in,
    .pf_int_out     = bsp_port_gt911_int_out,
    .pf_int_high    = bsp_port_gt911_int_high,
    .pf_int_low     = bsp_port_gt911_int_low,
    .pf_rst_high    = bsp_port_gt911_rst_high,
    .pf_rst_low     = bsp_port_gt911_rst_low,
};
static struct gt911_oper gt911_ops = {
    .pf_delay_ms    = bsp_port_gt911_delay_ms,
};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_touchpad_register(void)
{
    struct touchpad_wrapper wrapper = {
        .idx        = 0,
        .user_data  = NULL,

        .pf_init               = bsp_adapter_touchpad_init,
        .pf_reset              = bsp_adapter_touchpad_reset,
        .pf_is_pressed         = bsp_adapter_touchpad_is_pressed,
        .pf_scan               = bsp_adapter_touchpad_scan,
        .pf_get_coordinates    = bsp_adapter_touchpad_get_coordinates,
    };

    bsp_wrapper_touchpad_link(&wrapper);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static int bsp_adapter_touchpad_init(struct touchpad_wrapper * self)
{
    bsp_driver_gt911_link(&gt911_drv, &gt911_ops, &gt911_i2c, &gt911_ctrl);

    if( gt911_drv.pf_init               == NULL || gt911_drv.pf_reset       == NULL ||
        gt911_drv.pf_scan               == NULL || gt911_drv.pf_is_pressed  == NULL || 
        gt911_drv.pf_get_coordinates    == NULL) {
        return 1;
    }

    if(gt911_drv.pf_init(&gt911_drv) == false) {
        return 2;
    }

    return 0;
}


static void bsp_adapter_touchpad_reset(struct touchpad_wrapper * self)
{
    gt911_drv.pf_reset(&gt911_drv);
}

static void bsp_adapter_touchpad_scan(struct touchpad_wrapper * self)
{
    gt911_drv.pf_scan(&gt911_drv);
}

static uint8_t bsp_adapter_touchpad_is_pressed(struct touchpad_wrapper * self)
{
    return gt911_drv.pf_is_pressed(&gt911_drv);
}



static void bsp_adapter_touchpad_get_coordinates(struct touchpad_wrapper * self, uint16_t * x, uint16_t * y, uint8_t num)
{
    gt911_drv.pf_get_coordinates(&gt911_drv, x, y, num);
}

/******************************* (END OF FILE) *********************************/

