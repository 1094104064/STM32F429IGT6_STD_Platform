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
static int8_t delay_wrapper_count = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void bsp_wrapper_delay_link(struct delay_wrapper * self)
{
    delay_wrapper_count++;
    if(delay_wrapper_count >= DELAY_MAX_NUM) {
        delay_wrapper_count = DELAY_MAX_NUM - 1;
    }

    int8_t idx = delay_wrapper_count;

    delay_wrappers[idx].idx = self->idx;
    delay_wrappers[idx].user_data= self->user_data;
    delay_wrappers[idx].init = self->init;
    delay_wrappers[idx].delay_us = self->delay_us;
    delay_wrappers[idx].delay_ms = self->delay_ms;
    delay_wrappers[idx].delay_sec = self->delay_sec;
}

void bsp_wrapper_delay_init(void)
{
    struct delay_wrapper * self = &delay_wrappers[delay_wrapper_count];

    if(self->init) {
        self->init(self);
    }
}

void bsp_wrapper_delay_us(uint32_t us)
{
    struct delay_wrapper * self = &delay_wrappers[delay_wrapper_count];

    if(self->delay_us) {
        self->delay_us(self, us);
    }
}

void bsp_wrapper_delay_ms(uint32_t ms)
{
    struct delay_wrapper * self = &delay_wrappers[delay_wrapper_count];

    if(self->delay_ms) {
        self->delay_ms(self, ms);
    }
}

void bsp_wrapper_delay_sec(uint32_t sec)
{
    struct delay_wrapper * self = &delay_wrappers[delay_wrapper_count];

    if(self->delay_sec) {
        self->delay_sec(self, sec);
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

