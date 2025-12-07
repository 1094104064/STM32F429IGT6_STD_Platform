/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_light.c
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
static void mempool_init(void);
static light_obj_t * mempool_alloc(void);
static void mempool_free(light_obj_t * obj);
/**********************
 *  STATIC VARIABLES
 **********************/
static light_obj_t gs_mempool[LIGHT_MAX_NUM];
static light_obj_t * free_list = NULL;     
static light_obj_t * used_list = NULL; 
static bool gs_mempool_initialized = false;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
light_obj_t * bsp_wrapper_light_obj_create(const light_ops_t * ops, const char * const name, void * const user_data)
{
    if(gs_mempool_initialized == false) {
        mempool_init();
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_light_find(name) != NULL) return NULL;

    light_obj_t * obj = mempool_alloc();
    if(obj == NULL) return NULL;

    memset(&obj->ctx, 0, sizeof(light_ctx_t));

    obj->ops= ops;
    obj->ctx.is_initialized = true;
    obj->ctx.user_data = user_data;
    strncpy(obj->ctx.name, name, sizeof(obj->ctx.name) - 1);

    return obj;
}

void bsp_wrapper_light_obj_delete(const char * const name)
{
    light_obj_t * obj = bsp_wrapper_light_find(name);

    if(obj != NULL) {
        mempool_free(obj);
    }
}

light_obj_t * bsp_wrapper_light_find(const char * const name)
{
    light_obj_t * obj = used_list;

    while (obj != NULL) {
        if (strncmp(obj->ctx.name, name, LIGHT_NAME_MAX_LEN) == 0) {
            return obj;
        }
        obj = obj->next;
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
static void mempool_init(void)
{
    if (free_list != NULL) return;

    memset(gs_mempool, 0, sizeof(gs_mempool));

    for(uint32_t i = 0; i < LIGHT_MAX_NUM; i++) {
        gs_mempool[i].next = &gs_mempool[i + 1];
    }
    gs_mempool[LIGHT_MAX_NUM - 1].next = NULL;

    free_list = &gs_mempool[0];
    used_list = NULL;
    gs_mempool_initialized = true;
}

static light_obj_t * mempool_alloc(void)
{
    if(!free_list)
        return NULL;

    light_obj_t * obj = free_list;
    free_list = free_list->next;

    obj->next = used_list;
    used_list = obj;

    return obj;
}

static void mempool_free(light_obj_t * obj)
{
    light_obj_t ** pp = &used_list;

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
