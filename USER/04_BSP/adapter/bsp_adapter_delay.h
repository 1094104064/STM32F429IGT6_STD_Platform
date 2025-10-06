/**
  ******************************************************************************
  *
  * @file    bsp_adapter_delay.h
  * @author  Jamin
  * @brief   Header file of bsp_adapter_delay module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_ADAPTER_DELAY_H
#define _BSP_ADAPTER_DELAY_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "user_conf.h"
#include "bsp_wrapper_delay.h"
#include "bsp_driver_delay.h"
#include "bsp_port_delay.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
delay_wrapper_t * bsp_adapter_delay_register(const char * const name, void * const user_data);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_ADAPTER_DELAY_H*/


/******************************* (END OF FILE) *********************************/

