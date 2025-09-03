/**
  ******************************************************************************
  *
  * @file    spi.h
  * @author  Jamin
  * @brief   Header file of spi module.
  *
  ******************************************************************************
  **/

#ifndef _SPI_H
#define _SPI_H


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
#define ST7735_MOSI_PORT    GPIOC
#define ST7735_MOSI_PIN     GPIO_Pin_12

#define ST7735_MISO_PORT    GPIOC
#define ST7735_MISO_PIN     GPIO_Pin_11

#define ST7735_SCL_PORT     GPIOC
#define ST7735_SCL_PIN      GPIO_Pin_10

#define ST7735_CS_PORT      GPIOD
#define ST7735_CS_PIN       GPIO_Pin_3




#define SFLASH_SPI          SPI1

#define SFLASH_MOSI_PORT    GPIOB
#define SFLASH_MOSI_PIN     GPIO_Pin_5

#define SFLASH_MISO_PORT    GPIOB
#define SFLASH_MISO_PIN     GPIO_Pin_4

#define SFLASH_SCK_PORT     GPIOB
#define SFLASH_SCK_PIN      GPIO_Pin_3

#define SFLASH_CS_PORT      GPIOG
#define SFLASH_CS_PIN       GPIO_Pin_3

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void STD_SPI1_Init(void);
void STD_SPI3_Init(void);
void STD_SPI_ReadWrite(SPI_TypeDef* SPIx, uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t size);
void STD_SPI_Transmit8Bit(SPI_TypeDef* SPIx, uint8_t byte);
void STD_SPI_Transmit16Bit(SPI_TypeDef* SPIx, uint16_t half_word, uint32_t len);
void STD_SPI_DMA_Transmit16Bit(SPI_TypeDef* SPIx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_SPI_H*/


/******************************* (END OF FILE) *********************************/

