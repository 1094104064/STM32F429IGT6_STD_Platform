/**
  ******************************************************************************
  *
  * @file    bsp_adapter_led.h
  * @author  Jamin
  * @brief   Header file of bsp_adapter_led module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_ADAPTER_LED_H
#define _BSP_ADAPTER_LED_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "bsp_wrapper_light.h"
/*********************
 *      DEFINES
 *********************/
#define BLUE_LED_NAME         "blueled"
#define YELLOW_LED_NAME       "yellowled"
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void InitLED(void);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_ADAPTER_LED_H*/


/******************************* (END OF FILE) *********************************/

