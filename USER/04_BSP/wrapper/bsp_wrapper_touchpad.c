/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_touchpad.c
  * @author  Jamin
  * @brief   Touchpad wrapper for BSP
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
#include "bsp_wrapper_touchpad.h"
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
static touchpad_obj_t gs_mempool[TOUCHPAD_MAX_NUM];
static uint8_t gsuc_index = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

touchpad_obj_t * bsp_wrapper_touchpad_obj_create(const touchpad_ops_t * ops, const char * const name, void * const user_data)
{
    if(gsuc_index >= TOUCHPAD_MAX_NUM) {
        gsuc_index = 0;
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_touchpad_find(name) != NULL) return NULL;

    memset(&gs_mempool[gsuc_index], 0, sizeof(touchpad_obj_t));

    gs_mempool[gsuc_index].ops = ops;

    gs_mempool[gsuc_index].ctx.idx       = gsuc_index;
    gs_mempool[gsuc_index].ctx.user_data = user_data;
    strncpy(gs_mempool[gsuc_index].ctx.name, name, sizeof(gs_mempool[gsuc_index].ctx.name) - 1);

    gsuc_index++;

    return &gs_mempool[gsuc_index - 1];
}

void bsp_wrapper_touchpad_obj_delete(const char * const name)
{
    touchpad_obj_t * obj = bsp_wrapper_touchpad_find(name);

    if(obj != NULL) {
        memset(obj, 0, sizeof(touchpad_obj_t));
    }
}

touchpad_obj_t * bsp_wrapper_touchpad_find(const char * const name)
{
    uint8_t i = 0;

    if(name == NULL) return NULL;

    for(i = 0; i < sizeof(gs_mempool) / sizeof(gs_mempool[0]); i++ ) {
        if(strncmp(gs_mempool[i].ctx.name, name, TOUCHPAD_NAME_MAX_LEN) == 0) {
            return &gs_mempool[i];
        }
    }
    return NULL;
}

bool bsp_wrapper_touchpad_init(touchpad_obj_t * obj)
{
    if(obj->ctx.is_initialized == true) return true;

    int ret = 1;

    if(obj->ops->pf_init)
        ret = obj->ops->pf_init();
        
    if(ret != 0) {
        return false;
    }

    obj->ctx.is_initialized = true;

    return true;
}

void bsp_wrapper_touchpad_reset(touchpad_obj_t * obj)
{
    if(obj->ops->pf_reset)
        obj->ops->pf_reset();
}

void bsp_wrapper_touchpad_scan(touchpad_obj_t * obj)
{
    if(obj->ops->pf_scan)
        obj->ops->pf_scan();
}

bool bsp_wrapper_touchpad_is_pressed(touchpad_obj_t * obj)
{
    if(obj->ops->pf_is_pressed)
        return obj->ops->pf_is_pressed();

    return false;
}

void bsp_wrapper_touchpad_get_coordinates(touchpad_obj_t * obj, uint16_t * x, uint16_t * y, uint8_t num)
{
    if(obj->ops->pf_get_coordinates)
        obj->ops->pf_get_coordinates(x, y, num);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

