/**
  ******************************************************************************
  *
  * @file    bsp_linker_st7735.h
  * @author  
  * @brief   Header file of bsp_linker_st7735 module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_LINKER_ST7735_H
#define _BSP_LINKER_ST7735_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_linker_st7735_spi_init(void);
void bsp_linker_st7735_spi_transmit_8bit(uint8_t data);
void bsp_linker_st7735_spi_transmit_16bit(uint16_t data, uint32_t size);
void bsp_linker_st7735_spi_dma_transmit_8bit(uint8_t * buf, uint32_t size);
void bsp_linker_st7735_spi_dma_transmit_16bit(uint16_t * buf, uint32_t size);
void bsp_linker_st7735_cs_high(void);
void bsp_linker_st7735_cs_low(void);
void bsp_linker_st7735_dc_high(void);
void bsp_linker_st7735_dc_low(void);
void bsp_linker_st7735_rst_high(void);
void bsp_linker_st7735_rst_low(void);
void bsp_linker_st7735_backlight_on(void);
void bsp_linker_st7735_backlight_off(void);
void bsp_linker_st7735_backlight_set(uint8_t brightness);
uint16_t bsp_linker_st7735_get_width(void);
uint16_t bsp_linker_st7735_get_height(void);
void bsp_linker_st7735_delay_ms(uint32_t ms);

/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_BSP_LINKER_ST7735_H*/


/******************************* (END OF FILE) *********************************/


