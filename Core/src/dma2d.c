/**
  ******************************************************************************
  *
  * @file    dma2d.c
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
#include "dma2d.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 
struct framebuffer_layer layers[2];
static struct framebuffer_layer *active_layer = &layers[0];
/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void STD_DMA2D_ClockCmd(FunctionalState NewState)
{
    if (NewState != DISABLE) {
        /* Enable the DMA2D clock */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
    }
    else {
        /* Disable the DMA2D clock */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, DISABLE);
    }
}

void STD_DMA2D_SwitchLayer(uint8_t layer)
{
    if (layer >= 2) {
        return;
    }

    active_layer = &layers[layer];
}

struct framebuffer_layer * STD_DMA2D_GetActiveLayer(void)
{
    return active_layer;
}

void STD_DMA2D_FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    DMA2D->CR &= ~(DMA2D_CR_START);                 /* 停止DMA2D */
    DMA2D->CR = DMA2D_R2M;                          /* 寄存器到SDRAM */
    DMA2D->OPFCCR = active_layer->pixel_format;                  /* 设置颜色格式 */
    DMA2D->OCOLR = color;                   /* 颜色 */

    if (active_layer->rotated == 0) {
        DMA2D->OOR = active_layer->width - width;
        DMA2D->OMAR = active_layer->start_address + active_layer->pixel_size * (active_layer->width * y + x);
        DMA2D->NLR = (width << 16) | (height);
    }
    else if(active_layer->rotated == 90) { 
        DMA2D->OOR = active_layer->width - height;
        DMA2D->OMAR = active_layer->start_address + 
                    active_layer->pixel_size * 
                    (y * active_layer->width + (active_layer->width - x - width - 50));
        DMA2D->NLR = (height << 16) | width;
    }
    else if(active_layer->rotated == 180) {
        DMA2D->OOR = active_layer->width - width;
        DMA2D->OMAR = active_layer->start_address + 
                    active_layer->pixel_size * 
                    ((active_layer->height - y - height) * active_layer->width + 
                    (active_layer->width - x - width));
        DMA2D->NLR = (width << 16) | height;
    }
    else if(active_layer->rotated == 270) {
        DMA2D->OOR = active_layer->width - height;
        DMA2D->OMAR = active_layer->start_address + 
                      active_layer->pixel_size * 
                      ((active_layer->height - x - 1 - width) * active_layer->width + y);
        DMA2D->NLR = (width) | (height << 16);
    }

    DMA2D->CR |= DMA2D_CR_START;

    while (DMA2D->CR & DMA2D_CR_START);
}

void STD_DMA2D_FillScreen(uint32_t color)
{
    DMA2D->CR &= ~(DMA2D_CR_START);                 /* 停止DMA2D */
    DMA2D->CR = DMA2D_R2M;                          /* 寄存器到SDRAM */
    DMA2D->OPFCCR = active_layer->pixel_format;                  /* 设置颜色格式 */
    DMA2D->OOR = 0;                                 /* 设置行偏移 */
    DMA2D->OMAR = active_layer->start_address;               /* 地址 */
    DMA2D->NLR = (active_layer->width << 16) | (active_layer->height);  /* 设定长度和宽度 */
    DMA2D->OCOLR = color;                   /* 颜色 */


    /* 等待垂直数据使能显示状态 ，即LTDC即将刷完一整屏数据的时候 */
    /* 因为在屏幕没有刷完一帧时进行刷屏，会有撕裂的现象 */
    /* 用户也可以使用 寄存器重载中断 进行判断，不过为了保证例程的简洁以及移植的方便性，这里直接使用判断寄存器的方法 */
    /* 如果不做判断，DMA2D刷一屏仅需0.6ms，加了之后，需要4.8ms刷一屏，不过屏幕本身的刷新率只有60帧，
       加上4.8ms的速度已经很快了，除非是对速度要求特别高的场合，不然建议加上判断垂直等待的语句，可以避免撕裂效应  */

    while (LTDC->CDSR != 0X00000001);    /* 判断显示状态寄存器LTDC_CDSR 的第0位 VDES：垂直数据使能显示状态 */

    DMA2D->CR |= DMA2D_CR_START; 

    while (DMA2D->CR & DMA2D_CR_START); /* 等待传输完成 */    
}

void STD_DMA2D_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    DMA2D->CR &= ~(DMA2D_CR_START);                 // 停止DMA2D
    DMA2D->CR = DMA2D_M2M;                          // 存储器到存储器模式
    DMA2D->FGPFCCR = active_layer->pixel_format;         // 前景颜色格式（如RGB565或RGB888）
    DMA2D->FGMAR = (uint32_t)data;                  // 源缓冲区地址（LVGL的颜色缓存）
    DMA2D->FGOR    =  0;

    DMA2D->OMAR = active_layer->start_address + active_layer->pixel_size * (y * active_layer->width + x); // 目标显存地址
    DMA2D->OOR = active_layer->width - width;                // 行偏移
    DMA2D->NLR = (width << 16) | height;           // 宽高
    
    DMA2D->CR |= DMA2D_CR_START;                   // 启动DMA2D

    while (DMA2D->CR & DMA2D_CR_START);            // 等待DMA2D完成
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

