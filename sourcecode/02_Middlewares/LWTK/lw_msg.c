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

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lw_msg_global_t * alloc_msg(void);
static void free_msg(lw_msg_global_t * msg);
/**********************
 *  STATIC VARIABLES
 **********************/
static lw_msg_global_t * gs_mempool = NULL;
static lw_msg_global_t * free_list = NULL;     
static lw_msg_global_t * used_list = NULL; 
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
  * @brief  Initialize the message queue.
  * @param  msg_buf: Pointer to the message buffer.
  * @param  buf_size: Size of the message buffer in bytes.
  * @retval None
  */
void lw_msg_global_init(lw_msg_global_t * msg_buf, uint32_t buf_size)
{
    gs_mempool = msg_buf;
    uint32_t msg_count = buf_size / sizeof(lw_msg_global_t);

    LW_MSG_MEMSET(gs_mempool, 0, buf_size);

    free_list = NULL;
    used_list = NULL;

    for(uint32_t i = 0; i < msg_count; i++){
        gs_mempool[i].next = free_list;
        free_list = &gs_mempool[i];
    }
}

/**
  * @brief  Subscribe to a message ID.
  * @param  msg_id: ID of the message to subscribe to.
  * @param  callback: Callback function to be called when the message is published.
  * @param  user_data: User data to be passed to the callback function.
  * @retval Pointer to the allocated message structure if successful, NULL otherwise.
  */
lw_msg_global_t * lw_msg_global_subscribe(uint32_t msg_id, lw_msg_cb_t callback, void * user_data)
{
    lw_msg_global_t * self = alloc_msg();
    if(self == NULL) {
        return NULL;
    }

    self->msg.id = msg_id;
    self->msg.callback = callback;
    self->msg.user_data = user_data;

    return self;
}

/**
  * @brief  Unsubscribe from a message ID.
  * @param  self: Pointer to the message structure to unsubscribe from.
  * @retval None
  */
void lw_msg_global_unsubscribe(lw_msg_global_t * self)
{
    if(self)
        free_msg(self);
}

/**
  * @brief  Publish a message ID.
  * @param  msg_id: ID of the message to publish.
  * @retval None
  */
void lw_msg_global_publish(uint32_t msg_id)
{
    lw_msg_global_t * self = used_list;
    while(self) {
        if(self->msg.id == msg_id && self->msg.callback != NULL) {
            self->msg.callback(&self->msg);
        }
        self = self->next;
    }
}

/**
  * @brief  Initialize a message object.
  * @param  obj: Pointer to the message object structure.
  * @retval None
  */
void lw_msg_object_init(lw_msg_object_t * obj)
{
    LW_MSG_MEMSET(obj, 0, sizeof(lw_msg_object_t));
}

/**
  * @brief  Add an event to a message object.
  * @param  obj: Pointer to the message object structure.
  * @param  msg_id: ID of the message to subscribe to.
  * @param  callback: Callback function to be called when the message is published.
  * @param  user_data: User data to be passed to the callback function.
  * @retval None
  */
void lw_msg_object_add_event(lw_msg_object_t * obj, uint32_t msg_id, lw_msg_cb_t callback, void * user_data)
{
    if(obj->msg_cnt >= LW_OBJ_MAX_MSG_CNT) {
        return;
    }

    obj->msg[obj->msg_cnt].id = msg_id;
    obj->msg[obj->msg_cnt].callback = callback;
    obj->msg[obj->msg_cnt].user_data = user_data;

    obj->msg_cnt++;
}

/**
  * @brief  Remove an event from a message object.
  * @param  obj: Pointer to the message object structure.
  * @param  callback: Callback function to be removed.
  * @retval None
  */
void lw_msg_object_remove_event(lw_msg_object_t * obj, lw_msg_cb_t callback)
{
    uint32_t write_idx = 0;

    for (uint32_t read_idx = 0; read_idx < obj->msg_cnt; read_idx++) {
        if (obj->msg[read_idx].callback != callback) {
            if (write_idx != read_idx) {
                obj->msg[write_idx] = obj->msg[read_idx];
            }
            write_idx++;
        }
    }

    obj->msg_cnt = write_idx;
}

/**
  * @brief  Remove all events from a message object.
  * @param  obj: Pointer to the message object structure.
  * @retval None
  */
void lw_msg_object_remove_all_event(lw_msg_object_t * obj)
{
    LW_MSG_MEMSET(obj->msg, 0, sizeof(lw_msg_t) * LW_OBJ_MAX_MSG_CNT);
    obj->msg_cnt = 0;
}

/**
  * @brief  Send an event to a message object.
  * @param  obj: Pointer to the message object structure.
  * @param  msg_id: ID of the message to send.
  * @retval None
  */
void lw_msg_object_send_event(lw_msg_object_t * obj, uint32_t msg_id)
{
    for(uint32_t i = 0; i < obj->msg_cnt; i++) {
        if(obj->msg[i].id == msg_id && obj->msg[i].callback != NULL) {
            obj->msg[i].callback(&obj->msg[i]);
        }
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
  * @brief  Allocate a message structure.
  * @param  None
  * @retval Pointer to the allocated message structure if successful, NULL otherwise.
  */
static lw_msg_global_t * alloc_msg(void)
{
    if(!free_list)
        return NULL;

    lw_msg_global_t * self = free_list;
    free_list = free_list->next;

    self->next = used_list;
    used_list = self;

    return self;
}

/**
  * @brief  Free a message structure.
  * @param  self: Pointer to the message structure to free.
  * @retval None
  */
static void free_msg(lw_msg_global_t * self)
{
    lw_msg_global_t ** pp = &used_list;

    while(*pp) {
        if(*pp == self) {
            *pp = self->next;   
            break;
        }
        pp = &((*pp)->next);
    }

    self->next = free_list;
    free_list = self;
}


/******************************* (END OF FILE) *********************************/



 







