/**
  ******************************************************************************
  *
  * @file    os_impl_sema.h
  * @author  
  * @brief   Header file of os_impl_sema module.
  *
  ******************************************************************************
  **/
  
#ifndef _OS_IMPL_SEMA_H
#define _OS_IMPL_SEMA_H


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
 *      FREERTOS
 *********************/
#ifdef USING_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"

typedef SemaphoreHandle_t os_semaphore_handle_t;

#endif
/**********************
*  GLOBAL PROTOTYPES
 **********************/
int32_t os_impl_sema_binary_create(os_semaphore_handle_t * handle);
int32_t os_impl_sema_counting_create(os_semaphore_handle_t * handle, uint32_t max_count, uint32_t init_count);
void os_impl_sema_delete(os_semaphore_handle_t handle);
int32_t os_impl_sema_give(os_semaphore_handle_t handle);
int32_t os_impl_sema_take(os_semaphore_handle_t handle, uint32_t timeout);







#ifdef __cplusplus
}
#endif

#endif /*_OS_IMPL_SEMA_H*/


/******************************* (END OF FILE) *********************************/


