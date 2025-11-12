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

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
extern const light_ops_t blue_led_ops;
extern const light_ops_t yellow_led_ops;
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_ADAPTER_LED_H*/


/******************************* (END OF FILE) *********************************/

