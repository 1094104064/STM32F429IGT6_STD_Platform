/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_sflash.c
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
#include "bsp_wrapper_sflash.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 
const struct sflash_wrapper wrp_sflash = {
    .obj_create        = bsp_wrapper_sflash_create,
    .obj_delete        = bsp_wrapper_sflash_delete,
    .find              = bsp_wrapper_sflash_find,
    
    .init              = bsp_wrapper_sflash_init,
    .read_jedec_id     = bsp_wrapper_sflash_read_jedec_id,
    .read              = bsp_wrapper_sflash_read,
    .erase             = bsp_wrapper_sflash_erase,
    .write             = bsp_wrapper_sflash_write,
    .erase_write       = bsp_wrapper_sflash_erase_write,
    .chip_erase        = bsp_wrapper_sflash_chip_erase,
};
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void mempool_init(void);
static sflash_obj_t * mempool_alloc(void);
static void mempool_free(sflash_obj_t * obj);
/**********************
 *  STATIC VARIABLES
 **********************/
static sflash_obj_t gs_mempool[SFLASH_MAX_NUM];
static sflash_obj_t * free_list = NULL;     
static sflash_obj_t * used_list = NULL; 
static bool gs_mempool_initialized = false;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
sflash_obj_t * bsp_wrapper_sflash_create(const sflash_ops_t * ops, const char * const name, void * const user_data)
{
    if(gs_mempool_initialized == false) {
        mempool_init();
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_sflash_find(name) != NULL) return NULL;

    sflash_obj_t * obj = mempool_alloc();
    if(obj == NULL) return NULL;

    memset(&obj->ctx, 0, sizeof(sflash_ctx_t));

    obj->ops= ops;
    obj->ctx.is_initialized = true;
    obj->ctx.user_data = user_data;
    strncpy(obj->ctx.name, name, sizeof(obj->ctx.name) - 1);

    return obj;
}

void bsp_wrapper_sflash_delete(const char * const name)
{
    sflash_obj_t * obj = bsp_wrapper_sflash_find(name);

    if(obj != NULL) {
        mempool_free(obj);
    }
}

sflash_obj_t * bsp_wrapper_sflash_find(const char * const name)
{
    sflash_obj_t * obj = used_list;

    while (obj != NULL) {
        if (strncmp(obj->ctx.name, name, SFLASH_NAME_MAX_LEN) == 0) {
            return obj;
        }
        obj = obj->next;
    }
    return NULL;
}

bool bsp_wrapper_sflash_init(sflash_obj_t * obj)
{
    if(obj->ctx.is_initialized == true) return true;

    int ret = 1;
    if(obj->ops->pf_init)
        obj->ops->pf_init();

    if(ret != 0) {
        return false;
    }

    obj->ctx.is_initialized = true;

    return true;
}

bool bsp_wrapper_sflash_read_jedec_id(sflash_obj_t * obj, uint32_t * id)
{
    if(obj->ops->pf_read_jedec_id)
        return obj->ops->pf_read_jedec_id(id);
    return false;
}

bool bsp_wrapper_sflash_read(sflash_obj_t * obj, uint32_t address, uint8_t * dst, uint32_t length)
{
    /* check the flash address bound */
    if (address + length > obj->ctx.chip_capacity) {
        return false;
    }

    if(obj->ops->pf_fast_read) {
        return obj->ops->pf_fast_read(address, dst, length);
    } 
    else if(obj->ops->pf_read) {
        return obj->ops->pf_read(address, dst, length);
    }

    return false;
}

bool bsp_wrapper_sflash_erase(sflash_obj_t * obj, uint32_t address, uint32_t length)
{
    bool result = true;

    /* check the flash address bound */
    if (address + length > obj->ctx.chip_capacity) {

        return false;
    }

    if (address == 0 && length == obj->ctx.chip_capacity) {
        
        return bsp_wrapper_sflash_chip_erase(obj);
    }

    size_t e_index;

    /* Find the suitable eraser.
     * The largest size eraser is at the end of eraser table.
     * In order to decrease erase command counts, so the find process is from the end of eraser table. */
    for (e_index = SFLASH_ERASE_TYPE_MAX_NUM - 1;; e_index--) {

        if ((obj->ops->eraser[e_index].size != 0) && 
            (length >= obj->ops->eraser[e_index].size) && 
            (address % obj->ops->eraser[e_index].size == 0)) 
        {
            break;
        }

        if (e_index == 0) {
            break;
        }
    }

    if((obj->ops->eraser[e_index].size == 0) ||
       (obj->ops->eraser[e_index].pf_erasing == NULL)) {
        /* No suitable eraser found */
        return false;
    }

    size_t e_size = obj->ops->eraser[e_index].size;

    /* loop erase operate. erase unit is erase granularity */
    while (length) {

        result = obj->ops->eraser[e_index].pf_erasing(address, length);
        if (result != true) {
            goto __exit;
        }


        /* make erase align and calculate next erase address */
        if (address % e_size != 0) {
            if (length > e_size - (address % e_size)) {

                length -= e_size - (address % e_size);
                address += e_size - (address % e_size);
            } 
            else {
                goto __exit;
            }
        } 
        else {
            if (length > e_size) {

                length -= e_size;
                address += e_size;
            } 
            else {
                goto __exit;
            }
        }
    }

__exit:

    return result;
}

bool bsp_wrapper_sflash_write(sflash_obj_t * obj, uint32_t address, const uint8_t * src, uint32_t length)
{
    if(obj->ops->pf_write)
       return obj->ops->pf_write(address, src, length);
    
    return false;
}

bool bsp_wrapper_sflash_erase_write(sflash_obj_t * obj, uint32_t address, const uint8_t * src, uint32_t length)
{
    bool result = true;

    result = bsp_wrapper_sflash_erase(obj, address, length);

    if (result == true) {
        result = bsp_wrapper_sflash_write(obj, address, src, length);
    }

    return result;
}

bool bsp_wrapper_sflash_chip_erase(sflash_obj_t * obj)
{

    if(obj->ops->pf_erase_chip)
        return obj->ops->pf_erase_chip();

    return false;
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
static void mempool_init(void)
{
    if (free_list != NULL) return;

    memset(gs_mempool, 0, sizeof(gs_mempool));

    for(uint32_t i = 0; i < SFLASH_MAX_NUM; i++) {
        gs_mempool[i].next = &gs_mempool[i + 1];
    }
    gs_mempool[SFLASH_MAX_NUM - 1].next = NULL;

    free_list = &gs_mempool[0];
    used_list = NULL;
    gs_mempool_initialized = true;
}

static sflash_obj_t * mempool_alloc(void)
{
    if(!free_list)
        return NULL;

    sflash_obj_t * obj = free_list;
    free_list = free_list->next;

    obj->next = used_list;
    used_list = obj;

    return obj;
}

static void mempool_free(sflash_obj_t * obj)
{
    sflash_obj_t ** pp = &used_list;

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
