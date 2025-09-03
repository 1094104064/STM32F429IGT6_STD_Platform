/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_delay.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_delay module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_WRAPPER_DELAY_H
#define _BSP_WRAPPER_DELAY_H


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
/*********************
 *      DEFINES
 *********************/
#define DELAY_MAX_NUM  2
/**********************
 *      TYPEDEFS
 **********************/

struct delay_wrapper {
    int8_t idx;
    void * user_data;

    void (* init)(struct delay_wrapper * self);
    void (* delay_us)(struct delay_wrapper * self, uint32_t us);
    void (* delay_ms)(struct delay_wrapper * self, uint32_t ms);
    void (* delay_sec)(struct delay_wrapper * self, uint32_t sec);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_delay_link(struct delay_wrapper * self);
void bsp_wrapper_delay_init(void);
void bsp_wrapper_delay_us(uint32_t us);
void bsp_wrapper_delay_ms(uint32_t ms);
void bsp_wrapper_delay_sec(uint32_t sec);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_DELAY_H*/


/******************************* (END OF FILE) *********************************/

