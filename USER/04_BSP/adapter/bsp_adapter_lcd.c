/**
  ******************************************************************************
  *
  * @file    bsp_adapter_lcd.c
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
#include "bsp_adapter_lcd.h"
#include "bsp_driver_lcd.h"
#include "bsp_hal_lcd.h"
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
static lcd_driver_t gs_lcd_driver;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/
static int ops_lcd_init(void)
{
    static const lcd_handle_t simple_panel_handle = {
        .pf_hal_init        = bsp_hal_lcd_simple_panel_init,
        .pf_backlight_on    = bsp_hal_lcd_backlight_on,
        .pf_backlight_off   = bsp_hal_lcd_backlight_off,
        .pf_backlight_set   = bsp_hal_lcd_backlight_set,
        .pf_put_pixel       = bsp_hal_lcd_put_pixel,
        .pf_fill_rect       = bsp_hal_lcd_fill_rectangle,
        .pf_fill_screen     = bsp_hal_lcd_fill_screen,
        .pf_copy_buffer     = bsp_hal_lcd_copy_buffer,
        .pf_get_width       = bsp_hal_lcd_get_width,
        .pf_get_height      = bsp_hal_lcd_get_height,
        .pf_get_framebuffer = bsp_hal_lcd_get_layer_address,
    };

}

static void ops_lcd_backlight_on(void)
{
    gs_lcd_driver.pf_backlight_on(&gs_lcd_driver);
}

static void ops_lcd_backlight_off(void)
{
    gs_lcd_driver.pf_backlight_off(&gs_lcd_driver);
}

static void ops_lcd_put_pixel(uint16_t x, uint16_t y, uint32_t color)
{
    gs_lcd_driver.pf_put_pixel(&gs_lcd_driver, x, y, color);
}

static void ops_lcd_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    gs_lcd_driver.pf_fill_rect(&gs_lcd_driver, x, y, width, height, color);
}

static void ops_lcd_fill_screen(uint32_t color)
{
    gs_lcd_driver.pf_fill_screen(&gs_lcd_driver, color);
}

static void ops_lcd_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    gs_lcd_driver.pf_copy_buffer(&gs_lcd_driver, x, y, width, height, data);
}

static uint16_t ops_lcd_get_width(void)
{
    return gs_lcd_driver.width;
}

static uint16_t ops_lcd_get_height(void)
{
    return gs_lcd_driver.height;
}

static uint32_t ops_lcd_get_framebuffer(void)
{
    return gs_lcd_driver.framebuffer;
}

const display_ops_t lcd_ops = {
    .pf_init            = ops_lcd_init,
    .pf_backlight_on    = ops_lcd_backlight_on,
    .pf_backlight_off   = ops_lcd_backlight_off,
    .pf_put_pixel       = ops_lcd_put_pixel,
    .pf_fill_rect       = ops_lcd_fill_rect,
    .pf_fill_screen     = ops_lcd_fill_screen,
    .pf_copy_buffer     = ops_lcd_copy_buffer,
    .pf_get_width       = ops_lcd_get_width,
    .pf_get_height      = ops_lcd_get_height,
    .pf_get_framebuffer = ops_lcd_get_framebuffer,
};

/******************************* (END OF FILE) *********************************/



 







