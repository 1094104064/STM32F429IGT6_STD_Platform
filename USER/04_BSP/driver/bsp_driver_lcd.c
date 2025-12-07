/**
  ******************************************************************************
  *
  * @file    bsp_driver_lcd.c
  * @author  
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
#include "bsp_driver_lcd.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#if LCD_DEBUG_ENABLE

static pf_printf_t printf_cb = NULL;

static lcd_log_level_t log_level = LCD_LOG_NONE;

#define LOG_OUTPUT(level, fmt, ...) do { \
    if (printf_cb && level <= log_level) { \
        printf_cb("[LCD] " fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_E(fmt, ...) LOG_OUTPUT(LCD_LOG_ERROR, "E: " fmt"\r\n", ##__VA_ARGS__)    /* 用于输出详细的调试信息 */
#define LOG_I(fmt, ...) LOG_OUTPUT(LCD_LOG_INFO,  "I: " fmt"\r\n", ##__VA_ARGS__)    /* 用于记录重要的操作节点或状态变化 */
#define LOG_D(fmt, ...) LOG_OUTPUT(LCD_LOG_DEBUG, "D: " fmt"\r\n", ##__VA_ARGS__)    /* 用于表示发生了错误 */


#define ASSERT_NULL(param)                                                          \
        do {                                                                        \
            if(param == NULL) { LOG_E("NULL pointer: \r\n", #param); while(1); }    \
        } while (0)

#else

#define LOG_E(fmt, ...)
#define LOG_I(fmt, ...)
#define LOG_D(fmt, ...)

#define ASSERT_NULL(param)                                                          \
        do {                                                                        \
            if(param == NULL) { while(1); }                                         \
        } while (0)

#endif
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool lcd_init                (lcd_driver_t * self);
static void lcd_put_pixel           (lcd_driver_t * self, uint16_t x, uint16_t y, uint32_t color);
static void lcd_fill_area           (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
static void lcd_fill_screen         (lcd_driver_t * self, uint32_t color);
static void lcd_flush         (lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
static void lcd_switch_framebuffer  (lcd_driver_t * self, uint8_t layerx);
static void lcd_set_orientation     (lcd_driver_t * self, uint16_t rotated);
static void lcd_backlight_on        (lcd_driver_t * self);
static void lcd_backlight_off       (lcd_driver_t * self);
static void lcd_backlight_set       (lcd_driver_t * self, uint8_t brightness);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_lcd_link(lcd_driver_t * drv, const lcd_handle_t * handle)
{
    ASSERT_NULL(drv);
    ASSERT_NULL(handle);

    drv->handle = handle;

    drv->pf_init                = lcd_init;
    drv->pf_backlight_on        = lcd_backlight_on;
    drv->pf_backlight_off       = lcd_backlight_off;
    drv->pf_backlight_set       = lcd_backlight_set;
    drv->pf_put_pixel           = lcd_put_pixel;
    drv->pf_fill_area           = lcd_fill_area;
    drv->pf_fill_screen         = lcd_fill_screen;
    drv->pf_flush               = lcd_flush;
    drv->pf_switch_framebuffer  = lcd_switch_framebuffer;
    drv->pf_set_orientation     = lcd_set_orientation;
}

#if LCD_DEBUG_ENABLE
void bsp_driver_lcd_log_init(pf_printf_t cb, lcd_log_level_t level)
{
    printf_cb = cb;
    log_level = level;
}
#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool lcd_init(lcd_driver_t * self)
{
    ASSERT_NULL(self->handle->pf_hardware_init);
    ASSERT_NULL(self->handle->pf_backlight_on);
    ASSERT_NULL(self->handle->pf_backlight_off);
    ASSERT_NULL(self->handle->pf_backlight_set);
    ASSERT_NULL(self->handle->pf_put_pixel);
    ASSERT_NULL(self->handle->pf_fill_area);
    ASSERT_NULL(self->handle->pf_fill_screen);
    ASSERT_NULL(self->handle->pf_flush);
    ASSERT_NULL(self->handle->pf_switch_framebuffer);
    ASSERT_NULL(self->handle->pf_get_width);
    ASSERT_NULL(self->handle->pf_get_height);
    ASSERT_NULL(self->handle->pf_get_framebuffer);

    self->handle->pf_hardware_init();

    self->width         = self->handle->pf_get_width();
    self->height        = self->handle->pf_get_height();
    self->framebuffer   = self->handle->pf_get_framebuffer();
    self->rotated       = 0;

    return true;
}


static void lcd_put_pixel(lcd_driver_t * self, uint16_t x, uint16_t y, uint32_t color)
{
    self->handle->pf_put_pixel(x, y, color, self->rotated);
}

static void lcd_fill_area(lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    self->handle->pf_fill_area(x, y, width, height, color);
}

static void lcd_fill_screen(lcd_driver_t * self, uint32_t color)
{
    self->handle->pf_fill_screen(color);
}

static void lcd_flush(lcd_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    self->handle->pf_flush(x, y, width, height, data);
}

static void lcd_switch_framebuffer(lcd_driver_t * self, uint8_t layerx)
{
    self->handle->pf_switch_framebuffer(layerx);
}


static void lcd_set_orientation(lcd_driver_t * self, uint16_t rotated)
{
    self->rotated = rotated;
}


static void lcd_backlight_on(lcd_driver_t * self)
{
    self->handle->pf_backlight_on();
}

static void lcd_backlight_off(lcd_driver_t * self)
{
    self->handle->pf_backlight_off();
}

static void lcd_backlight_set(lcd_driver_t * self, uint8_t brightness)
{
    self->handle->pf_backlight_set(brightness);
}

/******************************* (END OF FILE) *********************************/

