/**
  ******************************************************************************
  *
  * @file    ud_display.h
  * @author  
  * @brief   Header file of display module.
  *
  ******************************************************************************
  **/

#ifndef _UD_DISPLAY_H
#define _UD_DISPLAY_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
/**********************
 *      TYPEDEFS
 **********************/
typedef struct ud_display_ops display_ops_t;

typedef void (* display_prepare_fn_t)                 (const display_ops_t ** ops);
typedef void (* display_put_pixel_fn_t)               (const display_ops_t ** ops, uint16_t x, uint16_t y, uint32_t color);
typedef void (* display_fill_area_fn_t)               (const display_ops_t ** ops, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
typedef void (* display_transfer_pixels_fn_t)         (const display_ops_t ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const void * data);
typedef void (* display_enable_backlight_fn_t)        (const display_ops_t ** ops, bool state);
typedef void (* display_set_backlight_brightness_fn_t)(const display_ops_t ** ops, uint8_t brightness);
typedef void (* display_get_backlight_brightness_fn_t)(const display_ops_t ** ops, uint8_t * brightness);
typedef void (* display_ioctl_fn_t)                   (const display_ops_t ** ops, unsigned int cmd, void * arg);
/*********************
 *      DEFINES
 *********************/
struct ud_display_ops
{
    display_prepare_fn_t                  prepare;
    display_put_pixel_fn_t                put_pixel;
    display_fill_area_fn_t                fill_area;
    display_transfer_pixels_fn_t          transfer_pixels;
    display_enable_backlight_fn_t         enable_backlight;
    display_set_backlight_brightness_fn_t set_backlight_brightness;
    display_get_backlight_brightness_fn_t get_backlight_brightness;
    display_ioctl_fn_t                    ioctl;
};

struct ud_display
{
    const struct ud_display_ops ** ops;

    uint16_t width;
    uint16_t height;
};



/**********************
*  GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_UD_DISPLAY_H*/


/******************************* (END OF FILE) *********************************/




