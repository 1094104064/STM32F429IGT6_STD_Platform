/**
  ******************************************************************************
  *
  * @file    ltdc.h
  * @author  Jamin
  * @brief   Header file of ltdc module.
  *
  ******************************************************************************
  **/

#ifndef _LTDC_H
#define _LTDC_H


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

#define LCD_GPIO_CLK	      RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF |\
                              RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI

#define LTDC_R2_PORT          GPIOH
#define LTDC_R2_PIN           GPIO_Pin_8
#define LTDC_R2_PINSOURCE     GPIO_PinSource8

#define LTDC_R3_PORT          GPIOH
#define LTDC_R3_PIN           GPIO_Pin_9
#define LTDC_R3_PINSOURCE     GPIO_PinSource9

#define LTDC_R4_PORT          GPIOH
#define LTDC_R4_PIN           GPIO_Pin_10
#define LTDC_R4_PINSOURCE     GPIO_PinSource10

#define LTDC_R5_PORT          GPIOH
#define LTDC_R5_PIN           GPIO_Pin_11
#define LTDC_R5_PINSOURCE     GPIO_PinSource11

#define LTDC_R6_PORT          GPIOB
#define LTDC_R6_PIN           GPIO_Pin_1
#define LTDC_R6_PINSOURCE     GPIO_PinSource1
#define LTDC_R6_AF			  GPIO_AF9_LTDC		

#define LTDC_R7_PORT          GPIOG
#define LTDC_R7_PIN           GPIO_Pin_6
#define LTDC_R7_PINSOURCE     GPIO_PinSource6

#define LTDC_G2_PORT          GPIOH
#define LTDC_G2_PIN           GPIO_Pin_13
#define LTDC_G2_PINSOURCE     GPIO_PinSource13

#define LTDC_G3_PORT          GPIOH
#define LTDC_G3_PIN           GPIO_Pin_14
#define LTDC_G3_PINSOURCE     GPIO_PinSource14

#define LTDC_G4_PORT          GPIOH
#define LTDC_G4_PIN           GPIO_Pin_15
#define LTDC_G4_PINSOURCE     GPIO_PinSource15

#define LTDC_G5_PORT          GPIOI
#define LTDC_G5_PIN           GPIO_Pin_0
#define LTDC_G5_PINSOURCE     GPIO_PinSource0

#define LTDC_G6_PORT          GPIOI
#define LTDC_G6_PIN           GPIO_Pin_1
#define LTDC_G6_PINSOURCE     GPIO_PinSource1

#define LTDC_G7_PORT          GPIOI
#define LTDC_G7_PIN           GPIO_Pin_2
#define LTDC_G7_PINSOURCE     GPIO_PinSource2

#define LTDC_B2_PORT          GPIOD
#define LTDC_B2_PIN           GPIO_Pin_6
#define LTDC_B2_PINSOURCE     GPIO_PinSource6

#define LTDC_B3_PORT          GPIOG
#define LTDC_B3_PIN           GPIO_Pin_11
#define LTDC_B3_PINSOURCE     GPIO_PinSource11

#define LTDC_B4_PORT          GPIOI
#define LTDC_B4_PIN           GPIO_Pin_4
#define LTDC_B4_PINSOURCE     GPIO_PinSource4

#define LTDC_B5_PORT          GPIOI
#define LTDC_B5_PIN           GPIO_Pin_5
#define LTDC_B5_PINSOURCE     GPIO_PinSource5

#define LTDC_B6_PORT          GPIOI
#define LTDC_B6_PIN           GPIO_Pin_6
#define LTDC_B6_PINSOURCE     GPIO_PinSource6

#define LTDC_B7_PORT          GPIOI
#define LTDC_B7_PIN           GPIO_Pin_7
#define LTDC_B7_PINSOURCE     GPIO_PinSource7


#define LTDC_CLK_PORT         GPIOG					
#define LTDC_CLK_PIN          GPIO_Pin_7
#define LTDC_CLK_PINSOURCE    GPIO_PinSource7

#define LTDC_HSYNC_PORT       GPIOI					
#define LTDC_HSYNC_PIN        GPIO_Pin_10
#define LTDC_HSYNC_PINSOURCE  GPIO_PinSource10

#define LTDC_VSYNC_PORT       GPIOI					
#define LTDC_VSYNC_PIN        GPIO_Pin_9
#define LTDC_VSYNC_PINSOURCE  GPIO_PinSource9

#define LTDC_DE_PORT          GPIOF					
#define LTDC_DE_PIN           GPIO_Pin_10
#define LTDC_DE_PINSOURCE     GPIO_PinSource10




// 1. 设置LTDC时钟，这里设置为10MHz，即刷新率在60帧左右，过高或者过低都会造成闪烁 
// 2. 这里为了方便计算数值应在3-18之间，单位为MHz，具体设置时钟的代码在 LCD_Init()
// 3. 这里的时钟并不是越快越好！！！
// 4. 过高的时钟会加大对SDRAM的占用，容易造成花屏，并且屏幕本身不支持这么高的刷新率，还会导致屏幕闪烁显示异常等
// 5. 过低的时钟会导致刷新率太低，屏幕会有闪烁
// 6. 10M的时钟，刷新率在60Hz左右
#define LCD_CLK (10)



/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void STD_LTDC_TimingInit(uint32_t hsw, uint32_t hbp, uint32_t hfp, uint32_t vsw, uint32_t vbp, uint32_t vfp);
void STD_LTDC_ResolutionInit(uint16_t width, uint16_t height);
void STD_LTDC_PixelInfoExport(uint8_t * p_format, uint8_t * p_size);
void STD_LTDC_SetARGB8888(void);
void STD_LTDC_SetRGB888(void);
void STD_LTDC_SetRGB565(void);
void STD_LTDC_SetARGB1555(void);
void STD_LTDC_SetARGB4444(void);
void STD_LTDC_SetL8(void);
void STD_LTDC_SetAL44(void);
void STD_LTDC_SetAL88(void);
void STD_LTDC_SetLayer0(uint32_t address);
void STD_LTDC_SetLayer1(uint32_t address);
void STD_LTDC_Init(void);
void STD_LTDC_LayerInit(LTDC_Layer_TypeDef * layerx, uint8_t pixel_format, uint32_t start_address);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_LTDC_H*/


/******************************* (END OF FILE) *********************************/

