/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_display.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_display module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_WRAPPER_DISPLAY_H
#define _BSP_WRAPPER_DISPLAY_H


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
#define DISPLAY_MAX_NUM 2
/**********************
 *      TYPEDEFS
 **********************/

enum {
    DISP_RGB565_BLACK       = 0x0000,         // 黑色
    DISP_RGB565_WHITE       = 0xFFFF,         // 白色
    DISP_RGB565_RED         = 0xF800,         // 红色
    DISP_RGB565_GREEN       = 0x07E0,         // 绿色
    DISP_RGB565_BLUE        = 0x001F,         // 蓝色
    DISP_RGB565_YELLOW      = 0xFFE0,         // 黄色
    DISP_RGB565_CYAN        = 0x07FF,         // 青色
    DISP_RGB565_MAGENTA     = 0xF81F,         // 品红
    DISP_RGB565_GRAY        = 0x8410,         // 灰色
    DISP_RGB565_ORANGE      = 0xFC00,         // 橙色
    DISP_RGB565_BROWN       = 0xA145,         // 棕色
    DISP_RGB565_PURPLE      = 0x8010,         // 紫色
};

enum {
    DISP_RGB888_BLACK       = 0x000000,     // 黑色
    DISP_RGB888_WHITE       = 0xFFFFFF,     // 白色
    DISP_RGB888_RED         = 0xFF0000,     // 红色
    DISP_RGB888_GREEN       = 0x00FF00,     // 绿色
    DISP_RGB888_BLUE        = 0x0000FF,     // 蓝色
    DISP_RGB888_YELLOW      = 0xFFFF00,     // 黄色
    DISP_RGB888_CYAN        = 0x00FFFF,     // 青色
    DISP_RGB888_MAGENTA     = 0xFF00FF,     // 品红
    DISP_RGB888_GRAY        = 0x808080,     // 灰色
    DISP_RGB888_ORANGE      = 0xFFA500,     // 橙色
    DISP_RGB888_BROWN       = 0xA52A2A,     // 棕色
    DISP_RGB888_PURPLE      = 0x800080,     // 紫色
};


struct display_wrapper {
    int8_t          idx;
    const char *    name;
    void *          user_data;
    uint32_t        width;
    uint32_t        height;

    int  (* pf_init)            (struct display_wrapper * self);
    void (* pf_backlight_on)    (struct display_wrapper * self);
    void (* pf_backlight_off)   (struct display_wrapper * self);
    void (* pf_put_pixel)       (struct display_wrapper * self, uint16_t x, uint16_t y, uint32_t color);
    void (* pf_fill_rect)       (struct display_wrapper * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void (* pf_fill_screen)     (struct display_wrapper * self, uint32_t color);
    void (* pf_copy_buffer)     (struct display_wrapper * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
};


/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_display_link(struct display_wrapper * self);
bool bsp_wrapper_display_init(void);
void bsp_wrapper_display_backlight_on(void);
void bsp_wrapper_display_backlight_off(void);
void bsp_wrapper_display_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void bsp_wrapper_display_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void bsp_wrapper_display_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
void bsp_wrapper_display_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
void bsp_wrapper_display_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
void bsp_wrapper_display_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void bsp_wrapper_display_draw_arc(uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color);
void bsp_wrapper_display_draw_ellipse(int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color);
void bsp_wrapper_display_fill_screen(uint32_t color);
void bsp_wrapper_display_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * image_data);
void bsp_wrapper_display_draw_grad_rgb565(uint16_t grid_size);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_DISPLAY_H*/


/******************************* (END OF FILE) *********************************/

