/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_sflash.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_sflash module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_WRAPPER_SFLASH_H
#define _BSP_WRAPPER_SFLASH_H


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
#define SFLASH_MAX_NUM       2
#define SFLASH_NAME_MAX_LEN  16
#define SFLASH_ERASE_TYPE_MAX_NUM 4
/**********************
 *      TYPEDEFS
 **********************/
typedef struct sflash_eraser   sflash_eraser_t;
typedef struct sflash_ops      sflash_ops_t;
typedef struct sflash_ctx      sflash_ctx_t;
typedef struct sflash_object   sflash_obj_t;
typedef struct sflash_wrapper  sflash_wrapper_t;

struct sflash_eraser {
    uint32_t    size;
    bool        (* pf_erasing)          (uint32_t address, uint32_t length);
};

struct sflash_ops
{
    int     (* pf_init)                 (void);
    void    (* pf_read_jedec_id)        (uint32_t * id);
    bool    (* pf_read)                 (uint32_t address, uint8_t * dst, uint32_t length);
    bool    (* pf_fast_read)            (uint32_t address, uint8_t * dst, uint32_t length);
    bool    (* pf_erase_chip)           (void);
    bool    (* pf_write)                (uint32_t address, const uint8_t * src, uint32_t length);
    sflash_eraser_t                     eraser[SFLASH_ERASE_TYPE_MAX_NUM];

};

struct sflash_ctx
{
    uint8_t         idx;
    void *          user_data;
    char            name[SFLASH_NAME_MAX_LEN];
    bool            is_initialized;
    uint32_t        chip_capacity;
};

struct sflash_object
{
    const sflash_ops_t * ops;
    sflash_ctx_t ctx;
};

struct sflash_wrapper 
{
    sflash_obj_t *  (* obj_create)  (const sflash_ops_t * ops, const char * const name, void * const user_data);
    void            (* obj_delete)  (const char * const name);
    sflash_obj_t *  (* find)        (const char * const name);
    
    bool            (* init)        (sflash_obj_t * obj);
    void            (* read_jedec_id)(sflash_obj_t * obj, uint32_t * id);
    bool            (* read)        (sflash_obj_t * obj, uint32_t address, uint8_t * dst, uint32_t length);
    bool            (* erase)       (sflash_obj_t * obj, uint32_t address, uint32_t length);
    bool            (* write)       (sflash_obj_t * obj, uint32_t address, const uint8_t * src, uint32_t length);
    bool            (* erase_write) (sflash_obj_t * obj, uint32_t address, const uint8_t * src, uint32_t length);
    bool            (* chip_erase)  (sflash_obj_t * obj);
};

extern const struct sflash_wrapper wrp_sflash;

/**********************
*  GLOBAL PROTOTYPES
 **********************/

sflash_obj_t *  bsp_wrapper_sflash_create(const sflash_ops_t * ops, const char * const name, void * const user_data);
void            bsp_wrapper_sflash_delete(const char * const name);
sflash_obj_t *  bsp_wrapper_sflash_find(const char * const name);
bool            bsp_wrapper_sflash_init(sflash_obj_t * obj);
void            bsp_wrapper_sflash_read_jedec_id(sflash_obj_t * obj, uint32_t * id);
bool            bsp_wrapper_sflash_read(sflash_obj_t * obj, uint32_t address, uint8_t * dst, uint32_t length);
bool            bsp_wrapper_sflash_erase(sflash_obj_t * obj, uint32_t address, uint32_t length);
bool            bsp_wrapper_sflash_write(sflash_obj_t * obj, uint32_t address, const uint8_t * src, uint32_t length);
bool            bsp_wrapper_sflash_erase_write(sflash_obj_t * obj, uint32_t address, const uint8_t * src, uint32_t length);
bool            bsp_wrapper_sflash_chip_erase(sflash_obj_t * obj);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_SFLASH_H*/


/******************************* (END OF FILE) *********************************/

