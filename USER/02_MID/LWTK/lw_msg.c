/**
  ******************************************************************************
  *
  * @file    lw_msg.c
  * @author  
  * @brief   source file of message module.
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
#include "lw_msg.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

struct lw_msg
{
    bool        is_used;
    uint32_t    id;
    lw_msg_cb_t callback;
    void *      user_data;
};
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lw_msg_t * alloc_msg(void);
static void free_msg(lw_msg_t * msg);
/**********************
 *  STATIC VARIABLES
 **********************/
static lw_msg_t * msg_queue = NULL;
static uint32_t msg_queue_items = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
  * @brief  Initialize the message queue.
  * @param  msg_buf: Pointer to the message buffer.
  * @param  buf_size: Size of the message buffer in bytes.
  * @retval None
  */
void lw_msg_init(lw_msg_t * msg_buf, uint32_t buf_size)
{
    msg_queue = msg_buf;
    msg_queue_items = buf_size / sizeof(lw_msg_t);

    memset(msg_queue, 0, buf_size);
}

/**
  * @brief  Subscribe to a message ID.
  * @param  msg_id: ID of the message to subscribe to.
  * @param  callback: Callback function to be called when the message is published.
  * @param  user_data: User data to be passed to the callback function.
  * @retval Pointer to the allocated message structure if successful, NULL otherwise.
  */
lw_msg_t * lw_msg_subscribe(uint32_t msg_id, lw_msg_cb_t callback, void * user_data)
{
    lw_msg_t * msg = alloc_msg();
    if(msg == NULL) {
        return NULL;
    }
    msg->is_used    = true;
    msg->id         = msg_id;
    msg->callback   = callback;
    msg->user_data  = user_data;

    return msg;
}

/**
  * @brief  Unsubscribe from a message ID.
  * @param  msg: Pointer to the message structure to unsubscribe from.
  * @retval None
  */
void lw_msg_unsubscribe(lw_msg_t * msg)
{
    if(msg)
        free_msg(msg);
}

/**
  * @brief  Publish a message ID.
  * @param  msg_id: ID of the message to publish.
  * @retval None
  */
void lw_msg_publish(uint32_t msg_id)
{
    for(uint32_t i = 0; i < msg_queue_items; i++) {
        if(!msg_queue[i].is_used) {
            continue;
        }

        if(msg_queue[i].id == msg_id && msg_queue[i].callback != NULL) {
            msg_queue[i].callback(&msg_queue[i]);
        }
    }
}

/**
  * @brief  Add an event to a message object.
  * @param  obj: Pointer to the message object structure.
  * @param  msg_id: ID of the message to subscribe to.
  * @param  callback: Callback function to be called when the message is published.
  * @param  user_data: User data to be passed to the callback function.
  * @retval None
  */
void lw_msg_obj_add_event(lw_msg_obj_t * obj, uint32_t msg_id, lw_msg_cb_t callback, void * user_data)
{
    if(obj->msg_cnt >= LW_OBJ_MAX_MSG_CNT) {
        return;
    }

    obj->msg[obj->msg_cnt] = lw_msg_subscribe(msg_id, callback, user_data);
    obj->msg_cnt++;
}

/**
  * @brief  Remove an event from a message object.
  * @param  obj: Pointer to the message object structure.
  * @param  callback: Callback function to be removed.
  * @retval None
  */
void lw_msg_obj_remove_event(lw_msg_obj_t * obj, lw_msg_cb_t callback)
{
    for(uint32_t i = 0; i < obj->msg_cnt; i++) {
        if(obj->msg[i]->callback == callback) {
            lw_msg_unsubscribe(obj->msg[i]);

            for(uint32_t j = i; j < obj->msg_cnt - 1; j++) {
                obj->msg[j] = obj->msg[j + 1];
            }
            obj->msg_cnt--;
        }
    }
}

/**
  * @brief  Remove all events from a message object.
  * @param  obj: Pointer to the message object structure.
  * @retval None
  */
void lw_msg_obj_remove_all_event(lw_msg_obj_t * obj)
{
    for(uint32_t i = 0; i < obj->msg_cnt; i++) {
        lw_msg_unsubscribe(obj->msg[i]);
    }
    obj->msg_cnt = 0;
}

/**
  * @brief  Send an event to a message object.
  * @param  obj: Pointer to the message object structure.
  * @param  msg_id: ID of the message to send.
  * @retval None
  */
void lw_msg_obj_send_event(lw_msg_obj_t * obj, uint32_t msg_id)
{
    for(uint32_t i = 0; i < obj->msg_cnt; i++) {
        if(obj->msg[i]->id == msg_id && obj->msg[i]->callback != NULL) {
            obj->msg[i]->callback(obj->msg[i]);
        }
    }
}

/**
  * @brief  Get the ID of a message.
  * @param  self: Pointer to the message structure.
  * @retval ID of the message.
  */
uint32_t lw_msg_get_id(lw_msg_t * self)
{
    return self->id;
}

/**
  * @brief  Get the user data of a message.
  * @param  self: Pointer to the message structure.
  * @retval User data of the message.
  */
void * lw_msg_get_user_data(lw_msg_t * self)
{
    return self->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
  * @brief  Allocate a message structure.
  * @param  None
  * @retval Pointer to the allocated message structure if successful, NULL otherwise.
  */
static lw_msg_t * alloc_msg(void)
{
    for(uint32_t i = 0; i < msg_queue_items; i++) {
        if(msg_queue[i].is_used == false) {
            msg_queue[i].is_used = true;
            return &msg_queue[i];
        }
    }
    return NULL;  /*没有空间*/
}

/**
  * @brief  Free a message structure.
  * @param  msg: Pointer to the message structure to free.
  * @retval None
  */
static void free_msg(lw_msg_t * msg)
{
    uint32_t i = msg - msg_queue;
    if(i < msg_queue_items) {
        msg_queue[i].is_used = false;
    }
}


/******************************* (END OF FILE) *********************************/



 







