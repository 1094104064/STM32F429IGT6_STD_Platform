/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_delay.c
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
#include "bsp_wrapper_delay.h"
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
static struct delay_wrapper delay_wrappers[DELAY_MAX_NUM];
static uint8_t current_delay_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void bsp_wrapper_delay_link(struct delay_wrapper * self)
{
    uint8_t idx = 0;

    current_delay_idx++;

    if(current_delay_idx < DELAY_MAX_NUM) {
        idx = current_delay_idx;
    }
    else {
        current_delay_idx = 0;
    }

    memset(&delay_wrappers[idx], 0, sizeof(struct delay_wrapper));

    delay_wrappers[idx] = *self;
    delay_wrappers[idx].idx = idx;
}

bool bsp_wrapper_delay_init(void)
{
    int ret = 0;
    struct delay_wrapper * self = &delay_wrappers[current_delay_idx];

    if( self->pf_init       == NULL || self->pf_delay_us    == NULL ||
        self->pf_delay_ms   == NULL || self->pf_delay_sec   == NULL) {
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

void bsp_wrapper_delay_us(uint32_t us)
{
    struct delay_wrapper * self = &delay_wrappers[current_delay_idx];

    self->pf_delay_us(self, us);
}

void bsp_wrapper_delay_ms(uint32_t ms)
{
    struct delay_wrapper * self = &delay_wrappers[current_delay_idx];

    self->pf_delay_ms(self, ms);
}

void bsp_wrapper_delay_sec(uint32_t sec)
{
    struct delay_wrapper * self = &delay_wrappers[current_delay_idx];

    self->pf_delay_sec(self, sec);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

