/**
  ******************************************************************************
  *
  * @file    bsp_driver_debug.c
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
#include "bsp_driver_debug.h"
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
static void debug_init(struct debug_driver * self);
static void debug_putc(struct debug_driver * self, char c);
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_debug_link(struct debug_driver * self, struct debug_oper * oper)
{
    if (self == NULL || oper == NULL) {
        return;
    }

    self->oper = oper;

    self->init = debug_init;
    self->putc = debug_putc;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void debug_init(struct debug_driver * self)
{
    if (self == NULL || self->oper == NULL || self->oper->init == NULL) {
        return;
    }

    self->oper->init();
}

static void debug_putc(struct debug_driver * self, char c)
{
    if (self == NULL || self->oper == NULL || self->oper->putc == NULL) {
        return;
    }

    self->oper->putc(c);
}

/******************************* (END OF FILE) *********************************/

