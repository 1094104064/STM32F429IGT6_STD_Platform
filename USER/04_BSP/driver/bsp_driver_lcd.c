/**
  ******************************************************************************
  *
  * @file    bsp_driver_lcd.c
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
#include "bsp_driver_lcd.h"
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
static bool lcd_init            (lcd_driver_t * self);
static void lcd_backlight_on    (lcd_driver_t * self);
static void lcd_backlight_off   (lcd_driver_t * self);
static void lcd_put_pixel       (lcd_driver_t * self, uint16_t x, uint16_t y, uint32_t color);
static void lcd_fill_rect       (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
static void lcd_fill_screen     (lcd_driver_t * self, uint32_t color);
static void lcd_copy_buffer     (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);

static void lcd_set_rotated     (lcd_driver_t * self, uint16_t rotated);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_lcd_link(lcd_driver_t * drv, const lcd_handle_t * handle)
{
    lcd_assert_null(drv);
    lcd_assert_null(handle);

    if (drv == NULL || handle == NULL) {
        return;
    }

    drv->handle = handle;

    drv->pf_init            = lcd_init;
    drv->pf_backlight_on    = lcd_backlight_on;
    drv->pf_backlight_off   = lcd_backlight_off;
    drv->pf_put_pixel       = lcd_put_pixel;
    drv->pf_fill_rect       = lcd_fill_rect;
    drv->pf_fill_screen     = lcd_fill_screen;
    drv->pf_copy_buffer     = lcd_copy_buffer;
    drv->pf_set_rotated     = lcd_set_rotated;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/


static bool lcd_init(lcd_driver_t * self)
{
    self->handle->pf_hal_init();

    
    self->width = self->handle->pf_get_width();
    self->height = self->handle->pf_get_height();
    self->framebuffer = self->handle->pf_get_framebuffer();
}

static void lcd_backlight_on(lcd_driver_t * self)
{
    self->handle->pf_backlight_on();
}

static void lcd_backlight_off(lcd_driver_t * self)
{
    self->handle->pf_backlight_off();
}

static void lcd_put_pixel(lcd_driver_t * self, uint16_t x, uint16_t y, uint32_t color)
{
    self->handle->pf_put_pixel(x, y, color, self->rotated);
}

static void lcd_fill_rect(lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    self->handle->pf_fill_rect(x, y, width, height, color);
}

static void lcd_fill_screen(lcd_driver_t * self, uint32_t color)
{
    self->handle->pf_fill_screen(color);
}

static void lcd_copy_buffer(lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    self->handle->pf_copy_buffer(x, y, width, height, data);
}



static void lcd_set_rotated(lcd_driver_t * self, uint16_t rotated)
{
    self->rotated = rotated;
}



/******************************* (END OF FILE) *********************************/

