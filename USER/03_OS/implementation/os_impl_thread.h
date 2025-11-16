/**
  ******************************************************************************
  *
  * @file    os_impl_thread.h
  * @author  
  * @brief   Header file of os_impl_thread module.
  *
  ******************************************************************************
  **/
  
#ifndef _OS_IMPL_THREAD_H
#define _OS_IMPL_THREAD_H


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
#include "task.h"

typedef TaskFunction_t os_thread_pf;
typedef TaskHandle_t os_thread_handle_t;

typedef struct thread_ctx thread_ctx_t;
struct thread_ctx 
{
    os_thread_pf pf;
    const char *name;
    uint32_t stack_size;
    uint32_t priority;
    void *arg;
    os_thread_handle_t * handle;
} ;


#endif

/*********************
 *      RT-THREAD
 *********************/
 

 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
int32_t os_impl_thread_create(thread_ctx_t * ctx);
void os_impl_thread_delete(os_thread_handle_t handle);
void os_impl_scheduler_start(void);
void os_impl_thread_suspend_all(void);
void os_impl_thread_resume(os_thread_handle_t handle);
void os_impl_thread_delay(uint32_t ticks);
void os_impl_thread_enter_critical(void);
void os_impl_thread_exit_critical(void);
void os_impl_thread_yield(void);
void os_impl_thread_disable_interrupts(void);
void os_impl_thread_enable_interrupts(void);
uint32_t os_impl_thread_get_tick_count(void);

#ifdef __cplusplus
}
#endif

#endif /*_OS_IMPL_THREAD_H*/


/******************************* (END OF FILE) *********************************/


