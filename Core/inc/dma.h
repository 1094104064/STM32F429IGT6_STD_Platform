/**
  ******************************************************************************
  *
  * @file    dma.h
  * @author  Jamin
  * @brief   Header file of dma module.
  *
  ******************************************************************************
  **/
  
#ifndef _DMA_H
#define _DMA_H


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
void STD_DMA_ClockCmd(DMA_TypeDef* DMAx, FunctionalState NewState);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_DMA_H*/


/******************************* (END OF FILE) *********************************/

