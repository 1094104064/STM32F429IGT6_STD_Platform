/**
  ******************************************************************************
  *
  * @file    bsp_driver_delay.c
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
#include "bsp_driver_delay.h"
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
static void delay_init(struct delay_driver * self);
static void delay_us(struct delay_driver * self, uint32_t us);
static void delay_ms(struct delay_driver * self, uint32_t ms);
static void delay_sec(struct delay_driver * self, uint32_t sec);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_delay_link(struct delay_driver * self, struct delay_oper * oper)
{
    if(self == NULL || oper == NULL) {
        return;
    }

    self->oper = oper;

    self->init = delay_init;
    self->delay_us = delay_us;
    self->delay_ms = delay_ms;
    self->delay_sec = delay_sec;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static void delay_init(struct delay_driver * self)
{
    if(self == NULL || self->oper == NULL) {
        return;
    }

    self->oper->init();
}


static void delay_us(struct delay_driver * self, uint32_t us)
{
    if(self == NULL || self->oper == NULL) {
        return;
    }

    self->oper->delay_us(us);
}

static void delay_ms(struct delay_driver * self, uint32_t ms)
{
    if(self == NULL || self->oper == NULL) {
        return;
    }

    self->oper->delay_ms(ms);
}

static void delay_sec(struct delay_driver * self, uint32_t sec)
{
    if(self == NULL || self->oper == NULL) {
        return;
    }

    self->oper->delay_sec(sec);
}

/******************************* (END OF FILE) *********************************/

