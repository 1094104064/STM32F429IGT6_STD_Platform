/**
  ******************************************************************************
  *
  * @file    bsp_port_debug.h
  * @author  Jamin
  * @brief   Header file of bsp_port_debug module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_PORT_DEBUG_H
#define _BSP_PORT_DEBUG_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "usart.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void bsp_port_debug_init(void)
{

}

static inline void bsp_port_debug_putc(char c)
{
    /*!< Send single byte */
    USART_SendData(USART1, (uint8_t)c);	
    
    /*!< Waiting for sending to complete */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
}

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_DEBUG_H*/


/******************************* (END OF FILE) *********************************/

