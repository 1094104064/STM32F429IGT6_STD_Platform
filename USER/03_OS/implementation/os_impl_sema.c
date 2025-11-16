/**
  ******************************************************************************
  *
  * @file    template.c
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
#include "os_impl_sema.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
#ifdef USING_FREERTOS

int32_t os_impl_sema_binary_create(os_semaphore_handle_t * handle)
{
    int32_t ret;
    SemaphoreHandle_t sema_handle;
    sema_handle = xSemaphoreCreateBinary();

    if(sema_handle == NULL) {
        ret = OS_FAIL;
    }
    else {
        ret = OS_SUCCESS;
        *handle = sema_handle;
    }
    return ret;
}

int32_t os_impl_sema_counting_create(os_semaphore_handle_t * handle, uint32_t max_count, uint32_t init_count)
{
    int32_t ret;
    SemaphoreHandle_t sema_handle;
    sema_handle = xSemaphoreCreateCounting(max_count, init_count);

    if(sema_handle == NULL) {
        ret = OS_FAIL;
    }
    else {
        ret = OS_SUCCESS;
        *handle = sema_handle;
    }
    return ret;
}

void os_impl_sema_delete(os_semaphore_handle_t handle)
{
    vSemaphoreDelete(handle);
}

int32_t os_impl_sema_give(os_semaphore_handle_t handle)
{
    int32_t ret;
    BaseType_t status;
    SemaphoreHandle_t sema_handle = (SemaphoreHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        status = xSemaphoreGiveFromISR(sema_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        status = xSemaphoreGive(sema_handle);
    }

    ret = (status == pdTRUE) ? OS_SUCCESS : OS_FAIL;

    return ret;
}

int32_t os_impl_sema_take(os_semaphore_handle_t handle, uint32_t timeout)
{
    int32_t ret;
    BaseType_t status;
    SemaphoreHandle_t sema_handle = (SemaphoreHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        status = xSemaphoreTakeFromISR(sema_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        status = xSemaphoreTake(sema_handle, timeout);
    }

    ret = (status == pdTRUE) ? OS_SUCCESS : OS_FAIL;

    return ret;
}

#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 








