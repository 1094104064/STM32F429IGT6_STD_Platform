/**
  ******************************************************************************
  *
  * @file    systick.c
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
#include "systick.h"
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
static volatile uint32_t uwTick = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void STD_SYSTICK_Init(void)
{
    /* 1ms */
    SysTick_Config(SystemCoreClock / 1000);
}

void STD_SYSTICK_Inc(uint32_t tick_freq)
{
    uwTick += tick_freq;
}

uint32_t STD_SYSTICK_Get(void)
{
    return uwTick;
}

void STD_SYSTICK_Delay(uint32_t delay)
{
    uint32_t start = uwTick;
    while (uwTick - start < delay);
}

void STD_SYSTICK_Suspend(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void STD_SYSTICK_Resume(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
