/**
  ******************************************************************************
  *
  * @file    spi.c
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
#include "spi.h"
#include "gpio.h"
#include "systick.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
struct spi_dma_setting {
    DMA_Stream_TypeDef *    tx_stream;
    uint32_t                tx_channel;
    DMA_Stream_TypeDef *    rx_stream;
    uint32_t                rx_channel;
};
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static const struct spi_dma_setting * get_spi_dma_setting(SPI_TypeDef* SPIx);
/**********************
 *  STATIC VARIABLES
 **********************/

/* SPI1 TX and RX default settings */
static const struct spi_dma_setting spi1_dma = {
    .tx_stream  = DMA2_Stream3,
    .tx_channel = DMA_Channel_3,

    .rx_stream  = DMA2_Stream2,
    .rx_channel = DMA_Channel_3,
};

/* SPI2 TX and RX default settings */
static const struct spi_dma_setting spi2_dma = {
    .tx_stream  = DMA1_Stream4,
    .tx_channel = DMA_Channel_0,

    .rx_stream  = DMA1_Stream3,
    .rx_channel = DMA_Channel_0,
};

/* SPI3 TX and RX default settings */
static const struct spi_dma_setting spi3_dma = {
    .tx_stream  = DMA1_Stream5,
    .tx_channel = DMA_Channel_0,

    .rx_stream  = DMA1_Stream0,
    .rx_channel = DMA_Channel_0,
};

/* SPI4 TX and RX default settings */
static const struct spi_dma_setting spi4_dma = {
    .tx_stream  = DMA2_Stream1,
    .tx_channel = DMA_Channel_4,

    .rx_stream  = DMA2_Stream0,
    .rx_channel = DMA_Channel_4,
};

/* SPI5 TX and RX default settings */
static const struct spi_dma_setting spi5_dma = {
    .tx_stream  = DMA2_Stream6,
    .tx_channel = DMA_Channel_7,

    .rx_stream  = DMA2_Stream5,
    .rx_channel = DMA_Channel_7,
};

/* SPI6 TX and RX default settings */
static const struct spi_dma_setting spi6_dma = {
    .tx_stream  = DMA2_Stream5,
    .tx_channel = DMA_Channel_1,

    .rx_stream  = DMA2_Stream0,
    .rx_channel = DMA_Channel_1,
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void STD_SPI1_Init(void)
{
    STD_GPIO_Init(SFLASH_MOSI_PORT, SFLASH_MOSI_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI1);
    STD_GPIO_Init(SFLASH_MISO_PORT, SFLASH_MISO_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI1);
    STD_GPIO_Init(SFLASH_SCK_PORT,  SFLASH_SCK_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI1);
    STD_GPIO_Init(SFLASH_CS_PORT,   SFLASH_CS_PIN,   GPIO_Mode_OUT,GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0);

    /* Important */
    GPIO_SetBits(SFLASH_CS_PORT, SFLASH_CS_PIN);

    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;      /**< Full duplex communication*/
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                      /**< Master mode*/
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                      /**< 8-bit data length*/
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                        /**< information*/
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                       /**< Clock Phase configuration*/
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                         /**< Clock Polarity configuration*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;              /**< 2-split frequency*/
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                     /**< Data transfer high first*/
    SPI_InitStructure.SPI_CRCPolynomial     = 7;                                    /**< This bit is invalid when CRC is disabled*/
    SPI_Init(SPI1, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(SPI1, ENABLE);	
}

void STD_SPI3_Init(void)
{
    STD_GPIO_Init(ST7735_MOSI_PORT, ST7735_MOSI_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI3);
    STD_GPIO_Init(ST7735_MISO_PORT, ST7735_MISO_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI3);
    STD_GPIO_Init(ST7735_SCL_PORT,  ST7735_SCL_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_AF_SPI3);
    STD_GPIO_Init(ST7735_CS_PORT,   ST7735_CS_PIN,   GPIO_Mode_OUT,GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP    , 0);


    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;      /**< Full duplex communication*/
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                      /**< Master mode*/
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                      /**< 8-bit data length*/
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                        /**< information*/
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                       /**< Clock Phase configuration*/
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                         /**< Clock Polarity configuration*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;              /**< 4-split frequency*/
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                     /**< Data transfer high first*/
    SPI_InitStructure.SPI_CRCPolynomial     = 7;                                    /**< This bit is invalid when CRC is disabled*/
    SPI_Init(SPI3, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(SPI3, ENABLE);

}

void STD_SPI_ReadWrite(SPI_TypeDef* SPIx, uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {

        // Wait until the transmit buffer is empty
        while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
        // Send the data
        SPI_I2S_SendData(SPIx, tx_buffer[i]);
        // Wait until a data is received
        while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
        // Read the received data
        rx_buffer[i] = SPI_I2S_ReceiveData(SPIx);
    }
}

void STD_SPI_Transmit8Bit(SPI_TypeDef* SPIx, uint8_t byte)
{
    /*!< Loop while DR register in not empty */
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) != RESET);

    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPIx, byte);

    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    /*!< Loop while DR register in not empty */
    while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) != RESET);
}

