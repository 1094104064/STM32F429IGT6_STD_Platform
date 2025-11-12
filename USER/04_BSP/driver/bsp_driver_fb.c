/**
  ******************************************************************************
  *
  * @file    bsp_driver_fb.c
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
#include "bsp_driver_fb.h"
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
static bool fb_init         (fb_driver_t * self);
static void fb_backlight_on (fb_driver_t * self);
static void fb_backlight_off(fb_driver_t * self);
static void fb_put_pixel    (fb_driver_t * self, uint16_t x, uint16_t y, uint32_t color);
static void fb_fill_rect    (fb_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
static void fb_fill_screen  (fb_driver_t * self, uint32_t color);
static void fb_copy_buffer  (fb_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
static void fb_set_start_addr(fb_driver_t * self, uint32_t address);
static void fb_set_rotated(fb_driver_t * self, uint16_t rotated);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_fb_link(fb_driver_t * drv, const fb_handle_t * handle)
{
    fb_assert_null(drv);
    fb_assert_null(handle);

    if (drv == NULL || handle == NULL) {
        return;
    }

    drv->handle = handle;

    drv->pf_init            = fb_init;
    drv->pf_backlight_on    = fb_backlight_on;
    drv->pf_backlight_off   = fb_backlight_off;
    drv->pf_put_pixel       = fb_put_pixel;
    drv->pf_fill_rect       = fb_fill_rect;
    drv->pf_fill_screen     = fb_fill_screen;
    drv->pf_copy_buffer     = fb_copy_buffer;
    drv->pf_set_start_addr  = fb_set_start_addr;
    drv->pf_set_rotated     = fb_set_rotated;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/


static bool fb_init(fb_driver_t * self)
{
    self->handle->pf_timing_set( self->handle->hsync_width,
                                 self->handle->hback_porch,
                                 self->handle->hfront_porch,
                                 self->handle->vsync_width,
                                 self->handle->vback_porch,
                                 self->handle->vfront_porch );

    self->handle->pf_resolution_set( self->handle->active_width,
                                     self->handle->active_height );

    self->handle->pf_color_mode_set();  

    self->handle->pf_pixel_info_get( &self->pixel_format,
                                     &self->pixel_size );

    self->handle->pf_layer_set( self->start_address );

    self->handle->pf_init();
}

static void fb_backlight_on(fb_driver_t * self)
{
    self->handle->pf_backlight_on();
}

static void fb_backlight_off(fb_driver_t * self)
{
    self->handle->pf_backlight_off();
}

static void fb_put_pixel(fb_driver_t * self, uint16_t x, uint16_t y, uint32_t color)
{
    uint16_t rotated        = self->rotated;
    uint32_t pixel_pos      = 0;
    uint32_t width          = self->handle->active_width;
    uint32_t height         = self->handle->active_height;
    uint8_t pixel_size      = self->pixel_size;
    uint32_t start_address  = self->start_address;

    if(rotated == 0) {
        pixel_pos = y * width + x;
    }
    else if(rotated == 90) {
        pixel_pos = x * width + (width - 1 - y);
    }
    else if(rotated == 180) {
        pixel_pos = (height - 1 - y) * width + 
                    (width - 1 - x);
    }
    else if(rotated == 270) {
        pixel_pos = (height - 1 - x) * width + y;
    }


    if(pixel_size == 4) {
        *(volatile uint32_t*)( start_address + 4 * pixel_pos ) = color ;
    }
    else if(pixel_size == 3) {
        *(volatile uint16_t*)( start_address + 3 * pixel_pos ) = color;
        *(volatile uint8_t*)( start_address + 3 * pixel_pos + 2 ) = color >> 16;
    }
    else if(pixel_size == 2) {
        *(volatile uint16_t*)(start_address + 2 * pixel_pos ) = color;
    }
    else {

    }
}

static void fb_fill_rect(fb_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    self->handle->pf_fill_rect(x, y, width, height, color);
}

static void fb_fill_screen(fb_driver_t * self, uint32_t color)
{
    self->handle->pf_fill_screen(color);
}

static void fb_copy_buffer(fb_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    self->handle->pf_copy_buffer(x, y, width, height, data);
}

static void fb_set_start_addr(fb_driver_t * self, uint32_t address)
{
    self->start_address = address;
}

static void fb_set_rotated(fb_driver_t * self, uint16_t rotated)
{
    self->rotated = rotated;
}


/******************************* (END OF FILE) *********************************/

