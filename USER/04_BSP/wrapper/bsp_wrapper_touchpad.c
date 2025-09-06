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
static uint8_t current_touchpad_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void bsp_wrapper_touchpad_link(struct touchpad_wrapper * self)
{
    uint8_t idx = 0;

    current_touchpad_idx++;

    if(current_touchpad_idx < TOUCHPAD_MAX_NUM) {
        idx = current_touchpad_idx;
    }
    else {
        current_touchpad_idx = 0;
    }

    memset(&touchpad_wrappers[idx], 0, sizeof(struct touchpad_wrapper));

    touchpad_wrappers[idx] = *self;
    touchpad_wrappers[idx].idx = idx;

    if(touchpad_wrappers[idx].name == NULL) {
        pr_warn("This wrapper has no name and will be filled with a default name");
        touchpad_wrappers[idx].name = "touchpad_default";
    }
}

bool bsp_wrapper_touchpad_init(void)
{
    int ret = 0;
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    if( self->pf_init               == NULL || self->pf_reset   == NULL ||
        self->pf_is_pressed         == NULL || self->pf_scan    == NULL ||
        self->pf_get_coordinates    == NULL) {
        pr_fatal("%s : there is a missing function pointer", self->name);
        return false;
    }

    ret = self->pf_init(self);

    if(ret != 0) {
        pr_error("%s : failed to initialize, error code: %d", self->name, ret);
        return false;
    }

    pr_info("%s : initialized successfully", self->name);

    return true;
}


void bsp_wrapper_touchpad_reset(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->pf_reset(self);
}

void bsp_wrapper_touchpad_scan(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->pf_scan(self);
}

uint8_t bsp_wrapper_touchpad_is_pressed(void)
{
    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    return self->pf_is_pressed(self);
}

void bsp_wrapper_touchpad_get_coordinates(uint16_t * x, uint16_t * y, uint8_t num)
{
    if(x == NULL || y == NULL) {
        return;
    }

    struct touchpad_wrapper * self = &touchpad_wrappers[current_touchpad_idx];

    self->pf_get_coordinates(self, x, y, num);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

