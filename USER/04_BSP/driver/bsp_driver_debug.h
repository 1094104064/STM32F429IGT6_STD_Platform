/**
  ******************************************************************************
  *
  * @file    bsp_driver_debug.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_debug module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_DRIVER_DEBUG_H
#define _BSP_DRIVER_DEBUG_H



#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
struct debug_oper {
    void (*init)(void);
    void (*putc)(char c);
};

struct debug_driver {
    struct debug_oper * oper;

    void (* init)(struct debug_driver * self);
    void (* putc)(struct debug_driver * self, char c);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_debug_link(struct debug_driver * self, struct debug_oper * oper);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_DEBUG_H*/


/******************************* (END OF FILE) *********************************/

