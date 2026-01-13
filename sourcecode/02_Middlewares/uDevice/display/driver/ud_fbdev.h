/**
  ******************************************************************************
  *
  * @file    ud_fbdev.h
  * @author  
  * @brief   Header file of framebuffer device module.
  *
  ******************************************************************************
  **/
  
#ifndef _UD_FBDEV_H
#define _UD_FBDEV_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "../core/ud_display.h"
#include "../adapter/panel_rgb.h"
#include "../adapter/panel_backlight.h"
/*********************
 *      DEFINES
 *********************/
struct ud_fbdev_bitfield
{
    uint32_t length;
    uint32_t offset;
    uint32_t msb_right;
};

struct ud_fbdev_fix
{
    void * start_address;
};


struct ud_fbdev_var
{
    uint16_t width;
    uint16_t height;

    uint32_t bits_per_pixel;
    
    struct ud_fbdev_bitfield red;
    struct ud_fbdev_bitfield green;
    struct ud_fbdev_bitfield blue;
    struct ud_fbdev_bitfield alpha;
};


struct ud_fbdev
{
    const struct ud_display_ops * display_ops;
    const struct panel_rgb_ops  ** rgb_ops;
    const struct panel_backlight_ops ** backlight_ops;

    const struct ud_fbdev_fix  * fix;
    const struct ud_fbdev_var  * var;
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

#endif /*_UD_FBDEV_H*/


/******************************* (END OF FILE) *********************************/


