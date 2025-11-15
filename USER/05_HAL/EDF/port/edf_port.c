/**
  ******************************************************************************
  *
  * @file    edf_port.c
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
#include "edf_port.h"
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
/**
 * @brief This function disable the interrupt.
 */
void edf_interrupt_disable(void)
{

}

/**
 * @brief This function enable the interrupt.
 */
void edf_interrupt_enable(void)
{

}

/**
 * @brief This function delay us.
 *
 * @param us The delay time.
 */
EDF_WEAK void edf_delay_us(uint32_t us)
{
#ifndef MR_CFG_SYSCLK_FREQ
#define MR_CFG_SYSCLK_FREQ              (72000000)
#endif /* MR_CFG_SYSCLK_FREQ */
#if (MR_CFG_SYSCLK_FREQ > 1000000)
#define MR_DELAY_COUNT                  (MR_CFG_SYSCLK_FREQ / 1000000)
#else
#define MR_DELAY_COUNT                  (1)
#endif /* (MR_CFG_SYSCLK_FREQ > 1000000) */
    for (volatile uint32_t i = 0; i < us * MR_DELAY_COUNT; i++) {
        __asm__("nop");
    }
#undef MR_DELAY_COUNT
#undef MR_CFG_SYSCLK_FREQ
}

/**
 * @brief This function delay ms.
 *
 * @param ms The delay time.
 */
EDF_WEAK void edf_delay_ms(uint32_t ms)
{
    for (volatile uint32_t i = 0; i < ms; i++) {
        edf_delay_us(1000);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







