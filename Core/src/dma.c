/**
  ******************************************************************************
  *
  * @file    dma.c
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
#include "dma.h"
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
void STD_DMA_ClockCmd(DMA_TypeDef* DMAx, FunctionalState NewState)
{
    if(DMAx == DMA1) {
        if (NewState != DISABLE) {
            /* Enable the DMA clock */
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        }
        else {
            /* Disable the DMA clock */
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, DISABLE);
        }
    }
    else if(DMAx == DMA2) {
        if (NewState != DISABLE) {
            /* Enable the DMA clock */
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
        }
        else {
            /* Disable the DMA clock */
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, DISABLE);
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

