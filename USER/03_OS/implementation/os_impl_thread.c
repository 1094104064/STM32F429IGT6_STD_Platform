/**
  ******************************************************************************
  *
  * @file    os_impl_thread.c
  * @author  
  * @brief   
  *
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  **/
  
  
/*********************
 *      INCLUDES
 *********************/
#include "os_impl_thread.h"
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/*********************
 *      FREERTOS
 *********************/
#ifdef USING_FREERTOS

int32_t os_impl_thread_create(thread_ctx_t * ctx)
{
    int32_t ret = OS_SUCCESS;

    BaseType_t error_code;
    error_code = xTaskCreate(ctx->pf, 
                             ctx->name, 
                             ctx->stack_size / sizeof(StackType_t),
                             ctx->arg,
                             ctx->priority,
                             ctx->handle);
                             
    if (pdPASS != error_code) {
        ret = OS_FAIL;
    }
    return ret;
}

void os_impl_thread_delete(os_thread_handle_t handle)
{
    vTaskDelete(handle);
}

void os_impl_scheduler_start(void)
{
    vTaskStartScheduler();
}

void os_impl_thread_suspend_all(void)
{
    vTaskSuspendAll();
}

void os_impl_thread_resume(os_thread_handle_t handle)
{
    if(pdTRUE == xPortIsInsideInterrupt()) {
        xTaskResumeFromISR(handle);
    }
    else {
        vTaskResume(handle);
    }
}

void os_impl_thread_delay(uint32_t ticks)
{
    vTaskDelay(ticks);
}

void os_impl_thread_enter_critical(void)
{
    if(pdTRUE == xPortIsInsideInterrupt()) {
        taskENTER_CRITICAL_FROM_ISR();
    }
    else {
        taskENTER_CRITICAL();
    }
}

void os_impl_thread_exit_critical(void)
{
    if(pdTRUE == xPortIsInsideInterrupt()) {
        taskEXIT_CRITICAL_FROM_ISR(0);
    }
    else {
        taskEXIT_CRITICAL();
    }
}

void os_impl_thread_yield(void)
{
    if(pdTRUE == xPortIsInsideInterrupt()) {
        portYIELD_FROM_ISR(0);
    }
    else {
        portYIELD();
    }
}

void os_impl_thread_disable_interrupts(void)
{
    taskDISABLE_INTERRUPTS();
}

void os_impl_thread_enable_interrupts(void)
{
    taskENABLE_INTERRUPTS();
}   

uint32_t os_impl_thread_get_tick_count(void)
{
    return xTaskGetTickCount();
}

#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







