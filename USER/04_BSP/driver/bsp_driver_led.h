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
#include "user_conf.h"
/*********************
 *      DEFINES
 *********************/

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

