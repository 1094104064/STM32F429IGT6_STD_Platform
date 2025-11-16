/**
  ******************************************************************************
  *
  * @file    os_impl_queue.h
  * @author  
  * @brief   Header file of os_impl_queue module.
  *
  ******************************************************************************
  **/
  
#ifndef _OS_IMPL_QUEUE_H
#define _OS_IMPL_QUEUE_H


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
#include "queue.h"

typedef QueueHandle_t os_queue_handle_t;


#endif

 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
int32_t os_impl_queue_create(size_t queue_length, size_t item_size, os_queue_handle_t * handle);
void os_impl_queue_delete(os_queue_handle_t handle);
int32_t os_impl_queue_send(os_queue_handle_t handle, const void * data, uint32_t timeout);
int32_t os_impl_queue_receive(os_queue_handle_t handle, void * data, uint32_t timeout);
int32_t os_impl_queue_waiting(os_queue_handle_t handle);







#ifdef __cplusplus
}
#endif

#endif /*_OS_IMPL_QUEUE_H*/


/******************************* (END OF FILE) *********************************/


