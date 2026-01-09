/**
  ******************************************************************************
  *
  * @file    lw_btn.c
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
#include "lw_btn.h"
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
static lw_btn_t * alloc_btn(void);
static void free_btn(lw_btn_t * btn);
static void process_button_event(lw_btn_t * btn);
/**********************
 *  STATIC VARIABLES
 **********************/
static volatile uint32_t gsul_btn_tick = 0;

static lw_btn_t * gs_mempool = NULL;
static lw_btn_t * free_list = NULL;     
static lw_btn_t * used_list = NULL;     
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
  * @brief  Initialize the button module.
  * @param  btn_buf: Pointer to the button buffer.
  * @param  buf_size: Size of the button buffer in bytes.
  */
void lw_btn_init(lw_btn_t * btn_buf, uint32_t buf_size)
{
    gs_mempool = btn_buf;
    uint32_t btn_count = buf_size / sizeof(lw_btn_t);

    LW_BTN_MEMSET(gs_mempool, 0, buf_size);

    free_list = NULL;
    used_list = NULL;

    for(uint32_t i = 0; i < btn_count; i++){
        gs_mempool[i].next = free_list;
        free_list = &gs_mempool[i];
    }
}

/**
  * @brief  Create a new button instance.
  * @param  button_id: ID of the button.
  * @param  pf_is_pressed: Function pointer to check if the button is pressed.
  * @return Pointer to the created button instance, or NULL if allocation fails.
  */
lw_btn_t * lw_btn_create(uint8_t button_id, bool (* pf_is_pressed)(uint8_t button_id))
{
    lw_btn_t * btn = alloc_btn();
    if(btn == NULL) {
        return NULL;
    }

    btn->button_id = button_id;
    btn->pf_is_pressed = pf_is_pressed;
    btn->paused = false;
    btn->current_pressed = false;
    btn->last_tick = 0;
    btn->press_time = 0;
    btn->debounce_tick = 0;
    btn->last_click_tick = 0;
    btn->click_tick = 0;
    btn->long_press_repeat_cnt = 0;
    btn->long_press_repeat_period = LW_BTN_LONG_PRESS_REPEAT_PERIOD_MS;
    btn->long_long_press_enable = false;
    btn->long_long_press_time = 0;
    btn->state = LW_BTN_STATE_IDLE;
    btn->event_dsc_cnt = 0;
    LW_BTN_MEMSET(btn->event_dsc, 0, sizeof(btn->event_dsc));

    return btn;
}

/**
  * @brief  Delete a button instance.
  * @param  btn: Pointer to the button instance to delete.
  */
void lw_btn_delete(lw_btn_t * btn)
{
    if(btn)
        free_btn(btn);
}

/**
  * @brief  Add an event callback to a button instance.
  * @param  btn: Pointer to the button instance.
  * @param  code: Event code to associate with the callback.
  * @param  event_cb: Function pointer to the event callback.
  * @param  param: Optional parameter to pass to the callback.
  * @return True if the event callback was added successfully, false otherwise.
  */
bool lw_btn_add_event(lw_btn_t * btn, lw_btn_event_t code, lw_btn_event_cb_t event_cb, void * param)
{
    if(btn->event_dsc_cnt >= LW_BTN_EVENT_MAX)
        return false;

    btn->event_dsc[btn->event_dsc_cnt].code = code;
    btn->event_dsc[btn->event_dsc_cnt].event_cb = event_cb;
    btn->event_dsc[btn->event_dsc_cnt].param = param;
    btn->event_dsc_cnt++;
    return true;
}

/**
  * @brief  Remove an event callback from a button instance.
  * @param  btn: Pointer to the button instance.
  * @param  event_cb: Function pointer to the event callback to remove.
  * @return True if the event callback was removed successfully, false otherwise.
  */
bool lw_btn_remove_event(lw_btn_t * btn, lw_btn_event_cb_t event_cb)
{
    uint32_t write_idx = 0;

    for (uint32_t read_idx = 0; read_idx < btn->event_dsc_cnt; read_idx++) {
        if (btn->event_dsc[read_idx].event_cb != event_cb) {
            if (write_idx != read_idx) {
                btn->event_dsc[write_idx] = btn->event_dsc[read_idx];
            }
            write_idx++;
        }
    }

    bool removed = (write_idx != btn->event_dsc_cnt);  
    btn->event_dsc_cnt = write_idx;

    return removed;
}

/**
  * @brief  Pause processing of events for a button instance.
  * @param  btn: Pointer to the button instance.
  */
void lw_btn_pause(lw_btn_t * btn)
{
    btn->paused = true;
}

/**
  * @brief  Resume processing of events for a button instance.
  * @param  btn: Pointer to the button instance.
  */
