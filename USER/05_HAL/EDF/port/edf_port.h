/**
  ******************************************************************************
  *
  * @file    edf_port.h
  * @author  
  * @brief   Header file of edf_port module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_PORT_H
#define _EDF_PORT_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "edf_macro.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void edf_interrupt_disable(void);
void edf_interrupt_enable(void);

EDF_WEAK void edf_delay_us(uint32_t us);
EDF_WEAK void edf_delay_ms(uint32_t ms);

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_EDF_PORT_H*/


/******************************* (END OF FILE) *********************************/


