/**
  ******************************************************************************
  *
  * @file    bsp_driver_lcd.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_lcd module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_DRIVER_LCD_H
#define _BSP_DRIVER_LCD_H


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

#define LCD_DEBUG_ENABLE 0

#if LCD_DEBUG_ENABLE

    #define lcd_dbg(fmt, ...)         printf("%s [%d] : " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define lcd_assert_null(param)                                                            \
            do {                                                                              \
                if(param == NULL) { lcd_dbg("NULL pointer: %s", #param); while(1); }          \
            } while (0)    
#else
    #define lcd_dbg(fmt, ...)             do {} while (0)
    #define lcd_assert_null(param)        do {} while (0)
#endif

/**********************
 *      TYPEDEFS
 **********************/


typedef struct lcd_handle lcd_handle_t;
typedef struct lcd_driver lcd_driver_t;


struct lcd_handle
{
    void     (* pf_hal_init)            (void);
    void     (* pf_backlight_on)        (void);
    void     (* pf_backlight_off)       (void);
    void     (* pf_backlight_set)       (uint8_t brightness);
    void     (* pf_fill_rect)           (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void     (* pf_fill_screen)         (uint32_t color);
    void     (* pf_copy_buffer)         (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    uint16_t (* pf_get_width)           (void);
    uint16_t (* pf_get_height)          (void);
    uint32_t (* pf_get_start_addr)      (void);
};

struct lcd_driver
{
    const lcd_handle_t * handle;

    uint8_t         pixel_format;
    uint8_t         pixel_size;
    uint16_t        rotated;

    uint16_t        width;
    uint16_t        height;
    uint32_t        framebuffer;

    bool (* pf_init)            (lcd_driver_t * self);
    void (* pf_backlight_on)    (lcd_driver_t * self);
    void (* pf_backlight_off)   (lcd_driver_t * self);
    void (* pf_put_pixel)       (lcd_driver_t * self, uint16_t x, uint16_t y, uint32_t color);
    void (* pf_fill_rect)       (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fill_screen)     (lcd_driver_t * self, uint32_t color);
    void (* pf_copy_buffer)     (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    void (* pf_set_start_addr)  (lcd_driver_t * self, uint32_t address);
    void (* pf_set_rotated)     (lcd_driver_t * self, uint16_t rotated);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_lcd_link(lcd_driver_t * drv, const lcd_handle_t * handle);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_LCD_H*/


/******************************* (END OF FILE) *********************************/

