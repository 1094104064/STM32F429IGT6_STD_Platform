/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_touchpad.c
  * @author  
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
const struct touchpad_wrapper wrp_touchpad = {
    .obj_create        = bsp_wrapper_touchpad_obj_create,
    .obj_delete        = bsp_wrapper_touchpad_obj_delete,
    .find              = bsp_wrapper_touchpad_find,
    
    .init              = bsp_wrapper_touchpad_init,
    .is_pressed       = bsp_wrapper_touchpad_is_pressed,
    .control          = bsp_wrapper_touchpad_control,
    .get_xy           = bsp_wrapper_touchpad_get_xy,
};
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void mempool_init(void);
static touchpad_obj_t * mempool_alloc(void);
static void mempool_free(touchpad_obj_t * obj);
/**********************
 *  STATIC VARIABLES
 **********************/
static touchpad_obj_t gs_mempool[TOUCHPAD_MAX_NUM];
static touchpad_obj_t * free_list = NULL;     
static touchpad_obj_t * used_list = NULL; 
static bool gs_mempool_initialized = false;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

touchpad_obj_t * bsp_wrapper_touchpad_obj_create(const touchpad_ops_t * ops, const char * const name, void * const user_data)
{
    if(gs_mempool_initialized == false) {
        mempool_init();
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_touchpad_find(name) != NULL) return NULL;

    touchpad_obj_t * obj = mempool_alloc();
    if(obj == NULL) return NULL;

    memset(&obj->ctx, 0, sizeof(touchpad_ctx_t));

    obj->ops= ops;
    obj->ctx.is_initialized = true;
    obj->ctx.user_data = user_data;
    strncpy(obj->ctx.name, name, sizeof(obj->ctx.name) - 1);

    return obj;
}

void bsp_wrapper_touchpad_obj_delete(const char * const name)
{
    touchpad_obj_t * obj = bsp_wrapper_touchpad_find(name);

    if(obj != NULL) {
        mempool_free(obj);
    }
}

touchpad_obj_t * bsp_wrapper_touchpad_find(const char * const name)
{
    touchpad_obj_t * obj = used_list;

    while (obj != NULL) {
        if (strncmp(obj->ctx.name, name, TOUCHPAD_NAME_MAX_LEN) == 0) {
            return obj;
        }
        obj = obj->next;
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

bool bsp_wrapper_touchpad_is_pressed(touchpad_obj_t * obj)
{
    if(obj->ops->pf_is_pressed)
        return obj->ops->pf_is_pressed();

    return false;
}

void bsp_wrapper_touchpad_control(touchpad_obj_t * obj, int cmd, void * arg)
{
    if(obj->ops->pf_control)
        obj->ops->pf_control(cmd, arg);
}

void bsp_wrapper_touchpad_get_xy(touchpad_obj_t * obj, uint16_t * x, uint16_t * y, uint8_t read_num)
{
    if(obj->ops->pf_get_xy)
        obj->ops->pf_get_xy(x, y, read_num);
}





/**********************
 *   STATIC FUNCTIONS
 **********************/
static void mempool_init(void)
{
    if (free_list != NULL) return;

    memset(gs_mempool, 0, sizeof(gs_mempool));

    for(uint32_t i = 0; i < TOUCHPAD_MAX_NUM; i++) {
        gs_mempool[i].next = &gs_mempool[i + 1];
    }
    gs_mempool[TOUCHPAD_MAX_NUM - 1].next = NULL;

    free_list = &gs_mempool[0];
    used_list = NULL;
    gs_mempool_initialized = true;
}

static touchpad_obj_t * mempool_alloc(void)
{
    if(!free_list)
        return NULL;

    touchpad_obj_t * obj = free_list;
    free_list = free_list->next;

    obj->next = used_list;
    used_list = obj;

    return obj;
}

static void mempool_free(touchpad_obj_t * obj)
{
    touchpad_obj_t ** pp = &used_list;

    while(*pp) {
        if(*pp == obj) {
            *pp = obj->next;   
            break;
        }
        pp = &((*pp)->next);
    }

    obj->next = free_list;
    free_list = obj;
}



/******************************* (END OF FILE) *********************************/

