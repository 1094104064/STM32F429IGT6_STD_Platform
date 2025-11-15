/**
  ******************************************************************************
  *
  * @file    bsp_adapter_led.c
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
#include "bsp_adapter_led.h"
#include "bsp_wrapper_light.h"
#include "bsp_driver_led.h"
#include "bsp_linker_led.h"
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

static led_driver_t gs_led1_driver;

static led_driver_t gs_led2_driver;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int ops_led1_init(void)
{
    static const led_handle_t blue_led_handle = {
        .pf_hal_init    = bsp_linker_led1_init,
        .pf_on          = bsp_linker_led1_on,
        .pf_off         = bsp_linker_led1_off,
    };

    bsp_driver_led_link(&gs_led1_driver, &blue_led_handle);

    if (gs_led1_driver.pf_init(&gs_led1_driver) == false) {
        return 2;
    }

    return 0;
}

static void ops_led1_on(void)
{
    gs_led1_driver.pf_on(&gs_led1_driver);
}

static void ops_led1_off(void)
{
    gs_led1_driver.pf_off(&gs_led1_driver);
}

const light_ops_t blue_led_ops = {
    .pf_init    = ops_led1_init,
    .pf_on      = ops_led1_on,
    .pf_off     = ops_led1_off,
};

static int ops_led2_init(void)
{
   static const led_handle_t yellow_led_handle = {
       .pf_hal_init    = bsp_linker_led2_init,
       .pf_on          = bsp_linker_led2_on,
       .pf_off         = bsp_linker_led2_off,
   };

   bsp_driver_led_link(&gs_led2_driver, &yellow_led_handle);

   if (gs_led2_driver.pf_init(&gs_led2_driver) == false) {
       return 2;
   }

   return 0;
}

static void ops_led2_on(void)
{
    gs_led2_driver.pf_on(&gs_led2_driver);
}

static void ops_led2_off(void)
{
    gs_led2_driver.pf_off(&gs_led2_driver);
}

const light_ops_t yellow_led_ops = {
    .pf_init    = ops_led2_init,
    .pf_on      = ops_led2_on,
    .pf_off     = ops_led2_off,
};

/******************************* (END OF FILE) *********************************/
