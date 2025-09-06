/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_light.c
  * @author  Jamin
  * @brief   
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
#include "bsp_wrapper_light.h"
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
static struct light_wrapper light_wrappers[LIGHT_MAX_NUM];
static uint8_t current_light_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_wrapper_light_link(struct light_wrapper * self)
{
    uint8_t idx = 0;

    current_light_idx++;

    if(current_light_idx < LIGHT_MAX_NUM) {
        idx = current_light_idx;
    }
    else {
        current_light_idx = 0;
    }

    memset(&light_wrappers[idx], 0, sizeof(struct light_wrapper));

    light_wrappers[idx] = *self;
    light_wrappers[idx].idx = idx;

    if(light_wrappers[idx].name == NULL) {
        pr_warn("This wrapper has no name and will be filled with a default name");
        light_wrappers[idx].name = "light_default";
    }
}

bool bsp_wrapper_light_init(void)
{
    int ret = 0;
    struct light_wrapper * self = &light_wrappers[current_light_idx];

    if( self->pf_init == NULL || self->pf_on == NULL ||
        self->pf_off == NULL) {
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

void bsp_wrapper_light_deinit(void)
{
    struct light_wrapper * self = &light_wrappers[current_light_idx];

    memset(self, 0, sizeof(struct light_wrapper));
}

void bsp_wrapper_light_set_operation_object(const char * name)
{
    for(uint32_t i = 0; i < ARRAY_SIZE(light_wrappers); i++) {
        if(strcmp(light_wrappers[i].name, name) == 0) {
            current_light_idx = i;
            break;
        }
    }
}

void bsp_wrapper_light_on(void)
{
    struct light_wrapper * self = &light_wrappers[current_light_idx];

    self->pf_on(self);
}

void bsp_wrapper_light_off(void)
{
    struct light_wrapper * self = &light_wrappers[current_light_idx];

    self->pf_off(self);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
