/**
  ******************************************************************************
  *
  * @file    bsp_adapter_delay.c
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
#include "bsp_adapter_delay.h"
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
static int bsp_adapter_delay_init(struct delay_wrapper * self);
static void bsp_adapter_delay_us(struct delay_wrapper * self, uint32_t us);
static void bsp_adapter_delay_ms(struct delay_wrapper * self, uint32_t ms);
static void bsp_adapter_delay_sec(struct delay_wrapper * self, uint32_t sec);
/**********************
 *  STATIC VARIABLES
 **********************/

static struct delay_driver gs_delay_instance;

static const struct delay_class gsc_delay_class = {
    .pf_delay_us = bsp_port_delay_us,
    .pf_delay_ms = bsp_port_delay_ms,
    .pf_delay_sec = bsp_port_delay_sec,
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
delay_wrapper_t * bsp_adapter_delay_register(const char * const name, void * const user_data)
{   
    delay_wrapper_t wrapper = 
    {
        .pf_init        = bsp_adapter_delay_init,
        .pf_delay_us    = bsp_adapter_delay_us,
        .pf_delay_ms    = bsp_adapter_delay_ms,
        .pf_delay_sec   = bsp_adapter_delay_sec,
    };

    return bsp_wrapper_delay_create(&wrapper, name, user_data);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static int bsp_adapter_delay_init(struct delay_wrapper * self)
{
    bsp_driver_delay_instantiate(&gs_delay_instance, &gsc_delay_class);

    if( gs_delay_instance.pf_init       == NULL || gs_delay_instance.pf_delay_us    == NULL ||
        gs_delay_instance.pf_delay_ms   == NULL || gs_delay_instance.pf_delay_sec   == NULL) {
        return 1;
    }

    if(gs_delay_instance.pf_init(&gs_delay_instance) == false) {
        return 2;
    }

    return 0;
}

static void bsp_adapter_delay_us(struct delay_wrapper * self, uint32_t us)
{
    gs_delay_instance.pf_delay_us(&gs_delay_instance, us);
}

static void bsp_adapter_delay_ms(struct delay_wrapper * self, uint32_t ms)
{
    gs_delay_instance.pf_delay_ms(&gs_delay_instance, ms);
}

static void bsp_adapter_delay_sec(struct delay_wrapper * self, uint32_t sec)
{
    gs_delay_instance.pf_delay_sec(&gs_delay_instance, sec);
}

/******************************* (END OF FILE) *********************************/

