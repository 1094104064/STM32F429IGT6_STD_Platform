/**
  ******************************************************************************
  *
  * @file    bsp_port_lcd.h
  * @author  Jamin
  * @brief   Header file of bsp_port_lcd module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_PORT_LCD_H
#define _BSP_PORT_LCD_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "ltdc.h"
#include "fmc.h"
#include "dma2d.h"
/*********************
 *      DEFINES
 *********************/
#define LCD_BL_PORT GPIOD
#define LCD_BL_PIN  GPIO_Pin_13
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

static void bsp_port_lcd_layer_config(uint8_t * pixel_size, uint16_t * rotated, uint32_t * start_address)
{
    if (pixel_size == NULL || rotated == NULL || start_address == NULL) {
        return;
    }

    struct framebuffer_layer * layer = STD_DMA2D_GetActiveLayer();
    if (layer == NULL) {
        return;
    }

    *pixel_size = layer->pixel_size;
    *rotated = layer->rotated;
    *start_address = layer->start_address;
}

static inline void bsp_port_lcd_backlight_on(void)
{
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
}

static inline void bsp_port_lcd_backlight_off(void)
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
}

static inline void bsp_port_lcd_put_pixel(uint16_t x, uint16_t y, uint32_t color)
{

}

static inline void bsp_port_lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    STD_DMA2D_FillRectangle(x, y, width, height, color);
}

static inline void bsp_port_lcd_fill_screen(uint32_t color)
{
    STD_DMA2D_FillScreen(color);
}

static inline void bsp_port_lcd_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    STD_DMA2D_CopyBuffer(x, y, width, height, data);
}


/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_LCD_H*/


/******************************* (END OF FILE) *********************************/

