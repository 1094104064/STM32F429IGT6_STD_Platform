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

struct led_oper {

    void (* pf_on)(void);
    void (* pf_off)(void);
};

struct led_driver {
    struct led_oper * oper;

    bool (* pf_init)(struct led_driver * self);
    void (* pf_on)(struct led_driver * self);
    void (* pf_off)(struct led_driver * self);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_led_link(struct led_driver * self, struct led_oper * oper);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_LED_H*/


/******************************* (END OF FILE) *********************************/

