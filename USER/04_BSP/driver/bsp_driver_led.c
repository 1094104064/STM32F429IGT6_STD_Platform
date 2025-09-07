/**
  ******************************************************************************
  *
  * @file    bsp_driver_led.c
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
#include "bsp_driver_led.h"
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
static bool led_init(struct led_driver * self);
static void led_on(struct led_driver * self);
static void led_off(struct led_driver * self);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_led_link(struct led_driver * self, struct led_oper * oper)
{
    led_assert_null(self);
    led_assert_null(oper);

    if (self == NULL || oper == NULL) {
        return;
    }

    self->oper = oper;

    self->pf_init   = led_init;
    self->pf_on     = led_on;
    self->pf_off    = led_off;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool led_init(struct led_driver * self)
{
    led_assert_null(self->oper->pf_on);
    led_assert_null(self->oper->pf_off);

    if( self->oper->pf_on   == NULL ||
        self->oper->pf_off  == NULL)

        return false;

    led_dbg("led init successfully");

    return true;
}

static void led_on(struct led_driver * self)
{
    self->oper->pf_on();
}

static void led_off(struct led_driver * self)
{
    self->oper->pf_off();
}

/******************************* (END OF FILE) *********************************/
