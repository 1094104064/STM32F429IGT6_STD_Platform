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
static bool delay_init(struct delay_driver * self);
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
    
    self->pf_init       = delay_init;
    self->pf_delay_us   = delay_us;
    self->pf_delay_ms   = delay_ms;
    self->pf_delay_sec  = delay_sec;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool delay_init(struct delay_driver * self)
{
    if(self->oper->pf_delay_us  == NULL ||
       self->oper->pf_delay_ms  == NULL ||
       self->oper->pf_delay_sec == NULL) {
        return false;
    }

    return true;
}


static void delay_us(struct delay_driver * self, uint32_t us)
{
    self->oper->pf_delay_us(us);
}

static void delay_ms(struct delay_driver * self, uint32_t ms)
{
    self->oper->pf_delay_ms(ms);
}

static void delay_sec(struct delay_driver * self, uint32_t sec)
{
    self->oper->pf_delay_sec(sec);
}

/******************************* (END OF FILE) *********************************/

