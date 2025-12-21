/**
  ******************************************************************************
  *
  * @file    bsp_linker_w25q64.h
  * @author  
  * @brief   Header file of bsp_linker_w25q64 module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_LINKER_W25Q64_H
#define _BSP_LINKER_W25Q64_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_linker_w25q64_spi_init(void);
bool bsp_linker_w25q64_spi_read_write(uint8_t * src, uint8_t * dst, uint32_t size);
void bsp_linker_w25q64_spi_cs_high(void);
void bsp_linker_w25q64_spi_cs_low(void);
/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_BSP_LINKER_W25Q64_H*/


/******************************* (END OF FILE) *********************************/


