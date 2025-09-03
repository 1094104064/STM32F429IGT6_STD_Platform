/**
  ******************************************************************************
  *
  * @file    bsp_port_st7735.h
  * @author  Jamin
  * @brief   Header file of bsp_port_st7735 module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_PORT_ST7735_H
#define _BSP_PORT_ST7735_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "gpio.h"
#include "spi.h"
#include "systick.h"
/*********************
 *      DEFINES
 *********************/
#define ST7735_SPI  SPI3
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

static inline void bsp_port_st7735_transmit_8bit(uint8_t data)
{
    STD_SPI_Transmit8Bit(ST7735_SPI, data);
}

static inline void bsp_port_st7735_transmit_16bit(uint16_t data, uint32_t size)
{
    STD_SPI_Transmit16Bit(ST7735_SPI, data, size);
}


static inline void bsp_port_st7735_transmit_dma_8bit(uint8_t * buf, uint32_t size)
{

}

static inline void bsp_port_st7735_transmit_dma_16bit(uint16_t * buf, uint32_t size)
{
    STD_SPI_DMA_Transmit16Bit(ST7735_SPI, 0, 0, size, 1, buf);
}

static inline void bsp_port_st7735_delay_ms(uint32_t ms)
{
    STD_SYSTICK_Delay(ms);
}

static inline void bsp_port_st7735_cs_high(void)
{
    GPIO_SetBits(ST7735_CS_PORT, ST7735_CS_PIN);
}

static inline void bsp_port_st7735_cs_low(void)
{
    GPIO_ResetBits(ST7735_CS_PORT, ST7735_CS_PIN);
}

static inline void bsp_port_st7735_dc_high(void)
{
    GPIO_SetBits(ST7735_DC_PORT, ST7735_DC_PIN);
}

static inline void bsp_port_st7735_dc_low(void)
{
    GPIO_ResetBits(ST7735_DC_PORT, ST7735_DC_PIN);
}

static inline void bsp_port_st7735_rst_high(void)
{
    GPIO_SetBits(ST7735_RST_PORT, ST7735_RST_PIN);
}

static inline void bsp_port_st7735_rst_low(void)
{
    GPIO_ResetBits(ST7735_RST_PORT, ST7735_RST_PIN);
}

static inline void bsp_port_st7735_backlight_on(void)
{
    GPIO_SetBits(ST7735_BL_PORT, ST7735_BL_PIN);
}

static inline void bsp_port_st7735_backlight_off(void)
{
    GPIO_ResetBits(ST7735_BL_PORT, ST7735_BL_PIN);
}


static inline void bsp_port_st7735_backlight_set(uint8_t brightness)
{
    
}




/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_ST7735_H*/


/******************************* (END OF FILE) *********************************/

