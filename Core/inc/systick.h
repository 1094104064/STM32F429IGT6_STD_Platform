/**
  ******************************************************************************
  *
  * @file    systick.h
  * @author  Jamin
  * @brief   Header file of systick module.
  *
  ******************************************************************************
  **/

#ifndef _SYSTICK_H
#define _SYSTICK_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "stm32f4xx.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void STD_SYSTICK_Init(void);
void STD_SYSTICK_Inc(uint32_t tick_freq);
uint32_t STD_SYSTICK_Get(void);
void STD_SYSTICK_Delay(uint32_t delay);
void STD_SYSTICK_Suspend(void);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_SYSTICK_H*/


/******************************* (END OF FILE) *********************************/

