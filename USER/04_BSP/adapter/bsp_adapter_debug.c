/**
  ******************************************************************************
  *
  * @file    bsp_adapter_debug.c
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
#include "bsp_adapter_debug.h"
#include "bsp_wrapper_debug.h"
#include "bsp_driver_debug.h"
#include "bsp_port_debug.h"
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
static void bsp_adapter_debug_init(struct debug_wrapper * self);
static void bsp_adapter_debug_putc(struct debug_wrapper * self, char c);
/**********************
 *  STATIC VARIABLES
 **********************/

static struct debug_driver driver;
static struct debug_oper    oper = {
    .init = bsp_port_debug_init,
    .putc = bsp_port_debug_putc,
};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_debug_register(void)
{
    struct debug_wrapper wrapper = {
        .idx = 0,
        .user_data = NULL,
        .init = bsp_adapter_debug_init,
        .putc = bsp_adapter_debug_putc,
    };

    bsp_wrapper_debug_link(&wrapper);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static void bsp_adapter_debug_init(struct debug_wrapper * self)
{
    bsp_driver_debug_link(&driver, &oper);

    driver.init(&driver);
}

static void bsp_adapter_debug_putc(struct debug_wrapper * self, char c)
{
    driver.putc(&driver, c);
}


/******************************* (END OF FILE) *********************************/

