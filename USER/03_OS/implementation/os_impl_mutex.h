/**
  ******************************************************************************
  *
  * @file    os_impl_mutex.h
  * @author  
  * @brief   Header file of os_impl_mutex module.
  *
  ******************************************************************************
  **/
  
#ifndef _OS_IMPL_MUTEX_H
#define _OS_IMPL_MUTEX_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
#include "user_conf.h"
#include "os_macro.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
#ifdef USING_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"

typedef QueueHandle_t os_mutex_handle_t;

#endif

/**********************
*  GLOBAL PROTOTYPES
 **********************/
int32_t os_impl_mutex_create(os_mutex_handle_t * handle);
void os_impl_mutex_delete(os_mutex_handle_t handle);
int32_t os_impl_mutex_give(os_mutex_handle_t handle);
int32_t os_impl_mutex_take(os_mutex_handle_t handle, uint32_t timeout);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_OS_IMPL_MUTEX_H*/


/******************************* (END OF FILE) *********************************/


