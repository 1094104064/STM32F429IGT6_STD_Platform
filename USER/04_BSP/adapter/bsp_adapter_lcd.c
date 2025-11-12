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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/
static int ops_lcd_init(void)
{

}

static void ops_lcd_backlight_on(void)
{

}

static void ops_lcd_backlight_off(void)
{

}

static void ops_lcd_put_pixel(uint16_t x, uint16_t y, uint32_t color)
{

}

static void ops_lcd_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{

}

static void ops_lcd_fill_screen(uint32_t color)
{

}

static void ops_lcd_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{

}

const display_ops_t lcd_ops = {
    .pf_init            = ops_lcd_init,
    .pf_backlight_on    = ops_lcd_backlight_on,
    .pf_backlight_off   = ops_lcd_backlight_off,
    .pf_put_pixel       = ops_lcd_put_pixel,
    .pf_fill_rect       = ops_lcd_fill_rect,
    .pf_fill_screen     = ops_lcd_fill_screen,
    .pf_copy_buffer     = ops_lcd_copy_buffer,
};

/******************************* (END OF FILE) *********************************/



 







