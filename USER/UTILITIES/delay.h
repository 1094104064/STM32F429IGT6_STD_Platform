/**
  ******************************************************************************
  *
  * @file    delay.h
  * @author  
  * @brief   Header file of delay module.
  *
  ******************************************************************************
  **/
  
#ifndef _DELAY_H
#define _DELAY_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdint.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void delay_init(void);
void delay_us(uint32_t num);
void delay_ms(uint32_t num);
void delay_s(uint32_t num);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_DELAY_H*/


/******************************* (END OF FILE) *********************************/


