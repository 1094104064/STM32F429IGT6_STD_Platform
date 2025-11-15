/**
  ******************************************************************************
  *
  * @file    edf_mem.h
  * @author  
  * @brief   Header file of edf_mem module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_MEM_H
#define _EDF_MEM_H
    

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
#include "edf_macro.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct edf_heap_block edf_heap_block_t;
 /**
 * @brief Heap block structure.
 */
struct edf_heap_block
{
    struct edf_heap_block *next;                                     /**< Point to next block */
    uint32_t size: 31;                                              /**< Size of this block */
    uint32_t allocated: 1;                                          /**< Allocated flag */
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void edf_heap_init(void);
EDF_WEAK void *edf_malloc(size_t size);
EDF_WEAK void edf_free(void *memory);
EDF_WEAK size_t edf_malloc_usable_size(void *memory);
EDF_WEAK void *edf_calloc(size_t num, size_t size);
EDF_WEAK void *edf_realloc(void *memory, size_t size);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_EDF_MEM_H*/


/******************************* (END OF FILE) *********************************/