void STD_SPI_Transmit16Bit(SPI_TypeDef* SPIx, uint16_t half_word, uint32_t len)
{
    SPIx->CR1 &= 0xFFBF;            /* disable spi */
    SPIx->CR1 |= SPI_DataSize_16b;  /* set data size to 16 bits */
    SPIx->CR1 |= 0x0040;            /* enable spi */

    for(uint32_t i = 0; i < len; i++) {
        SPIx->DR = half_word;
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0);	        
    }
    while((SPIx->SR & SPI_I2S_FLAG_BSY) != RESET);

    SPIx->CR1 &= 0xFFBF;	        /* disable spi */
    SPIx->CR1 &= 0xF7FF;	        /* set data size to 8 bits */
    SPIx->CR1 |= 0x0040;	        /* enable spi */
}

void STD_SPI_DMA_Transmit16Bit(SPI_TypeDef* SPIx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data)
{
    uint32_t start = 0;
    const struct spi_dma_setting * spi_dma = get_spi_dma_setting(SPIx);

    SPIx->CR1 &= 0xFFBF;            /* disable spi */
    SPIx->CR1 |= SPI_DataSize_16b;  /* set data size to 16 bits */
    SPIx->CR1 |= 0x0040;            /* enable spi */

    start = STD_SYSTICK_Get();
    DMA_DeInit(spi_dma->tx_stream);
    while(DMA_GetCmdStatus(spi_dma->tx_stream) != RESET) {
        if(STD_SYSTICK_Get() - start > 1000) {
            break;
        }
    }

    DMA_InitTypeDef DMA_InitStructure = {0};

    DMA_InitStructure.DMA_Channel               = spi_dma->tx_channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(SPIx->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t)data;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize            = width * height;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_Init(spi_dma->tx_stream, &DMA_InitStructure);

#if 0
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

    DMA_Cmd(spi_dma->tx_stream, DISABLE);
    DMA_SetCurrDataCounter(spi_dma->tx_stream, width * height);

    DMA_Cmd(spi_dma->tx_stream, ENABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);

    start = STD_SYSTICK_Get();
    /* Wait until transfer complete */
    while(DMA_GetFlagStatus(spi_dma->tx_stream, DMA_FLAG_TCIF5) == RESET) {
        if(STD_SYSTICK_Get() - start > 1000) {
            break;
        }
    }

    /* Clear transfer complete flag */
    DMA_ClearFlag(spi_dma->tx_stream, DMA_FLAG_TCIF5);

    SPIx->CR1 &= 0xFFBF;	        /* disable spi */
    SPIx->CR1 &= 0xF7FF;	        /* set data size to 8 bits */
    SPIx->CR1 |= 0x0040;	        /* enable spi */
}

static const struct spi_dma_setting * get_spi_dma_setting(SPI_TypeDef* SPIx)
{
    const struct spi_dma_setting * result;

    if(SPIx == SPI1) {
        result = &spi1_dma;
    }
    else if(SPIx == SPI2) {
        result = &spi2_dma;
    }
    else if(SPIx == SPI3) {
        result = &spi3_dma;
    }
    else if(SPIx == SPI4) {
        result = &spi4_dma;
    }
    else if(SPIx == SPI5) {
        result = &spi5_dma;
    }
    else if(SPIx == SPI6) {
        result = &spi6_dma;
    }
    else {
        result = &spi1_dma;
    }

    return result;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
