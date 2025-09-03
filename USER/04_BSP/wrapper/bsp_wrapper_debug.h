/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_debug.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_debug module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_WRAPPER_DEBUG_H
#define _BSP_WRAPPER_DEBUG_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
/*********************
 *      DEFINES
 *********************/
#define DEBUG_MAX_NUM 1
/**********************
 *      TYPEDEFS
 **********************/
struct debug_wrapper {
    int8_t idx;
    void * user_data;

    void (* init)(struct debug_wrapper * self);
    void (* putc)(struct debug_wrapper * self, char c);
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_debug_link(struct debug_wrapper * self);
void bsp_wrapper_debug_init(void);
void bsp_wrapper_debug_deinit(void);
void bsp_wrapper_debug_putc(char c);
void bsp_wrapper_debug_puts(const char * const fmt, ...);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_DEBUG_H*/


/******************************* (END OF FILE) *********************************/

