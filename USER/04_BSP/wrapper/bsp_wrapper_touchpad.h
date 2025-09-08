/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_touchpad.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_touchpad module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_WRAPPER_TOUCHPAD_H
#define _BSP_WRAPPER_TOUCHPAD_H


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
#define TOUCHPAD_MAX_NUM  2
/**********************
 *      TYPEDEFS
 **********************/
 
struct touchpad_wrapper {
    int8_t          idx;
    void *          user_data;
    const char *    name;

    int (* pf_init)            (struct touchpad_wrapper * self);
    void (* pf_reset)           (struct touchpad_wrapper * self);
    void (* pf_scan)            (struct touchpad_wrapper * self);
    uint8_t (* pf_is_pressed)   (struct touchpad_wrapper * self);
    void (* pf_get_coordinates) (struct touchpad_wrapper * self, uint16_t * x, uint16_t * y, uint8_t num);
};


/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_touchpad_link(struct touchpad_wrapper * self, const char * const name, void * const user_data);
bool bsp_wrapper_touchpad_init(void);
void bsp_wrapper_touchpad_reset(void);
void bsp_wrapper_touchpad_scan(void);
uint8_t bsp_wrapper_touchpad_is_pressed(void);
void bsp_wrapper_touchpad_get_coordinates(uint16_t * x, uint16_t * y, uint8_t num);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_TOUCHPAD_H*/


/******************************* (END OF FILE) *********************************/