void lw_btn_resume(lw_btn_t * btn)
{
    btn->paused = false;
}

/**
  * @brief  Reset the state of a button instance.
  * @param  btn: Pointer to the button instance.
  */
void lw_btn_reset(lw_btn_t * btn)
{
    btn->paused = false;
    btn->current_pressed = false;
    btn->last_tick = 0;
    btn->press_time = 0;
    btn->debounce_tick = 0;
    btn->last_click_tick = 0;
    btn->click_tick = 0;
    btn->long_press_repeat_cnt = 0;
    btn->long_press_repeat_period = LW_BTN_LONG_PRESS_REPEAT_PERIOD_MS;
    btn->long_long_press_enable = false;
    btn->long_long_press_time = 0;
    btn->state = LW_BTN_STATE_IDLE;
    btn->event_dsc_cnt = 0;

    LW_BTN_MEMSET(btn->event_dsc, 0, sizeof(btn->event_dsc));
}

/**
  * @brief  Process button events for all button instances.
  */
void lw_btn_handler(void)
{
    lw_btn_t * p = used_list;
    while(p) {
        process_button_event(p);
        p = p->next;
    }
}

/**
  * @brief  Increase the global tick counter for button event processing.
  * @param  tick_period: Time period to increase the tick counter.
  */
void lw_btn_increase_tick(uint32_t tick_period)
{
    gsul_btn_tick += tick_period;
}

/**
  * @brief  Set whether to stop processing events for a specific callback.
  * @param  btn: Pointer to the button instance.
  * @param  event_cb: Function pointer to the event callback.
  * @param  stop_processing: True to stop processing events, false to continue.
  */
