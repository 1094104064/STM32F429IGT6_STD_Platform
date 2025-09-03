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
static int8_t current_light_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_wrapper_light_link(struct light_wrapper * self)
{
    int8_t idx = 0;

    current_light_idx++;

    if((0 <= current_light_idx) && (current_light_idx < LIGHT_MAX_NUM)) {
        idx = current_light_idx;
    }
    else {
        current_light_idx = 0;
    }

    memset(&light_wrappers[idx], 0, sizeof(struct light_wrapper));

    light_wrappers[idx] = *self;
    light_wrappers[idx].idx = idx;

}

bool bsp_wrapper_light_init(void)
{
    int ret = 0;
    struct light_wrapper * self = &light_wrappers[current_light_idx];

    if(self->pf_init) {
        ret = self->pf_init(self);
        if(ret != 0) {
            pr_error("Light wrapper init failed: %s, error: %d", self->name, ret);
            return false;
        }
        else {
            pr_info("Light wrapper init succeeded: %s", self->name);
        }
    }
    else {
        pr_error("Light wrapper init function not defined: %s", self->name);
    }

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

    if(self->pf_on) {
        self->pf_on(self);
    }
}

void bsp_wrapper_light_off(void)
{
    struct light_wrapper * self = &light_wrappers[current_light_idx];

    if(self->pf_off) {
        self->pf_off(self);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
