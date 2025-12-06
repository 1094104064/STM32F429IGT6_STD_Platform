/**
  ******************************************************************************
  *
  * @file    lw_btn.h
  * @author  
  * @brief   Header file of lw_btn module.
  *
  ******************************************************************************
  **/
  
#ifndef _LW_BTN_H
#define _LW_BTN_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/
#define LW_BTN_MEMCPY(dst, src, size) memcpy(dst, src, size)
#define LW_BTN_MEMSET(buf, value, size) memset(buf, value, size)
#define LW_BTN_MEMMOVE(dst, src, size) memmove(dst, src, size)

#define LW_BTN_EVENT_MAX                    32
#define LW_BTN_DEBOUNCE_PERIOD_MS           20
#define LW_BTN_DOUBLE_CLICK_PERIOD_MS       300
#define LW_BTN_LONG_PRESS_PERIOD_MS         1000
#define LW_BTN_LONG_PRESS_REPEAT_PERIOD_MS  200
#define LW_BTN_LONG_LONG_PRESS_PERIOD_MS    3000
/**********************
 *      TYPEDEFS
 **********************/
/* Button event types */
typedef enum {
    LW_BTN_EVENT_PRESSED    = 0,
    LW_BTN_EVENT_PRESSING,
    LW_BTN_EVENT_RELEASED,
    LW_BTN_EVENT_CLICKED,
    LW_BTN_EVENT_SHORT_CLICKED,
    LW_BTN_EVENT_DOUBLE_CLICKED,
    LW_BTN_EVENT_LONG_PRESSED,
    LW_BTN_EVENT_LONG_PRESSING,
    LW_BTN_EVENT_LONG_PRESSING_END,
    LW_BTN_EVENT_LONG_LONG_PRESSED,
    LW_BTN_EVENT_LONG_LONG_PRESSING_END,
    LW_BTN_EVENT_ALL
} lw_btn_event_t;

typedef enum {
    LW_BTN_STATE_IDLE    = 0,
    LW_BTN_STATE_PRESS,
    LW_BTN_STATE_RELEASE,
    LW_BTN_STATE_LONG_PRESS,
    LW_BTN_STATE_LONG_LONG_PRESS,
} lw_btn_state_t;

typedef struct lw_btn_event_dsc lw_btn_event_dsc_t;

typedef void (* lw_btn_event_cb_t)(lw_btn_event_dsc_t * event_dsc);

struct lw_btn_event_dsc
{
    bool stop_processing;
    lw_btn_event_t code;
    lw_btn_event_cb_t event_cb;
    void * param;
};


/* Button structure */
typedef struct lw_btn lw_btn_t;

struct lw_btn
{
    struct lw_btn * next;

    bool        paused;
    bool        current_pressed;
    uint32_t    last_tick;  
    uint32_t    press_time;                                 
    uint32_t    debounce_tick;

    uint32_t    last_click_tick;
    uint32_t    click_tick;

    uint8_t     long_press_repeat_cnt;
    uint32_t    long_press_repeat_period;

    bool        long_long_press_enable;
    uint32_t    long_long_press_time;

	uint8_t     button_id;                
	bool        (* pf_is_pressed)(uint8_t button_id);  

    lw_btn_event_t event;
    lw_btn_state_t state;

    lw_btn_event_dsc_t event_dsc[LW_BTN_EVENT_MAX];
    uint32_t event_dsc_cnt;
                 
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/

static inline lw_btn_event_t lw_btn_get_event_code(lw_btn_event_dsc_t * event_dsc)
{
    return event_dsc->code;
}


static inline bool lw_btn_is_pressed(lw_btn_t * btn)
{
    return btn->pf_is_pressed(btn->button_id);
}

void        lw_btn_init                 (lw_btn_t * btn_buf, uint32_t buf_size);
lw_btn_t *  lw_btn_create               (uint8_t button_id, bool (*pf_is_pressed)(uint8_t button_id));
void        lw_btn_delete               (lw_btn_t * btn);
bool        lw_btn_add_event            (lw_btn_t * btn, lw_btn_event_t code, lw_btn_event_cb_t event_cb, void * param);
bool        lw_btn_remove_event         (lw_btn_t * btn, lw_btn_event_cb_t event_cb);
void        lw_btn_pause                (lw_btn_t * btn);
void        lw_btn_resume               (lw_btn_t * btn);
void        lw_btn_reset                (lw_btn_t * btn);
void        lw_btn_handler              (void);
void        lw_btn_increase_tick        (uint32_t tick_period);
void        lw_btn_set_stop_processing  (lw_btn_t * btn, lw_btn_event_cb_t event_cb, bool stop_processing);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_LW_BTN_H*/


/******************************* (END OF FILE) *********************************/


