/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_touchpad.c
  * @author  Jamin
  * @brief   Touchpad wrapper for BSP
  *
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  **/
  
  
/*********************
 *      INCLUDES
 *********************/
#include "bsp_wrapper_touchpad.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static struct touchpad_wrapper touchpad_wrappers[TOUCHPAD_MAX_NUM];
static int8_t current_touchpad_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void bsp_wrapper_touchpad_link(struct touchpad_wrapper * self)
{
    current_touchpad_idx++;
    if(current_touchpad_idx >= TOUCHPAD_MAX_NUM) {
        current_touchpad_idx = 0;
    }

    int8_t idx = current_touchpad_idx;

    touchpad_wrappers[idx] = *self;
}

void bsp_wrapper_touchpad_init(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->init(self);
}


void bsp_wrapper_touchpad_reset(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->reset(self);
}

void bsp_wrapper_touchpad_scan(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->scan(self);
}

uint8_t bsp_wrapper_touchpad_is_pressed(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    return self->is_pressed(self);
}

void bsp_wrapper_touchpad_get_coordinates(uint16_t * x, uint16_t * y, uint8_t num)
{
    if(x == NULL || y == NULL) {
        return;
    }

    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->get_coordinates(self, x, y, num);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

