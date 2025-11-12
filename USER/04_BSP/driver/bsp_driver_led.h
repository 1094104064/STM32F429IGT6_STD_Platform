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
#define LED_DEBUG_ENABLE 0

#if LED_DEBUG_ENABLE

    #define led_dbg(fmt, ...)         printf("%s [%d] : " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

    #define led_assert_null(param)                                                              \
            do {                                                                                \
                if(param == NULL) { led_dbg("NULL pointer: %s", #param); while(1); }            \
            } while (0)    
#else
    #define led_dbg(fmt, ...)             do {} while (0)
    #define led_assert_null(param)        do {} while (0)
#endif





/**********************
 *      TYPEDEFS
 **********************/

typedef struct led_handle led_handle_t;
typedef struct led_driver led_driver_t;

struct led_handle 
{
    void (* pf_hal_init)(void);
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
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_LED_H*/


/******************************* (END OF FILE) *********************************/

