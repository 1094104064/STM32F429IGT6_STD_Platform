/**
  ******************************************************************************
  *
  * @file    main.h
  * @author  Jamin
  * @brief   Header file of template module.
  *
  ******************************************************************************
  **/
  
#ifndef _MAIN_H
#define _MAIN_H


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
#include <stdarg.h>
#include <stddef.h>
#include <math.h>
#include "stm32f4xx.h"
#include "user_macros.h"
#include "user_conf.h"

#include "usart.h"
#include "systick.h"
#include "gpio.h"
#include "fmc.h"
#include "ltdc.h"
#include "spi.h"
#include "dwt.h"
#include "dma.h"
#include "dma2d.h"

#include "bsp_adapter_delay.h"
#include "bsp_wrapper_delay.h"

#include "bsp_adapter_light.h"
#include "bsp_wrapper_light.h"

#include "bsp_adapter_display.h"
#include "bsp_wrapper_display.h"

#include "bsp_adapter_touchpad.h"
#include "bsp_wrapper_touchpad.h"

#include "bsp_adapter_sflash.h"
#include "bsp_wrapper_sflash.h"

#include "elog.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_MAIN_H*/


/******************************* (END OF FILE) *********************************/
