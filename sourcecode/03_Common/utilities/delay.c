/**
  ******************************************************************************
  *
  * @file    delay.c
  * @author  
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
#include "delay.h"
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
static delay_cb_t delay_callback = NULL;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void delay_init(delay_cb_t delay_cb)
{
    delay_callback = delay_cb;
}

 
void delay_us(uint32_t num)
{
    delay_callback(num);
}

void delay_ms(uint32_t num)
{
    for(uint32_t i = 0; i < num; i++) {
        delay_callback(1000);
    }
}

void delay_s(uint32_t num)
{
    for(uint32_t i = 0; i < num; i++) {
        delay_ms(1000);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







