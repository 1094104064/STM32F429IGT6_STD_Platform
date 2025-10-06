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
void bsp_driver_delay_instantiate(struct delay_driver * self, const struct delay_class * p_class)
{
    delay_assert_null(self);
    delay_assert_null(p_class);

    if(self == NULL || p_class == NULL) {
        return;
    }

    self->p_class = p_class;

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
    delay_assert_null(self->p_class->pf_delay_us);
    delay_assert_null(self->p_class->pf_delay_ms);
    delay_assert_null(self->p_class->pf_delay_sec);

    if(self->p_class->pf_delay_us  == NULL ||
       self->p_class->pf_delay_ms  == NULL ||
       self->p_class->pf_delay_sec == NULL) {
        return false;
    }

    delay_dbg("delay init successfully");

    return true;
}


static void delay_us(struct delay_driver * self, uint32_t us)
{
    self->p_class->pf_delay_us(us);
}

static void delay_ms(struct delay_driver * self, uint32_t ms)
{
    self->p_class->pf_delay_ms(ms);
}

static void delay_sec(struct delay_driver * self, uint32_t sec)
{
    self->p_class->pf_delay_sec(sec);
}

/******************************* (END OF FILE) *********************************/

