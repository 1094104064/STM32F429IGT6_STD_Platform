/**
  ******************************************************************************
  *
  * @file    memory_manager.h
  * @author  
  * @brief   Header file of memory_manager module.
  *
  ******************************************************************************
  **/
  
#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

void mm_init(void);
void mm_deinit(void);
void * mm_alloc(size_t size);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_MEMORY_MANAGER_H*/


/******************************* (END OF FILE) *********************************/


