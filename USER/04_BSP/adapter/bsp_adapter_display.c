/**
  ******************************************************************************
  *
  * @file    bsp_adapter_display.c
  * @author  Jamin
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
#include "bsp_adapter_display.h"
#include "bsp_wrapper_display.h"
#include "bsp_driver_fb.h"
#include "bsp_driver_st7735.h"
#include "bsp_port_fb.h"
#include "bsp_port_st7735.h"
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
static fb_driver_t fb0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_display_register(void)
{
    static const display_ops_t fb_ops = {
        0
    };


    static const display_ops_t st7735_ops = {
        0
    };
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int fb0_init(void)
{
//    static const fb_handle_t fb0_handle = {

//        .active_width       = FB_DEFAULT_WIDTH,
//        .active_height      = FB_DEFAULT_HEIGHT,

//        .hsync_width        = FB_DEFAULT_HSW,
//        .hback_porch        = FB_DEFAULT_HBP,
//        .hfront_porch       = FB_DEFAULT_HFP,
//        .vback_porch        = FB_DEFAULT_VBP,
//        .vsync_width        = FB_DEFAULT_VSW,
//        .vfront_porch       = FB_DEFAULT_VFP,

//        .pf_timing_set      = bsp_port_fb_timing_set,
//        .pf_resolution_set  = bsp_port_fb_resolution_set,
//        .pf_pixel_info_get  = bsp_port_fb_pixel_info_get,
//        .pf_color_mode_set  = bsp_port_fb_rgb565_set,
//        .pf_layer_set       = bsp_port_fb_layer0_set,

//        .pf_init            = bsp_port_fb_init,
//        .pf_backlight_on    = bsp_port_fb_backlight_on,
//        .pf_backlight_off   = bsp_port_fb_backlight_off,
//        .pf_fill_rect       = bsp_port_lcd_fill_rectangle,
//        .pf_fill_screen     = bsp_port_lcd_fill_screen,
//        .pf_copy_buffer     = bsp_port_lcd_copy_buffer,
//        .pf_gpu_ready       = bsp_port_fb_gpu_ready,
//    };

//    bsp_driver_fb_link(&fb0, &fb0_handle);

//    fb0.pf_set_start_addr(&fb0, 0xD0000000);
//    fb0.pf_set_rotated(&fb0, DISP_ROT_0);
//    
//    return 0;
}

static void fb0_backlight_on(void)
{
    fb0.pf_backlight_on(&fb0);
}

static void fb0_backlight_off(void)
{
    fb0.pf_backlight_off(&fb0);
}

static void fb0_put_pixel(uint16_t x, uint16_t y, uint32_t color)
{
    fb0.pf_put_pixel(&fb0, x, y, color);
}

static void fb0_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    fb0.handle->pf_gpu_ready( fb0.pixel_format, 
                              fb0.pixel_size, 
                              fb0.rotated, 
                              fb0.handle->active_width, 
                              fb0.handle->active_height, 
                              fb0.start_address );

    fb0.pf_fill_rect(&fb0, x, y, width, height, color);
}

static void fb0_fill_screen(uint32_t color)
{
    fb0.handle->pf_gpu_ready( fb0.pixel_format, 
                              fb0.pixel_size, 
                              fb0.rotated, 
                              fb0.handle->active_width, 
                              fb0.handle->active_height, 
                              fb0.start_address );

    fb0.pf_fill_screen(&fb0, color);
}

static void fb0_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    fb0.handle->pf_gpu_ready( fb0.pixel_format, 
                              fb0.pixel_size, 
                              fb0.rotated, 
                              fb0.handle->active_width, 
                              fb0.handle->active_height, 
                              fb0.start_address );

    fb0.pf_copy_buffer(&fb0, x, y, width, height, data);
}


static int st7735_init(void)
{
    return 0;
}

static void st7735_backlight_on(void)
{
}
static void st7735_backlight_off(void)
{
}
static void st7735_put_pixel(uint16_t x, uint16_t y, uint32_t color)
{
}
static void st7735_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
}
static void st7735_fill_screen(uint32_t color)
{
}
static void st7735_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
}


/******************************* (END OF FILE) *********************************/

