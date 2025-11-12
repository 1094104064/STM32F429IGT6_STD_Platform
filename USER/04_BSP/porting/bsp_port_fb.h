/**
  ******************************************************************************
  *
  * @file    bsp_port_fb.h
  * @author  Jamin
  * @brief   Header file of bsp_port_lcd module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_PORT_FB_H
#define _BSP_PORT_FB_H


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
#define FB_BL_PORT GPIOD
#define FB_BL_PIN  GPIO_Pin_13
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

static void bsp_port_fb_timing_set(uint32_t hsync_width, uint32_t hback_porch, uint32_t hfront_porch, uint32_t vsync_width, uint32_t vback_porch, uint32_t vfront_porch)
{
    STD_LTDC_TimingInit(hsync_width, hback_porch, hfront_porch, vsync_width, vback_porch, vfront_porch);
}

static void bsp_port_fb_resolution_set(uint16_t width, uint16_t height)
{
    STD_LTDC_ResolutionInit(width, height);
}

static void bsp_port_fb_pixel_info_get(uint8_t * pixel_format, uint8_t * pixel_size)
{
    STD_LTDC_PixelInfoExport(pixel_format, pixel_size);
}

static void bsp_port_fb_rgb565_set(void)
{
    STD_LTDC_SetRGB565();
}

static void bsp_port_fb_rgb888_set(void)
{
    STD_LTDC_SetRGB888();
}

static void bsp_port_fb_argb888_set(void)
{
    STD_LTDC_SetARGB8888();
}

static void bsp_port_fb_layer0_set(uint32_t address)
{
    STD_LTDC_SetLayer0(address);
}

static void bsp_port_fb_layer1_set(uint32_t address)
{
    STD_LTDC_SetLayer1(address);
}


static void bsp_port_fb_init(void)
{
    STD_LTDC_Init();
    STD_LTDC_LayerInit();
}


static inline void bsp_port_fb_backlight_on(void)
{
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
}

static inline void bsp_port_fb_backlight_off(void)
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
}

static inline void bsp_port_fb_gpu_ready( uint8_t pixel_format, uint8_t pixel_size, uint16_t rotated, 
                                          uint16_t width, uint16_t height, uint32_t address )
{
    STD_DMA2D_Ready(pixel_format, pixel_size, rotated, width, height, address);
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

#endif /*_BSP_PORT_FB_H*/


/******************************* (END OF FILE) *********************************/

