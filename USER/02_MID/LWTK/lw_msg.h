/**
  ******************************************************************************
  *
  * @file    lw_msg.h
  * @author  
  * @brief   Header file of message module.
  *
  ******************************************************************************
  **/
  
#ifndef _LW_MSG_H
#define _LW_MSG_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define LW_OBJ_MAX_MSG_CNT   (32)
/**********************
 *      TYPEDEFS
 **********************/
typedef struct lw_msg lw_msg_t;
typedef struct lw_msg_obj lw_msg_obj_t;
typedef void (* lw_msg_cb_t)(lw_msg_t * self);

struct lw_msg_obj
{
    lw_msg_t * msg[LW_OBJ_MAX_MSG_CNT];
    uint32_t   msg_cnt;
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void        lw_msg_init                 (lw_msg_t * msg_buf, uint32_t buf_size);
lw_msg_t *  lw_msg_subscribe            (uint32_t msg_id, lw_msg_cb_t callback, void * user_data);
void        lw_msg_unsubscribe          (lw_msg_t * msg);
void        lw_msg_publish              (uint32_t msg_id);
void        lw_msg_obj_add_event        (lw_msg_obj_t * obj, uint32_t msg_id, lw_msg_cb_t callback, void * user_data);
void        lw_msg_obj_remove_event     (lw_msg_obj_t * obj, lw_msg_cb_t callback);
void        lw_msg_obj_remove_all_event (lw_msg_obj_t * obj);
void        lw_msg_obj_send_event       (lw_msg_obj_t * obj, uint32_t msg_id);
uint32_t    lw_msg_get_id               (lw_msg_t * self);
void *      lw_msg_get_user_data        (lw_msg_t * self);
/**********************
 *      MACROS
 **********************/
#if 1
enum lw_msg_id
{
    LW_MSG_ID_NONE = 0,

    LW_MSG_PRESSED,
    LW_MSG_PRESSING,
    LW_MSG_RELEASED,
    LW_MSG_CLICKED,
    LW_MSG_LONG_PRESSED,
    LW_MSG_LONG_PRESSING,
    LW_MSG_LONG_PRESSING_END,
    LW_MSG_SHORT_CLICKED,
    LW_MSG_SCROLL_BEGIN,
    LW_MSG_SCROLL_END,

    LW_MSG_VIEW_CHANGED,
    LW_MSG_PAGR_ENTER,
    LW_MSG_PAGR_EXIT,
    LW_MSG_PAGE_CHANGED,
    LW_MSG_SCREEN_CHANGED,

    LW_MSG_SYS_BOOT,
    LW_MSG_SYS_SHUTDOWN,
    LW_MSG_SYS_RESET,
    LW_MSG_SYS_SLEEP,
    LW_MSG_SYS_WAKEUP,
    LW_MSG_NETWORK_CONNECTED,
    LW_MSG_NETWORK_DISCONNECTED,
    LW_MSG_BATTERY_LOW,
    LW_MSG_BATTERY_FULL,
    LW_MSG_BATTERY_CHARGED,

    LW_MSG_DATA_READY,
    LW_MSG_DATA_RECEIVED,
    LW_MSG_DATA_SENT,
    LW_MSG_DATA_UPDATED,
    LW_MSG_DATA_ERROR,

    LW_MSG_SENSOR_READY,
    LW_MSG_SENSOR_UPDATED,
    LW_MSG_SENSOR_ERROR,

    LW_MSG_LANGUAGE_CHANGED,


    LW_MSG_ID_MAX,
};
#endif




#ifdef __cplusplus
}
#endif

#endif /*_LW_MSG_H*/


/******************************* (END OF FILE) *********************************/


