/**
  ******************************************************************************
  *
  * @file    bsp_adapter_light.c
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
#include "bsp_adapter_light.h"
#include "bsp_wrapper_light.h"
#include "bsp_driver_led.h"
#include "bsp_port_led.h"
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
static int bsp_adapter_light_init(struct light_wrapper * self);
static void bsp_adapter_light_on(struct light_wrapper * self);
static void bsp_adapter_light_off(struct light_wrapper * self);
/**********************
 *  STATIC VARIABLES
 **********************/

static const char * blue_led = "blue_led";
static const char * yellow_led = "yellow_led";

static struct led_driver blue_led_driver;
static struct led_oper    blue_led_oper = {
    .pf_on = bsp_port_blue_led_on,
    .pf_off = bsp_port_blue_led_off,
};

static struct led_driver yellow_led_driver;
static struct led_oper    yellow_led_oper = {
    .pf_on = bsp_port_yellow_led_on,
    .pf_off = bsp_port_yellow_led_off,
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_light_register(void)
{
    struct light_wrapper blue_led_wrapper = {
        .idx        = 0,
        .user_data  = NULL,
        .name       = blue_led,

        .pf_init    = bsp_adapter_light_init,
        .pf_on      = bsp_adapter_light_on,
        .pf_off     = bsp_adapter_light_off,
    };

    bsp_wrapper_light_link(&blue_led_wrapper);

    struct light_wrapper yellow_led_wrapper = {
        .idx = 1,
        .user_data  = NULL,
        .name       = yellow_led,

        .pf_init    = bsp_adapter_light_init,
        .pf_on      = bsp_adapter_light_on,
        .pf_off     = bsp_adapter_light_off,
    };
    bsp_wrapper_light_link(&yellow_led_wrapper);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static int bsp_adapter_light_init(struct light_wrapper * self)
{
    if(self->name == blue_led) {

        bsp_driver_led_link(&blue_led_driver, &blue_led_oper);

        if(blue_led_driver.pf_init == NULL || blue_led_driver.pf_on == NULL ||
           blue_led_driver.pf_off  == NULL) {
            return 1;
        }

        if (blue_led_driver.pf_init(&blue_led_driver) == false) {
            return 2;
        }
    }
    else if(self->name == yellow_led) {

        bsp_driver_led_link(&yellow_led_driver, &yellow_led_oper);

        if(yellow_led_driver.pf_init == NULL || yellow_led_driver.pf_on == NULL ||
           yellow_led_driver.pf_off  == NULL) {
            return 1;
        }

        if (yellow_led_driver.pf_init(&yellow_led_driver) == false) {
            return 2;
        }
    }
    else {
        pr_error("Unknown light name");
        return 3;
    }

    return 0;
}

static void bsp_adapter_light_on(struct light_wrapper * self)
{
    if(self->name == blue_led) {
        blue_led_driver.pf_on(&blue_led_driver);
    }
    else if(self->name == yellow_led) {
        yellow_led_driver.pf_on(&yellow_led_driver);
    }
    else {
        pr_error("Unknown light name");
    }
}

static void bsp_adapter_light_off(struct light_wrapper * self)
{
    if(self->name == blue_led) {
        blue_led_driver.pf_off(&blue_led_driver);
    }
    else if(self->name == yellow_led) {
        yellow_led_driver.pf_off(&yellow_led_driver);
    }
    else {
        pr_error("Unknown light name");
    }
}

/******************************* (END OF FILE) *********************************/
