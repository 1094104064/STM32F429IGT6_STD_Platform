/**
  ******************************************************************************
  *
  * @file    usart.h
  * @author  Jamin
  * @brief   Header file of usart module.
  *
  ******************************************************************************
  **/
  
#ifndef _USART_H
#define _USART_H


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
void STD_USART1_Init(void);
void STD_USART1_NVIC_Init(void);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_USART_H*/


/******************************* (END OF FILE) *********************************/

