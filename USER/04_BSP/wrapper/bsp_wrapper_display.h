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
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define DISPLAY_MAX_NUM         2
#define DISPLAY_NAME_MAX_LEN    16

enum 
{
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

enum 
{
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

enum 
{
    DISP_ROT_0                = 0,
    DISP_ROT_90               = 90,
    DISP_ROT_180              = 180,
    DISP_ROT_270              = 270,
};



/**********************
 *      TYPEDEFS
 **********************/
typedef struct display_object display_obj_t;
typedef struct display_ops display_ops_t;
typedef struct display_ctx display_ctx_t;

struct display_ops
{
    int      (* pf_init)            (void);
    void     (* pf_backlight_on)    (void);
    void     (* pf_backlight_off)   (void);
    void     (* pf_put_pixel)       (uint16_t x, uint16_t y, uint32_t color);
    void     (* pf_fill_rect)       (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void     (* pf_fill_screen)     (uint32_t color);
    void     (* pf_copy_buffer)     (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    uint16_t (* pf_get_width)       (void);
    uint16_t (* pf_get_height)      (void);
    uint32_t (* pf_get_framebuffer) (void);
};

struct display_ctx
{
    uint8_t         idx;
    void *          user_data;
    char            name[DISPLAY_NAME_MAX_LEN];
    bool            is_initialized;
    uint32_t        width;
    uint32_t        height;
};

struct display_object {
    const display_ops_t * ops;
    display_ctx_t ctx;
};

struct display_wrapper 
{
    display_obj_t * (* obj_create)          (const display_ops_t * ops, const char * const name, void * const user_data);
    void            (* obj_delete)          (const char * const name);
    display_obj_t * (* find)                (const char * const name);

    bool            (* init)                (display_obj_t * obj);
    void            (* backlight_on)        (display_obj_t * obj);
    void            (* backlight_off)       (display_obj_t * obj);
    void            (* backlight_set)       (display_obj_t * obj, uint8_t brightness);
    void            (* draw_pixel)          (display_obj_t * obj, uint16_t x, uint16_t y, uint32_t color);
    void            (* fill_rect)           (display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void            (* fill_screen)         (display_obj_t * obj, uint32_t color);
    void            (* draw_image)          (display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
    void            (* draw_line)           (display_obj_t * obj, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
    void            (* draw_circle)         (display_obj_t * obj, uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
    void            (* draw_triangle)       (display_obj_t * obj, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
    void            (* draw_rect)           (display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    void            (* draw_arc)            (display_obj_t * obj, uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color);
    void            (* draw_ellipse)        (display_obj_t * obj, int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color);
    uint16_t        (* get_width)           (void);
    uint16_t        (* get_height)          (void);
    uint32_t        (* get_framebuffer)     (void);
    void            (* draw_grad_rgb565)    (display_obj_t * obj, uint16_t grid_size);
};

extern const struct display_wrapper wrp_display;

/**********************
*  GLOBAL PROTOTYPES
 **********************/
display_obj_t * bsp_wrapper_display_create          (const display_ops_t *ops, const char *const name, void *const user_data);
void            bsp_wrapper_display_delete          (const char *const name);
display_obj_t * bsp_wrapper_display_find            (const char *const name);
bool            bsp_wrapper_display_init            (display_obj_t *obj);
void            bsp_wrapper_display_backlight_on    (display_obj_t *obj);
void            bsp_wrapper_display_backlight_off   (display_obj_t *obj);
void            bsp_wrapper_display_draw_pixel      (display_obj_t *obj, uint16_t x, uint16_t y, uint32_t color);
void            bsp_wrapper_display_fill_rect       (display_obj_t *obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void            bsp_wrapper_display_draw_line       (display_obj_t *obj, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
void            bsp_wrapper_display_draw_circle     (display_obj_t *obj, uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
void            bsp_wrapper_display_draw_triangle   (display_obj_t *obj, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
void            bsp_wrapper_display_draw_rect       (display_obj_t *obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void            bsp_wrapper_display_draw_arc        (display_obj_t *obj, uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color);
void            bsp_wrapper_display_draw_ellipse    (display_obj_t *obj, int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color);
void            bsp_wrapper_display_fill_screen     (display_obj_t *obj, uint32_t color);
void            bsp_wrapper_display_draw_image      (display_obj_t *obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t *image_data);
uint16_t        bsp_wrapper_display_get_width       (display_obj_t * obj);
uint16_t        bsp_wrapper_display_get_height      (display_obj_t * obj);
uint32_t        bsp_wrapper_display_get_framebuffer (display_obj_t * obj);
void            bsp_wrapper_display_draw_grad_rgb565(display_obj_t *obj, uint16_t grid_size);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_DISPLAY_H*/


/******************************* (END OF FILE) *********************************/

