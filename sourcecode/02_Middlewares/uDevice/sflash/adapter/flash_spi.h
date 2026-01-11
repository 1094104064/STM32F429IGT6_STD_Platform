/**
  ******************************************************************************
  *
  * @file    flash_spi.h
  * @author  
  * @brief   Header file of flash SPI module.
  *
  ******************************************************************************
  **/

#ifndef _FLASH_SPI_H
#define _FLASH_SPI_H


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
struct flash_spi_ops
{
    void (* select_chip)(const struct flash_spi_ops ** ops);
    void (* deselect_chip)(const struct flash_spi_ops ** ops);
    void (* read_write)(const struct flash_spi_ops ** ops, uint8_t * src, uint8_t * dst, uint32_t size);
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

static inline void flash_spi_select_chip(const struct flash_spi_ops ** ops)
{
    (*ops)->select_chip(ops);
}

static inline void flash_spi_deselect_chip(const struct flash_spi_ops ** ops)
{
    (*ops)->deselect_chip(ops);
}

static inline void flash_spi_read_write(const struct flash_spi_ops ** ops, uint8_t * src, uint8_t * dst, uint32_t size)
{
    (*ops)->read_write(ops, src, dst, size);
}

/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_FLASH_SPI_H*/


/******************************* (END OF FILE) *********************************/


