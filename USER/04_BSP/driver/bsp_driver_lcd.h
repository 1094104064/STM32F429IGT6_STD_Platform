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
#define LCD_DEBUG_ENABLE 1

/**********************
 *      TYPEDEFS
 **********************/
typedef int (* pf_printf_t)(const char* format, ...);

typedef enum 
{
    LCD_LOG_NONE = 0,
    LCD_LOG_ERROR,
    LCD_LOG_INFO,
    LCD_LOG_DEBUG,
} lcd_log_level_t;

typedef struct lcd_handle lcd_handle_t;
typedef struct lcd_driver lcd_driver_t;


struct lcd_handle
{
    void     (* pf_hardware_init)       (void);
    void     (* pf_put_pixel)           (uint16_t x, uint16_t y, uint32_t color, uint16_t rotated);
    void     (* pf_fill_area)           (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void     (* pf_fill_screen)         (uint32_t color);
    void     (* pf_flush)               (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    void     (* pf_switch_framebuffer)  (uint8_t layerx);
    void     (* pf_backlight_init)      (void);
    void     (* pf_backlight_on)        (void);
    void     (* pf_backlight_off)       (void);
    void     (* pf_backlight_set)       (uint8_t brightness);
    uint16_t (* pf_get_width)           (void);
    uint16_t (* pf_get_height)          (void);
    uint32_t (* pf_get_framebuffer)     (void);
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

    bool (* pf_init)                (lcd_driver_t * self);
    void (* pf_put_pixel)           (lcd_driver_t * self, uint16_t x, uint16_t y, uint32_t color);
    void (* pf_fill_area)           (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fill_screen)         (lcd_driver_t * self, uint32_t color);
    void (* pf_flush)         (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    void (* pf_switch_framebuffer)  (lcd_driver_t * self, uint8_t layerx);
    void (* pf_set_orientation)     (lcd_driver_t * self, uint16_t rotated);
    void (* pf_backlight_init)      (lcd_driver_t * self);
    void (* pf_backlight_on)        (lcd_driver_t * self);
    void (* pf_backlight_off)       (lcd_driver_t * self);
    void (* pf_backlight_set)       (lcd_driver_t * self, uint8_t brightness);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_lcd_link(lcd_driver_t * drv, const lcd_handle_t * handle);

#if LCD_DEBUG_ENABLE
void bsp_driver_lcd_log_init(pf_printf_t cb, lcd_log_level_t level);
#endif  
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_LCD_H*/


/******************************* (END OF FILE) *********************************/

