/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_light.c
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
#include "bsp_wrapper_light.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 
const struct light_wrapper wrp_light = 
{
    .obj_create = bsp_wrapper_light_obj_create,
    .obj_delete = bsp_wrapper_light_obj_delete,
    .find       = bsp_wrapper_light_find,

    .init       = bsp_wrapper_light_init,
    .on         = bsp_wrapper_light_on,
    .off        = bsp_wrapper_light_off,
};
/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static light_obj_t gs_mempool[LIGHT_MAX_NUM];
static uint8_t gsuc_index = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
light_obj_t * bsp_wrapper_light_obj_create(const light_ops_t * ops, const char * const name, void * const user_data)
{
    if(gsuc_index >= LIGHT_MAX_NUM) {
        gsuc_index = 0;
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_light_find(name) != NULL) return NULL;

    memset(&gs_mempool[gsuc_index], 0, sizeof(light_obj_t));

    gs_mempool[gsuc_index].ops = ops;

    gs_mempool[gsuc_index].ctx.idx       = gsuc_index;
    gs_mempool[gsuc_index].ctx.user_data = user_data;
    strncpy(gs_mempool[gsuc_index].ctx.name, name, sizeof(gs_mempool[gsuc_index].ctx.name) - 1);

    gsuc_index++;

    return &gs_mempool[gsuc_index - 1];
}

void bsp_wrapper_light_obj_delete(const char * const name)
{
    light_obj_t * obj = bsp_wrapper_light_find(name);

    if(obj != NULL) {
        memset(obj, 0, sizeof(light_obj_t));
    }
}

light_obj_t * bsp_wrapper_light_find(const char * const name)
{
    uint8_t i = 0;

    if(name == NULL) return NULL;

    for(i = 0; i < sizeof(gs_mempool) / sizeof(gs_mempool[0]); i++ ) {
        if(strncmp(gs_mempool[i].ctx.name, name, LIGHT_NAME_MAX_LEN) == 0) {
            return &gs_mempool[i];
        }
    }
    return NULL;
}

bool bsp_wrapper_light_init(light_obj_t * obj)
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

void bsp_wrapper_light_on(light_obj_t * obj)
{
    if(obj->ops->pf_on)
        obj->ops->pf_on();
}

void bsp_wrapper_light_off(light_obj_t * obj)
{
    if(obj->ops->pf_off)
        obj->ops->pf_off();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
