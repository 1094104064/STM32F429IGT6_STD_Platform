/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_sflash.c
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

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static sflash_obj_t gs_mempool[SFLASH_MAX_NUM];
static uint8_t gsuc_index = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
sflash_obj_t * bsp_wrapper_sflash_create(const sflash_ops_t * ops, const char * const name, void * const user_data)
{
    if(gsuc_index >= SFLASH_MAX_NUM) {
        gsuc_index = 0;
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_sflash_find(name) != NULL) return NULL;

    memset(&gs_mempool[gsuc_index], 0, sizeof(sflash_obj_t));

    gs_mempool[gsuc_index].ops = ops;

    gs_mempool[gsuc_index].ctx.idx       = gsuc_index;
    gs_mempool[gsuc_index].ctx.user_data = user_data;
    strncpy(gs_mempool[gsuc_index].ctx.name, name, sizeof(gs_mempool[gsuc_index].ctx.name) - 1);

    gsuc_index++;

    return &gs_mempool[gsuc_index - 1];
}

void bsp_wrapper_sflash_delete(const char * const name)
{
    sflash_obj_t * obj = bsp_wrapper_sflash_find(name);
    if(obj != NULL) {
        memset(obj, 0, sizeof(sflash_obj_t));
    }
}

sflash_obj_t * bsp_wrapper_sflash_find(const char * const name)
{
    uint8_t i = 0;

    for(i = 0; i < SFLASH_MAX_NUM; i++) {
        if(strncmp(gs_mempool[i].ctx.name, name, SFLASH_NAME_MAX_LEN) == 0) {
            return &gs_mempool[i];
        }
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

void bsp_wrapper_sflash_read_jedec_id(sflash_obj_t * obj, uint32_t * id)
{
    if(obj->ops->pf_read_jedec_id)
        obj->ops->pf_read_jedec_id(id);
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








/******************************* (END OF FILE) *********************************/
