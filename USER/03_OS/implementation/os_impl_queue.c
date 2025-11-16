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
#include "os_impl_queue.h"
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

/*********************
 *      FREERTOS
 *********************/
#ifdef USING_FREERTOS

int32_t os_impl_queue_create(size_t queue_length, size_t item_size, os_queue_handle_t * handle)
{
    int32_t ret;
    QueueHandle_t queue_handle;
    queue_handle = xQueueCreate(queue_length, item_size);

    if (queue_handle == NULL) {
        ret = OS_FAIL;
    }
    else {
        *handle = (os_queue_handle_t)queue_handle;
        int32_t num = uxQueueMessagesWaiting(queue_handle);
        ret = OS_SUCCESS;
    }
    return ret;
}

void os_impl_queue_delete(os_queue_handle_t handle)
{
    vQueueDelete(handle);
}

int32_t os_impl_queue_send(os_queue_handle_t handle, const void * data, uint32_t timeout)
{
    int32_t ret;
    BaseType_t status;
    QueueHandle_t queue_handle = (QueueHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        status = xQueueSendFromISR(queue_handle, data, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }   
    else {
        status = xQueueSend(queue_handle, data, timeout);
    } 

    if(status == pdPASS) {
        ret = OS_SUCCESS;
    }
    else if(status == errQUEUE_FULL) {
        ret = OS_FAIL;
    }
    else {
        ret = OS_FAIL;
    }

    return ret;
}

int32_t os_impl_queue_receive(os_queue_handle_t handle, void * data, uint32_t timeout)
{
    int32_t ret;
    BaseType_t status;
    QueueHandle_t queue_handle = (QueueHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        status = xQueueReceiveFromISR(queue_handle, data, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        status = xQueueReceive(queue_handle, data, timeout);
    }

    if(status == pdPASS) {
        ret = OS_SUCCESS;
    }
    else {
        ret = OS_FAIL;
    }
    return ret;
}

int32_t os_impl_queue_waiting(os_queue_handle_t handle)
{
    int32_t ret;
    QueueHandle_t queue_handle = (QueueHandle_t)handle;

    if(pdTRUE == xPortIsInsideInterrupt()) {
        ret = uxQueueMessagesWaitingFromISR(queue_handle);
    }
    else {
        ret = uxQueueMessagesWaiting(queue_handle);
    }
    return ret;
}



#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







