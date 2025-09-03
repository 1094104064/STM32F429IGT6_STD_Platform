/**
  ******************************************************************************
  *
  * @file    bsp_port_w25q64.h
  * @author  Jamin
  * @brief   Header file of bsp_port_w25q64 module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_PORT_W25Q64_H
#define _BSP_PORT_W25Q64_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "spi.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void bsp_port_w25q64_read_write(uint8_t * src, uint8_t * dst, uint32_t size)
{
    STD_SPI_ReadWrite(SFLASH_SPI, src, dst, size);
}

static inline void bsp_port_w25q64_cs_low(void)
{
    GPIO_ResetBits(SFLASH_CS_PORT, SFLASH_CS_PIN);
}

static inline void bsp_port_w25q64_cs_high(void)
{
    GPIO_SetBits(SFLASH_CS_PORT, SFLASH_CS_PIN);
}



/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_W25Q64_H*/


/******************************* (END OF FILE) *********************************/

