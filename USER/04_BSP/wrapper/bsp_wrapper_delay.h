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
#include "user_conf.h"
/*********************
 *      DEFINES
 *********************/
#define DELAY_MAX_NUM  2
/**********************
 *      TYPEDEFS
 **********************/

struct delay_wrapper {
    int8_t          idx;
    void *          user_data;
    const char *    name;

    int  (* pf_init)     (struct delay_wrapper * self);
    void (* pf_delay_us) (struct delay_wrapper * self, uint32_t us);
    void (* pf_delay_ms) (struct delay_wrapper * self, uint32_t ms);
    void (* pf_delay_sec)(struct delay_wrapper * self, uint32_t sec);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_delay_link(struct delay_wrapper * self);
bool bsp_wrapper_delay_init(void);
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

