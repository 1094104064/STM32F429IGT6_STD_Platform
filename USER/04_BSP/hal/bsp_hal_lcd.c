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
#include "debug.h"
#include "gpio.h"
/**********************
 *      MACROS
 **********************/
struct lcd_layer
{
    LTDC_Layer_TypeDef * layer;
    uint8_t pixel_format;
    uint8_t pixel_size;
    uint32_t start_address;
};
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
static struct lcd_layer layers[2];
static struct lcd_layer * act_layer = &layers[1];
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
    layers[1].layer = LTDC_Layer1;
    layers[1].pixel_format = LTDC_Pixelformat_RGB565;
    layers[1].pixel_size = 2;
    layers[1].start_address = start_address;

    // start_address = mm_alloc(LCD_WIDTH * LCD_HEIGHT * 4);
    // STD_LTDC_LayerInit(LTDC_Layer2, LTDC_Pixelformat_ARGB8888, start_address);
    // layers[2].layer = LTDC_Layer2;
    // layers[2].pixel_format = LTDC_Pixelformat_ARGB8888;
    // layers[2].pixel_size = 4;
    // layers[2].start_address = start_address;

    /* LCD Backlight */
    STD_GPIO_Init(LCD_BL_PORT, LCD_BL_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

    act_layer = &layers[1];

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

void bsp_hal_lcd_switch_layer(uint8_t layerx)
{
    if(layerx > 2) return;

    switch (layerx) {
    case 1:
        act_layer = &layers[1];
        break;
    
    case 2:
        act_layer = &layers[2];
        break;

    default:
        act_layer = &layers[1];
        break;
    }
}

void bsp_hal_lcd_put_pixel(uint16_t x, uint16_t y, uint32_t color, uint16_t rotated)
{
    uint32_t pixel_pos      = 0;
    uint32_t width          = LCD_WIDTH;
    uint32_t height         = LCD_HEIGHT;
    uint8_t pixel_size      = act_layer->pixel_size;
    uint32_t start_address  = act_layer->start_address;

    if(rotated == 0) {
        pixel_pos = y * width + x;
    }
    else if(rotated == 90) {
        pixel_pos = x * width + (width - 1 - y);
    }
    else if(rotated == 180) {
        pixel_pos = (height - 1 - y) * width + 
                    (width - 1 - x);
    }
    else if(rotated == 270) {
        pixel_pos = (height - 1 - x) * width + y;
    }
    else {
        pixel_pos = y * width + x;
    }

    if(pixel_size == 4) {
        *(volatile uint32_t*)( start_address + 4 * pixel_pos ) = color ;
    }
    else if(pixel_size == 3) {
        *(volatile uint16_t*)( start_address + 3 * pixel_pos ) = color;
        *(volatile uint8_t*)( start_address + 3 * pixel_pos + 2 ) = color >> 16;
    }
    else if(pixel_size == 2) {
        *(volatile uint16_t*)(start_address + 2 * pixel_pos ) = color;
    }
    else {
        *(volatile uint16_t*)(start_address + 2 * pixel_pos ) = color;
    }
}

void bsp_hal_lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    uint8_t pixel_format = act_layer->pixel_format;
    uint8_t pixel_size = act_layer->pixel_size;
    uint32_t start_address = act_layer->start_address;

    STD_DMA2D_Ready(pixel_format, pixel_size, width, height, start_address);
    STD_DMA2D_FillRectangle(x, y, width, height, color);
}

void bsp_hal_lcd_fill_screen(uint32_t color)
{
    uint8_t pixel_format    = act_layer->pixel_format;
    uint8_t pixel_size      = act_layer->pixel_size;
    uint32_t start_address  = act_layer->start_address;

    STD_DMA2D_Ready(pixel_format, pixel_size, LCD_WIDTH, LCD_HEIGHT, start_address);
    STD_DMA2D_FillScreen(color);
}

void bsp_hal_lcd_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    uint8_t pixel_format = act_layer->pixel_format;
    uint8_t pixel_size = act_layer->pixel_size;
    uint32_t start_address = act_layer->start_address;
    
    STD_DMA2D_Ready(pixel_format, pixel_size, width, height, start_address);
    STD_DMA2D_CopyBuffer(x, y, width, height, data);
}

uint16_t bsp_hal_lcd_get_width(void)
{
    return LCD_WIDTH;
}

uint16_t bsp_hal_lcd_get_height(void)
{
    return LCD_HEIGHT;
}

uint32_t bsp_hal_lcd_get_layer_address(void)
{
    return act_layer->start_address;
}

/******************************* (END OF FILE) *********************************/



 







