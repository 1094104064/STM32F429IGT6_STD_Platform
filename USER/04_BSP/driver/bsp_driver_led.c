/**
  ******************************************************************************
  *
  * @file    bsp_driver_led.c
  * @author  
  * @brief   Source file of bsp_driver_led module.
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
#include "bsp_driver_led.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#if LED_DEBUG_ENABLE

static pf_printf_t printf_cb = NULL;

static led_log_level_t log_level = LED_LOG_NONE;

#define LOG_OUTPUT(level, fmt, ...) do { \
    if (printf_cb && level <= log_level) { \
        printf_cb("[LED] " fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_E(fmt, ...) LOG_OUTPUT(LED_LOG_ERROR, "E: " fmt"\r\n", ##__VA_ARGS__)    /* 用于输出详细的调试信息 */
#define LOG_I(fmt, ...) LOG_OUTPUT(LED_LOG_INFO,  "I: " fmt"\r\n", ##__VA_ARGS__)    /* 用于记录重要的操作节点或状态变化 */
#define LOG_D(fmt, ...) LOG_OUTPUT(LED_LOG_DEBUG, "D: " fmt"\r\n", ##__VA_ARGS__)    /* 用于表示发生了错误 */


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
static bool led_init(led_driver_t * self);
static void led_on(led_driver_t * self);
static void led_off(led_driver_t * self);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_led_link(led_driver_t * drv, const led_handle_t * handle)
{
    ASSERT_NULL(drv);
    ASSERT_NULL(handle);

    drv->handle   = handle;

    drv->pf_init   = led_init;
    drv->pf_on     = led_on;
    drv->pf_off    = led_off;
}

#if LED_DEBUG_ENABLE
void bsp_driver_led_log_init(pf_printf_t cb, led_log_level_t level)
{
    printf_cb = cb;
    log_level = level;
}
#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool led_init(led_driver_t * self)
{
    ASSERT_NULL(self->handle->pf_on);
    ASSERT_NULL(self->handle->pf_off);
    ASSERT_NULL(self->handle->pf_gpio_init);

    self->handle->pf_gpio_init();

    LOG_I("led init successfully");

    return true;
}

static void led_on(led_driver_t * self)
{
    self->handle->pf_on();
}

static void led_off(led_driver_t * self)
{
    self->handle->pf_off();
}

/******************************* (END OF FILE) *********************************/
