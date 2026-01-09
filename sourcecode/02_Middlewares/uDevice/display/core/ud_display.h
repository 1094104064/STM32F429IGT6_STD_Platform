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
#include <stddef.h>
/*********************
 *      DEFINES
 *********************/
struct ud_display_ops
{
    void    (* put_pixel)               (struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint32_t color);
    void    (* fill_area)               (struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
    void    (* fill_screen)             (struct ud_display_ops ** ops, uint32_t color);
    void    (* transfer_pixels)         (struct ud_display_ops ** ops, const void * data, size_t size);

    void    (* enable_backlight)        (struct ud_display_ops ** ops, bool state);
    void    (* set_backlight_brightness)(struct ud_display_ops ** ops, uint8_t brightness);
    uint8_t (* get_backlight_brightness)(struct ud_display_ops ** ops);
    void    (* ioctl)                   (struct ud_display_ops ** ops, unsigned int cmd, void * arg);
};

struct ud_display
{
    const struct ud_display_ops * const ops;

    uint16_t width;
    uint16_t height;
};

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_TEMPLATE_H*/


/******************************* (END OF FILE) *********************************/




