/**
  ******************************************************************************
  *
  * @file    dwt.h
  * @author  Jamin
  * @brief   Header file of dwt module.
  *
  ******************************************************************************
  **/

#ifndef _DWT_H
#define _DWT_H


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
void STD_DWT_Init(void);
void STD_DWT_DelayUs(uint32_t us);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_DWT_H*/


/******************************* (END OF FILE) *********************************/

