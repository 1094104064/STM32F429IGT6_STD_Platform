/**
  ******************************************************************************
  *
  * @file    bsp_driver_fb.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_fb module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_DRIVER_FB_H
#define _BSP_DRIVER_FB_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/

#define FB_DEBUG_ENABLE 0

#if FB_DEBUG_ENABLE

    #define fb_dbg(fmt, ...)         printf("%s [%d] : " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

    #define fb_assert_null(param)                                                            \
            do {                                                                              \
                if(param == NULL) { fb_dbg("NULL pointer: %s", #param); while(1); }          \
            } while (0)    
#else
    #define fb_dbg(fmt, ...)             do {} while (0)
    #define fb_assert_null(param)        do {} while (0)
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct fb_handle fb_handle_t;
typedef struct fb_driver fb_driver_t;


struct fb_handle
{
    uint16_t        active_width;
    uint16_t        active_height;

    uint32_t        hsync_width;
    uint32_t        hback_porch;
    uint32_t        hfront_porch;
    uint32_t        vback_porch;
    uint32_t        vsync_width;
    uint32_t        vfront_porch;

    void (* pf_timing_set)          (uint32_t hsync_width, uint32_t hback_porch, uint32_t hfront_porch, uint32_t vsync_width, uint32_t vback_porch, uint32_t vfront_porch);
    void (* pf_resolution_set)      (uint16_t width, uint16_t height);
    void (* pf_pixel_info_get)      (uint8_t * pixel_format, uint8_t * pixel_size);
    void (* pf_color_mode_set)      (void);
    void (* pf_layer_set)           (uint32_t address);

    void (* pf_init)                (void);
    void (* pf_backlight_on)        (void);
    void (* pf_backlight_off)       (void);
    void (* pf_fill_rect)           (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fill_screen)         (uint32_t color);
    void (* pf_copy_buffer)         (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    void (* pf_gpu_ready)           ( uint8_t pixel_format, uint8_t pixel_size, uint16_t rotated, 
                                      uint16_t width, uint16_t height, uint32_t address );
};

struct fb_driver
{
    const fb_handle_t * handle;

    uint8_t         pixel_format;
    uint8_t         pixel_size;
    uint16_t        rotated;
    uint32_t        start_address;

    bool (* pf_init)            (fb_driver_t * self);
    void (* pf_backlight_on)    (fb_driver_t * self);
    void (* pf_backlight_off)   (fb_driver_t * self);
    void (* pf_put_pixel)       (fb_driver_t * self, uint16_t x, uint16_t y, uint32_t color);
    void (* pf_fill_rect)       (fb_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fill_screen)     (fb_driver_t * self, uint32_t color);
    void (* pf_copy_buffer)     (fb_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    void (* pf_set_start_addr)  (fb_driver_t * self, uint32_t address);
    void (* pf_set_rotated)     (fb_driver_t * self, uint16_t rotated);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_fb_link(fb_driver_t * drv, const fb_handle_t * handle);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_FB_H*/


/******************************* (END OF FILE) *********************************/

