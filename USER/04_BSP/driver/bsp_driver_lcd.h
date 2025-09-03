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
#include "user_conf.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct lcd_layer {
    uint8_t pixel_size;
    uint16_t rotated;
    uint32_t start_address;
};



struct lcd_panel {

    const char * name;

    uint16_t active_width;
    uint16_t active_height;

    uint32_t hsync_width;
    uint32_t hback_porch;
    uint32_t hfront_porch;
    uint32_t vback_porch;
    uint32_t vsync_width;
    uint32_t vfront_porch;

};


struct lcd_oper {
    void (* pf_layer_config)(uint8_t * pixel_size, uint16_t * rotated, uint32_t * start_address);
    void (* pf_backlight_on)(void);
    void (* pf_backlight_off)(void);
    void (* pf_put_pixel)(uint16_t x, uint16_t y, uint32_t color);
    void (* pf_fast_fill_rect)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fast_fill_screen)(uint32_t color);
    void (* pf_copy_buffer)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
};

struct lcd_driver {
    struct lcd_oper * oper;
    const struct lcd_panel * panel;
    struct lcd_layer * layer;

    bool (* pf_init)(struct lcd_driver * self);
    void (* pf_backlight_on)(struct lcd_driver * self);
    void (* pf_backlight_off)(struct lcd_driver * self);
    void (* pf_put_pixel)(struct lcd_driver * self, uint16_t x, uint16_t y, uint32_t color);
    void (* pf_fill_rect)(struct lcd_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fill_screen)(struct lcd_driver * self, uint32_t color);
    void (* pf_copy_buffer)(struct lcd_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_lcd_link(struct lcd_driver * drv, struct lcd_oper * oper, const struct lcd_panel * panel, struct lcd_layer * layer);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_LCD_H*/


/******************************* (END OF FILE) *********************************/

