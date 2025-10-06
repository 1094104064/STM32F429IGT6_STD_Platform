/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_delay.c
  * @author  Jamin
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
#include "bsp_wrapper_delay.h"
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
static delay_wrapper_t gs_wrappers[DELAY_MAX_NUM];
static uint8_t gs_index = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

delay_wrapper_t * bsp_wrapper_delay_create(delay_wrapper_t * src, const char * const name, void * const user_data)
{
    if(gs_index >= DELAY_MAX_NUM) {
        gs_index = 0;
    }

    if(bsp_wrapper_delay_find(name) != NULL) return NULL;

    memset(&gs_wrappers[gs_index], 0, sizeof(delay_wrapper_t));

    gs_wrappers[gs_index].pf_init       = src->pf_init;
    gs_wrappers[gs_index].pf_delay_us   = src->pf_delay_us;
    gs_wrappers[gs_index].pf_delay_ms   = src->pf_delay_ms;
    gs_wrappers[gs_index].pf_delay_sec  = src->pf_delay_sec;
    gs_wrappers[gs_index].user_data     = user_data;
    gs_wrappers[gs_index].index         = gs_index;

    strncpy(gs_wrappers[gs_index].name, name, sizeof(gs_wrappers[gs_index].name) - 1);

    gs_index++;

    return &gs_wrappers[gs_index - 1];
}

delay_wrapper_t * bsp_wrapper_delay_find(const char * const name)
{
    for(uint8_t i = 0; i < ARRAY_SIZE(gs_wrappers); i++ ) {
        if(strncmp(gs_wrappers[i].name, name, BSP_MAX_NAME_LEN) == 0) {
            return &gs_wrappers[i];
        }
    }
    return NULL;
}

bool bsp_wrapper_delay_init(delay_wrapper_t * obj)
{
    int ret = 0;
    
    assert_null(obj->pf_init);
    assert_null(obj->pf_delay_us);
    assert_null(obj->pf_delay_ms);
    assert_null(obj->pf_delay_sec);

    if( obj->pf_init       == NULL || obj->pf_delay_us    == NULL ||
        obj->pf_delay_ms   == NULL || obj->pf_delay_sec   == NULL) {
        return false;
    }

    ret = obj->pf_init(obj);

    if(ret != 0) {
        pr_error("%s : failed to initialize, error code: %d", obj->name, ret);
        return false;
    }

    pr_info("%s : initialized successfully", obj->name);

    return true;
}

void bsp_wrapper_delay_us(delay_wrapper_t * obj, uint32_t us)
{
    obj->pf_delay_us(obj, us);
}

void bsp_wrapper_delay_ms(delay_wrapper_t * obj, uint32_t ms)
{
    obj->pf_delay_ms(obj, ms);
}

void bsp_wrapper_delay_sec(delay_wrapper_t * obj, uint32_t sec)
{
    obj->pf_delay_sec(obj, sec);
}



/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

