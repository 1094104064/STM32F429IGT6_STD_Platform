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
#include "systick.h"
#include "dwt.h"
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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
 
void delay_init(void)
{
    STD_DWT_Init();
    STD_SYSTICK_Init();
}
 
void delay_us(uint32_t num)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t delay = num * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < delay);
}

void delay_ms(uint32_t num)
{
    uint32_t start = STD_SYSTICK_Get();
    while (STD_SYSTICK_Get() - start < num);
}

void delay_s(uint32_t num)
{
    uint32_t start = STD_SYSTICK_Get();
    while (STD_SYSTICK_Get() - start < num * 1000);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







