/**
  ******************************************************************************
  *
  * @file    bsp_hal_lcd.c
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
#include "bsp_hal_lcd.h"
#include "ltdc.h"
#include "dma2d.h"
#include "memory_manager.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define LCD_HBP  (43)
#define LCD_VBP  (12)
#define LCD_HSW  (1)
#define LCD_VSW  (1)
#define LCD_HFP  (8)
#define LCD_VFP  (8)

#define LCD_WIDTH   (480)
#define LCD_HEIGHT  (272)

#define LCD_BL_PORT GPIOD
#define LCD_BL_PIN  GPIO_Pin_13
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

/**********************
 *   STATIC FUNCTIONS
 **********************/
void bsp_hal_lcd_simple_panel_init(void)
{
    uint32_t start_address = (uint32_t)mm_alloc(LCD_WIDTH * LCD_HEIGHT * 2);
    STD_LTDC_TimingInit(LCD_HSW, LCD_HBP, LCD_HFP, LCD_VSW, LCD_VBP, LCD_VFP);
    STD_LTDC_ResolutionInit(LCD_WIDTH, LCD_HEIGHT);
    STD_LTDC_Init();
    STD_LTDC_LayerInit(LTDC_Layer1, LTDC_Pixelformat_RGB565, start_address);
    // start_address = mm_alloc(LCD_WIDTH * LCD_HEIGHT * 4);
    // STD_LTDC_LayerInit(LTDC_Layer2, LTDC_Pixelformat_ARGB8888, start_address);

    /* LCD Backlight */
    STD_GPIO_Init(LCD_BL_PORT, LCD_BL_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
}

void bsp_hal_lcd_backlight_on(void)
{
    GPIO_SetBits(LCD_BL_PORT, LCD_BL_PIN);
}

void bsp_hal_lcd_backlight_off(void)
{
    GPIO_ResetBits(LCD_BL_PORT, LCD_BL_PIN);
}

void bsp_hal_lcd_backlight_set(uint8_t brightness)
{

}

void bsp_hal_lcd_fill_rectangle()
{

}

void bsp_hal_lcd_fill_screen(uint32_t color)
{

}

void bsp_hal_lcd_copy_buffer()
{

}


/******************************* (END OF FILE) *********************************/



 







