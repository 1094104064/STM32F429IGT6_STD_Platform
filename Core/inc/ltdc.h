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


 enum {
    LCD_FORMAT_ARGB8888        = 0,
    LCD_FORMAT_RGB888          = 1,
    LCD_FORMAT_RGB565          = 2,
    LCD_FORMAT_ARGB1555        = 3,
    LCD_FORMAT_ARGB4444        = 4,
};

enum {
    LCD_ROT_0                = 0,
    LCD_ROT_90               = 90,
    LCD_ROT_180              = 180,
    LCD_ROT_270              = 270,
};

#define LCD_ROTATED LCD_ROT_0

// 1. 设置LTDC时钟，这里设置为10MHz，即刷新率在60帧左右，过高或者过低都会造成闪烁 
// 2. 这里为了方便计算数值应在3-18之间，单位为MHz，具体设置时钟的代码在 LCD_Init()
// 3. 这里的时钟并不是越快越好！！！
// 4. 过高的时钟会加大对SDRAM的占用，容易造成花屏，并且屏幕本身不支持这么高的刷新率，还会导致屏幕闪烁显示异常等
// 5. 过低的时钟会导致刷新率太低，屏幕会有闪烁
// 6. 10M的时钟，刷新率在60Hz左右
#define LCD_CLK (10)

#define LCD_DEFAULT_HBP  (43)
#define LCD_DEFAULT_VBP  (12)
#define LCD_DEFAULT_HSW  (1)
#define LCD_DEFAULT_VSW  (1)
#define LCD_DEFAULT_HFP  (8)
#define LCD_DEFAULT_VFP  (8)

#define LCD_DEFAULT_WIDTH   (480)
#define LCD_DEFAULT_HEIGHT  (272)


/* layer 1 */
#define LCD_LAYER1_ADDRESS      (0xD0000000)
#define LCD_LAYER1_COLOR_MODE   (LCD_FORMAT_RGB565)

#if (   LCD_LAYER1_COLOR_MODE == LCD_FORMAT_RGB565 ||\
        LCD_LAYER1_COLOR_MODE == LCD_FORMAT_ARGB1555 ||\
        LCD_LAYER1_COLOR_MODE == LCD_FORMAT_ARGB4444    )

    #define LCD_LAYER1_PIXEL_BYTE  (2)

#elif ( LCD_LAYER1_COLOR_MODE == LCD_FORMAT_RGB888  )

    #define LCD_LAYER1_PIXEL_BYTE  (3)

#else
    #define LCD_LAYER1_PIXEL_BYTE  (4)
#endif


/* layer 2 */
#define LCD_LAYER2_ENABLE       (0)
#define LCD_LAYER2_ADDRESS      (0xD0000000 + LCD_WIDTH * LCD_HEIGHT * LCD_LAYER1_PIXEL_BYTE)
#define LCD_LAYER2_COLOR_MODE   (LCD_FORMAT_RGB565)

#if (   LCD_LAYER2_COLOR_MODE == LCD_FORMAT_RGB565 ||\
        LCD_LAYER2_COLOR_MODE == LCD_FORMAT_ARGB1555 ||\
        LCD_LAYER2_COLOR_MODE == LCD_FORMAT_ARGB4444    )

    #define LCD_LAYER2_PIXEL_BYTE  (2)

#elif ( LCD_LAYER2_COLOR_MODE == LCD_FORMAT_RGB888  )

    #define LCD_LAYER2_PIXEL_BYTE  (3)
#else
    #define LCD_LAYER2_PIXEL_BYTE  (4)
#endif

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void STD_LTDC_Init(void);
void STD_LTDC_LayerInit(void);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_LTDC_H*/


/******************************* (END OF FILE) *********************************/

