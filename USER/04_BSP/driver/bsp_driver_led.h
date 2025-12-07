/**
  ******************************************************************************
  *
  * @file    bsp_driver_led.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_led module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_DRIVER_LED_H
#define _BSP_DRIVER_LED_H


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

#define LED_DEBUG_ENABLE 1

/**********************
 *      TYPEDEFS
 **********************/
typedef int (* pf_printf_t)(const char* format, ...);

typedef enum 
{
    LED_LOG_NONE = 0,
    LED_LOG_ERROR,
    LED_LOG_INFO,
    LED_LOG_DEBUG,
} led_log_level_t;

typedef struct led_handle led_handle_t;
typedef struct led_driver led_driver_t;

struct led_handle 
{
    void (* pf_gpio_init)(void);
    void (* pf_on)  (void);
    void (* pf_off) (void);
};

struct led_driver 
{
    const led_handle_t * handle;

    bool (* pf_init)    (led_driver_t * self);
    void (* pf_on)      (led_driver_t * self);
    void (* pf_off)     (led_driver_t * self);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_led_link(led_driver_t * drv, const led_handle_t * handle);

#if LED_DEBUG_ENABLE
void bsp_driver_led_log_init(pf_printf_t cb, led_log_level_t level);
#endif  
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_LED_H*/


/******************************* (END OF FILE) *********************************/

