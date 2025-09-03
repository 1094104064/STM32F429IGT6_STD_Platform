/**
  ******************************************************************************
  *
  * @file    bsp_port_delay.h
  * @author  Jamin
  * @brief   Header file of bsp_port_delay module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_PORT_DELAY_H
#define _BSP_PORT_DELAY_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "systick.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void bsp_port_delay_init(void)
{

}


static inline void bsp_port_delay_us(uint32_t us)
{
    uint32_t cycles = us * (SystemCoreClock / 1000000UL) / 4; // 根据指令周期调整
    while(cycles--) {
        __asm volatile ("nop"); // 空操作指令
    }
}

static inline void bsp_port_delay_ms(uint32_t ms)
{
    uint32_t start = STD_SYSTICK_Get();
    while (STD_SYSTICK_Get() - start < ms);
}

static inline void bsp_port_delay_sec(uint32_t sec)
{
    uint32_t start = STD_SYSTICK_Get();
    while (STD_SYSTICK_Get() - start < sec * 1000);
}

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_SYSTICK_H*/


/******************************* (END OF FILE) *********************************/

