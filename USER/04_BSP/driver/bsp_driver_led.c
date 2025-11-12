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
static bool led_init(led_driver_t * self);
static void led_on(led_driver_t * self);
static void led_off(led_driver_t * self);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_led_link(led_driver_t * drv, const led_handle_t * handle)
{
    led_assert_null(drv);
    led_assert_null(handle);

    if (drv == NULL || handle == NULL) {
        return;
    }

    drv->handle   = handle;

    drv->pf_init   = led_init;
    drv->pf_on     = led_on;
    drv->pf_off    = led_off;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool led_init(led_driver_t * self)
{
    led_assert_null(self->handle->pf_on);
    led_assert_null(self->handle->pf_off);
    led_assert_null(self->handle->pf_hal_init);

    
    if( self->handle->pf_hal_init   == NULL   ||
        self->handle->pf_on         == NULL   ||
        self->handle->pf_off        == NULL )

        return false;

    self->handle->pf_hal_init();
    led_dbg("led init successfully");

    return true;
}

static void led_on(led_driver_t * self)
{
    self->handle->pf_on();
}

static void led_off(led_driver_t * self)
{
    self->handle->pf_off();
}

/******************************* (END OF FILE) *********************************/
