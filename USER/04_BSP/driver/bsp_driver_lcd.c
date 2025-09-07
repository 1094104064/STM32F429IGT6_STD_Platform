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
static bool lcd_layer_config(struct lcd_driver * self);
static void lcd_backlight_on(struct lcd_driver * self);
static void lcd_backlight_off(struct lcd_driver * self);
static void lcd_put_pixel(struct lcd_driver * self, uint16_t x, uint16_t y, uint32_t color);
static void lcd_fill_rect(struct lcd_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
static void lcd_fill_screen(struct lcd_driver * self, uint32_t color);
static void lcd_copy_buffer(struct lcd_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_lcd_link(struct lcd_driver * drv, struct lcd_oper * oper, const struct lcd_panel * panel, struct lcd_layer * layer)
{
    lcd_assert_null(drv);
    lcd_assert_null(oper);
    lcd_assert_null(panel);
    lcd_assert_null(layer);

    if (drv == NULL || oper == NULL || panel == NULL || layer == NULL) {
        return;
    }

    drv->oper = oper;
    drv->panel = panel;
    drv->layer = layer;

    drv->pf_init            = lcd_layer_config;
    drv->pf_backlight_on    = lcd_backlight_on;
    drv->pf_backlight_off   = lcd_backlight_off;
    drv->pf_put_pixel       = lcd_put_pixel;
    drv->pf_fill_rect       = lcd_fill_rect;
    drv->pf_fill_screen     = lcd_fill_screen;
    drv->pf_copy_buffer     = lcd_copy_buffer;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool lcd_layer_config(struct lcd_driver * self)
{
    lcd_assert_null(self->oper->pf_layer_config);
    lcd_assert_null(self->oper->pf_backlight_on);
    lcd_assert_null(self->oper->pf_backlight_off);
    lcd_assert_null(self->oper->pf_put_pixel);
    lcd_assert_null(self->oper->pf_fast_fill_rect);
    lcd_assert_null(self->oper->pf_fast_fill_screen);
    lcd_assert_null(self->oper->pf_copy_buffer);

    if (self->oper->pf_layer_config      == NULL ||
        self->oper->pf_backlight_on      == NULL ||
        self->oper->pf_backlight_off     == NULL ||
        self->oper->pf_put_pixel         == NULL || 
        self->oper->pf_fast_fill_rect    == NULL || 
        self->oper->pf_fast_fill_screen  == NULL ||
        self->oper->pf_copy_buffer       == NULL) {
        return false;
    }

    self->oper->pf_layer_config( &self->layer->pixel_size, 
                                 &self->layer->rotated, 
                                 &self->layer->start_address);

    lcd_dbg("lcd init successfully");

    return true;
}


static void lcd_backlight_on(struct lcd_driver * self)
{
    self->oper->pf_backlight_on();
}

static void lcd_backlight_off(struct lcd_driver * self)
{
    self->oper->pf_backlight_off();
}

static void lcd_put_pixel(struct lcd_driver * self, uint16_t x, uint16_t y, uint32_t color)
{
    uint16_t rotated = self->layer->rotated;
    uint32_t pixel_pos = 0;

    if(rotated == 0) {
        pixel_pos = y * self->panel->active_width + x;
    }
    else if(rotated == 90) {
        pixel_pos = x * self->panel->active_width + (self->panel->active_width - 1 - y);
    }
    else if(rotated == 180) {
        pixel_pos = (self->panel->active_height - 1 - y) * self->panel->active_width + 
                    (self->panel->active_width - 1 - x);
    }
    else if(rotated == 270) {
        pixel_pos = (self->panel->active_height - 1 - x) * self->panel->active_width + y;
    }

    uint8_t pixel_size = self->layer->pixel_size;


    if(pixel_size == 4) {
        *(volatile uint32_t*)( self->layer->start_address + 4 * pixel_pos ) = color ;
    }
    else if(pixel_size == 3) {
        *(volatile uint16_t*)( self->layer->start_address + 3 * pixel_pos ) = color;
        *(volatile uint8_t*)( self->layer->start_address + 3 * pixel_pos + 2 ) = color >> 16;
    }
    else if(pixel_size == 2) {
        *(volatile uint16_t*)(self->layer->start_address + 2 * pixel_pos ) = color;
    }
    else {

    }
}

static void lcd_fill_rect(struct lcd_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    self->oper->pf_fast_fill_rect(x, y, width, height, color);
}

static void lcd_fill_screen(struct lcd_driver * self, uint32_t color)
{
    self->oper->pf_fast_fill_screen(color);
}

static void lcd_copy_buffer(struct lcd_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    self->oper->pf_copy_buffer(x, y, width, height, data);
}






/******************************* (END OF FILE) *********************************/

