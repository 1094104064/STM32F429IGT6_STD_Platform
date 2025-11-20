/**
  ******************************************************************************
  *
  * @file    os_impl_mutex.c
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
#include "os_impl_mutex.h"
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

int32_t os_impl_mutex_create(os_mutex_handle_t * handle)
{
    int32_t ret;
    QueueHandle_t mutex_handle = xSemaphoreCreateMutex();

    if(mutex_handle == NULL) {
        ret = OS_FAIL;
    }
    else {
        ret = OS_SUCCESS;
        *handle = (os_mutex_handle_t)mutex_handle;
    }
    
    return ret;
}

void os_impl_mutex_delete(os_mutex_handle_t handle)
{
    vSemaphoreDelete((SemaphoreHandle_t)handle);
}

int32_t os_impl_mutex_give(os_mutex_handle_t handle)
{
    int32_t ret;
    BaseType_t status;
    QueueHandle_t mutex_handle = (QueueHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        status = xSemaphoreGiveFromISR(mutex_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        status = xSemaphoreGive(mutex_handle);
    }

    ret = (status == pdTRUE) ? OS_SUCCESS : OS_FAIL;

    return ret;
}

int32_t os_impl_mutex_take(os_mutex_handle_t handle, uint32_t timeout)
{
    int32_t ret;
    BaseType_t status;
    QueueHandle_t mutex_handle = (QueueHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        status = xSemaphoreTakeFromISR(mutex_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        status = xSemaphoreTake(mutex_handle, timeout);
    }

    ret = (status == pdTRUE) ? OS_SUCCESS : OS_FAIL;

    return ret;
}
#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







