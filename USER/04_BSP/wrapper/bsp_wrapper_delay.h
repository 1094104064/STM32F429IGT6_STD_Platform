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
#include "user_macros.h"
/*********************
 *      DEFINES
 *********************/
#define DELAY_MAX_NUM  1
/**********************
 *      TYPEDEFS
 **********************/

typedef struct delay_wrapper delay_wrapper_t;

struct delay_wrapper 
{
    uint8_t             index;
    char                name[BSP_MAX_NAME_LEN];
    void *              user_data;

    int  (* pf_init)     (struct delay_wrapper * self);
    void (* pf_delay_us) (struct delay_wrapper * self, uint32_t us);
    void (* pf_delay_ms) (struct delay_wrapper * self, uint32_t ms);
    void (* pf_delay_sec)(struct delay_wrapper * self, uint32_t sec);
};



/**********************
*  GLOBAL PROTOTYPES
 **********************/
delay_wrapper_t * bsp_wrapper_delay_create(delay_wrapper_t * src, const char * const name, void * const user_data);
delay_wrapper_t * bsp_wrapper_delay_find(const char * const name);
bool bsp_wrapper_delay_init(delay_wrapper_t * obj);
void bsp_wrapper_delay_us(delay_wrapper_t * obj, uint32_t us);
void bsp_wrapper_delay_ms(delay_wrapper_t * obj, uint32_t ms);
void bsp_wrapper_delay_sec(delay_wrapper_t * obj, uint32_t sec);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_DELAY_H*/


/******************************* (END OF FILE) *********************************/