void lw_btn_set_stop_processing(lw_btn_t * btn, lw_btn_event_cb_t event_cb,bool stop_processing)
{
    uint32_t i = 0;
    for(i = 0; i < btn->event_dsc_cnt; i++) {
        if(btn->event_dsc[i].event_cb == event_cb) {
            btn->event_dsc[i].stop_processing = stop_processing;
            break;
        }
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
  * @brief  Send a button event to the registered callbacks.
  * @param  btn: Pointer to the button instance.
  * @param  code: Event code to send.
  * @return True if all callbacks processed the event, false if stopped.
  */
static bool send_event_core(lw_btn_t * btn, lw_btn_event_t code)
{
    uint32_t i = 0;
    lw_btn_event_dsc_t * event_dsc = &btn->event_dsc[0];

    for(i = 0; i < btn->event_dsc_cnt; i++) {
        if(event_dsc[i].code == code && event_dsc[i].event_cb != NULL) {
            event_dsc[i].event_cb(&event_dsc[i]);
            if(event_dsc[i].stop_processing) {
                return false;
            }  
        }
    }
    return true;
}

/**
  * @brief  Process button events for a single button instance.
  * @param  btn: Pointer to the button instance.
  */
static void process_button_event(lw_btn_t * btn)
{
    if(!btn || btn->paused) return;

    uint32_t now_tick = gsul_btn_tick;
    uint32_t elapsed_time = now_tick - btn->last_tick;
    btn->last_tick = now_tick;

    bool new_pressed = lw_btn_is_pressed(btn);
    bool state_changed = (new_pressed != btn->current_pressed);

    /* Debounce */
    if(state_changed) {
        if(btn->debounce_tick == 0) {
            btn->debounce_tick = now_tick;
        }
        else if(now_tick - btn->debounce_tick >= LW_BTN_DEBOUNCE_PERIOD_MS) {
                btn->debounce_tick = 0;
                btn->current_pressed = new_pressed;
        }
        else {
            return;
        }
    }
    else {
        btn->debounce_tick = 0;
    }

    bool pressed = btn->current_pressed;

    switch (btn->state)
    {
    case LW_BTN_STATE_IDLE:
        if(pressed) {
            btn->event = LW_BTN_EVENT_PRESSED;
            btn->state = LW_BTN_STATE_PRESS;
            if(true != send_event_core(btn, LW_BTN_EVENT_PRESSED)) return;
        }
        break;
    
    case LW_BTN_STATE_PRESS:
        if(!pressed) {
            btn->event = LW_BTN_EVENT_RELEASED;
            btn->state = LW_BTN_STATE_RELEASE;
            btn->press_time = 0;
            btn->last_click_tick = btn->click_tick;
            btn->click_tick = now_tick;
            if(true != send_event_core(btn, LW_BTN_EVENT_CLICKED)) return;
            if(true != send_event_core(btn, LW_BTN_EVENT_RELEASED)) return;
        }
        else {
            btn->press_time += elapsed_time;
                
            if(btn->press_time >= LW_BTN_LONG_PRESS_PERIOD_MS) {
                btn->event = LW_BTN_EVENT_LONG_PRESSED;
                btn->state = LW_BTN_STATE_LONG_PRESS;
                btn->long_long_press_time = btn->press_time;
                btn->press_time = 0;
                if(true != send_event_core(btn, LW_BTN_EVENT_LONG_PRESSED)) return;
            }
            else {
                btn->event = LW_BTN_EVENT_PRESSING;
                if(true != send_event_core(btn, LW_BTN_EVENT_PRESSING)) return;
            }
        }
        break;

    case LW_BTN_STATE_LONG_PRESS:
        if(pressed) {
            btn->press_time += elapsed_time;
            btn->long_long_press_time += elapsed_time;

            if(btn->long_long_press_enable && btn->long_long_press_time >= LW_BTN_LONG_LONG_PRESS_PERIOD_MS) {
                btn->event = LW_BTN_EVENT_LONG_LONG_PRESSED;
                btn->state = LW_BTN_STATE_LONG_LONG_PRESS;
                btn->long_long_press_time = 0;
                if(true != send_event_core(btn, LW_BTN_EVENT_LONG_LONG_PRESSED)) return;
                break;
            }

            if(btn->press_time >= 
              (btn->long_press_repeat_period == 0 ? 
               LW_BTN_LONG_PRESS_REPEAT_PERIOD_MS : btn->long_press_repeat_period)) {
                btn->event = LW_BTN_EVENT_LONG_PRESSING;
                btn->press_time = 0;
                btn->long_press_repeat_cnt++;

                uint32_t new_period = LW_BTN_LONG_PRESS_REPEAT_PERIOD_MS + 
                                      150 * 1000 / (1000 + btn->long_press_repeat_cnt * 80);
                if(new_period < 50) new_period = 50;
                btn->long_press_repeat_period = new_period;

                if(true != send_event_core(btn, LW_BTN_EVENT_LONG_PRESSING)) return;
            }
        }
        else {
            btn->event = LW_BTN_EVENT_LONG_PRESSING_END;
            btn->state = LW_BTN_STATE_IDLE;
            btn->press_time = 0;
            btn->long_press_repeat_cnt = 0;
            btn->long_press_repeat_period = LW_BTN_LONG_PRESS_REPEAT_PERIOD_MS;
            if(true != send_event_core(btn, LW_BTN_EVENT_CLICKED)) return;
            if(true != send_event_core(btn, LW_BTN_EVENT_LONG_PRESSING_END)) return;
        }
        break;

    case LW_BTN_STATE_RELEASE:
        if(pressed) {
            btn->event = LW_BTN_EVENT_PRESSED;
            btn->state = LW_BTN_STATE_PRESS;
            if(true != send_event_core(btn, LW_BTN_EVENT_PRESSED)) return;
        }
        else {
            btn->state = LW_BTN_STATE_IDLE;

            if((btn->click_tick - btn->last_click_tick) < LW_BTN_DOUBLE_CLICK_PERIOD_MS) {
                btn->event = LW_BTN_EVENT_DOUBLE_CLICKED;
                if(true != send_event_core(btn, LW_BTN_EVENT_DOUBLE_CLICKED)) return;
            }
            else {
                btn->event = LW_BTN_EVENT_SHORT_CLICKED;
                if(true != send_event_core(btn, LW_BTN_EVENT_SHORT_CLICKED)) return;
            }
        }
        break;

    case LW_BTN_STATE_LONG_LONG_PRESS:
        if(!pressed) {
            btn->event = LW_BTN_EVENT_LONG_LONG_PRESSING_END;
            btn->state = LW_BTN_STATE_IDLE;
            btn->press_time = 0;
            if(true != send_event_core(btn, LW_BTN_EVENT_LONG_LONG_PRESSING_END)) return;
        }
        break;

    default:
        break;
    }


}

/**
  * @brief  Allocate a new button instance from the free list.
  * @param  None
  * @retval Pointer to the allocated button instance, or NULL if no free instance.
  */
static lw_btn_t * alloc_btn(void)
{
    if(!free_list)
        return NULL;

    lw_btn_t * btn = free_list;
    free_list = free_list->next;

    btn->next = used_list;
    used_list = btn;

    return btn;
}

/**
  * @brief  Free a button instance and return it to the free list.
  * @param  btn: Pointer to the button instance to free.
  * @retval None
  */
static void free_btn(lw_btn_t * btn)
{
    lw_btn_t ** pp = &used_list;

    while(*pp) {
        if(*pp == btn) {
            *pp = btn->next;   
            break;
        }
        pp = &((*pp)->next);
    }

    btn->next = free_list;
    free_list = btn;
}

/******************************* (END OF FILE) *********************************/



 







