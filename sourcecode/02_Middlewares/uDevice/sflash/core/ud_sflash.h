/**
  ******************************************************************************
  *
  * @file    ud_sflash.h
  * @author  
  * @brief   Header file of sflash module.
  *
  ******************************************************************************
  **/

#ifndef _UD_SFLASH_H
#define _UD_SFLASH_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/**********************
 *      TYPEDEFS
 **********************/
typedef struct ud_sflash_ops ud_sflash_ops_t;

typedef void (* sflash_get_jedec_id_fn_t)(const ud_sflash_ops_t ** ops, uint32_t * jedec_id);
typedef void (* sflash_read_fn_t)(const ud_sflash_ops_t ** ops, uint32_t address, uint8_t * dst, uint32_t length);
typedef bool (* sflash_erase_fn_t)(const ud_sflash_ops_t ** ops, uint32_t address, uint32_t length);
typedef void (* sflash_erase_chip_fn_t)(const ud_sflash_ops_t ** ops);
typedef void (* sflash_write_fn_t)(const ud_sflash_ops_t ** ops, uint32_t address, const uint8_t * src, uint32_t length);

/*********************
 *      DEFINES
 *********************/
#define SFLASH_ERASE_TYPE_MAX_NUM    5

struct ud_sflash_eraser
{
    uint32_t erase_size;
    sflash_erase_fn_t erasing;
};

struct ud_sflash_ops
{
    sflash_get_jedec_id_fn_t get_jedec_id;
    sflash_read_fn_t read;
    struct ud_sflash_eraser eraser[SFLASH_ERASE_TYPE_MAX_NUM];
    sflash_erase_chip_fn_t erase_chip;
    sflash_write_fn_t write; 
};

struct ud_sflash
{
    const struct ud_sflash_ops ** ops;
    uint32_t chip_capacity;
};
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/



/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_UD_SFLASH_H*/


/******************************* (END OF FILE) *********************************/


