/**
  ******************************************************************************
  *
  * @file    dma2d.h
  * @author  Jamin
  * @brief   Header file of dma2d module.
  *
  ******************************************************************************
  **/
  
#ifndef _DMA2D_H
#define _DMA2D_H


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
struct framebuffer_layer {
    uint8_t pixel_format;
    uint8_t pixel_size;
    uint16_t rotated;
    uint16_t width;
    uint16_t height;
    uint32_t start_address;
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void STD_DMA2D_ClockCmd(FunctionalState NewState);
void STD_DMA2D_SwitchLayer(uint8_t layer);
struct framebuffer_layer * STD_DMA2D_GetActiveLayer(void);
void STD_DMA2D_FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void STD_DMA2D_FillScreen(uint32_t color);
void STD_DMA2D_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_DMA2D_H*/


/******************************* (END OF FILE) *********************************/

