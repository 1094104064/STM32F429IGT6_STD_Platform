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
#include "bsp_wrapper_delay.h"
#include "bsp_driver_delay.h"
#include "bsp_port_delay.h"
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

static struct delay_driver delay_drv;
static struct delay_oper delay_ops = {
    .pf_delay_us = bsp_port_delay_us,
    .pf_delay_ms = bsp_port_delay_ms,
    .pf_delay_sec = bsp_port_delay_sec,
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_delay_register(void)
{   
    struct delay_wrapper wrapper = {
        .idx = 0,
        .user_data = NULL,
        .init = bsp_adapter_delay_init,
        .delay_us = bsp_adapter_delay_us,
        .delay_ms = bsp_adapter_delay_ms,
        .delay_sec = bsp_adapter_delay_sec,
    };

    bsp_wrapper_delay_link(&wrapper);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static int bsp_adapter_delay_init(struct delay_wrapper * self)
{
    bsp_driver_delay_link(&delay_drv, &delay_ops);

    if( delay_drv.pf_init       == NULL || delay_drv.pf_delay_us    == NULL ||
        delay_drv.pf_delay_ms   == NULL || delay_drv.pf_delay_sec   == NULL) {
        return 1;
    }

    if(delay_drv.pf_init(&delay_drv) == false) {
        return 2;
    }

    return 0;
}

static void bsp_adapter_delay_us(struct delay_wrapper * self, uint32_t us)
{
    delay_drv.pf_delay_us(&delay_drv, us);
}

static void bsp_adapter_delay_ms(struct delay_wrapper * self, uint32_t ms)
{
    delay_drv.pf_delay_ms(&delay_drv, ms);
}

static void bsp_adapter_delay_sec(struct delay_wrapper * self, uint32_t sec)
{
    delay_drv.pf_delay_sec(&delay_drv, sec);
}

/******************************* (END OF FILE) *********************************/

