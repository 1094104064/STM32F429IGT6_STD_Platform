/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_light.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_light module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_WRAPPER_LIGHT_H
#define _BSP_WRAPPER_LIGHT_H


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
#define LIGHT_MAX_NUM 2
/**********************
 *      TYPEDEFS
 **********************/

struct light_wrapper {
    int8_t          idx;
    void *          user_data;
    const char *    name;

    int  (* pf_init)(struct light_wrapper * self);
    void (* pf_on)  (struct light_wrapper * self);
    void (* pf_off) (struct light_wrapper * self);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_light_link(struct light_wrapper * self, const char * const name, void * const user_data);
bool bsp_wrapper_light_init(void);
void bsp_wrapper_light_deinit(void);
void bsp_wrapper_light_set_operation_object(const char * name);
void bsp_wrapper_light_on(void);
void bsp_wrapper_light_off(void);
/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_LIGHT_H*/


/******************************* (END OF FILE) *********************************/

