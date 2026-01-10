/**
  ******************************************************************************
  *
  * @file    panel_spi.h
  * @author  
  * @brief   Header file of panel spi module.
  *
  ******************************************************************************
  **/

#ifndef _PANEL_SPI_H
#define _PANEL_SPI_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stddef.h>
/*********************
 *      DEFINES
 *********************/
struct panel_spi_ops
{
    void (* start_transmit)     (const struct panel_spi_ops ** ops);
    void (* stop_transmit)      (const struct panel_spi_ops ** ops);
    void (* write_data_8bit)    (const struct panel_spi_ops ** ops, uint8_t data);
    void (* write_command)      (const struct panel_spi_ops ** ops, uint8_t data);
    void (* write_data_16bit)   (const struct panel_spi_ops ** ops, uint16_t data);
    void (* write_buffer)       (const struct panel_spi_ops ** ops, const void * data, size_t size);
};

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void panel_spi_start_transmit(const struct panel_spi_ops ** ops)
{
    (*ops)->start_transmit(ops);
}

static inline void panel_spi_stop_transmit(const struct panel_spi_ops ** ops)
{
    (*ops)->stop_transmit(ops);
}

static inline void panel_spi_write_data_8bit(const struct panel_spi_ops ** ops, uint8_t data)
{
    (*ops)->write_data_8bit(ops, data);
}

static inline void panel_spi_write_command(const struct panel_spi_ops ** ops, uint8_t data)
{
    (*ops)->write_command(ops, data);
}

static inline void panel_spi_write_data_16bit(const struct panel_spi_ops ** ops, uint16_t data)
{
    (*ops)->write_data_16bit(ops, data);
}

static inline void panel_spi_write_buffer(const struct panel_spi_ops ** ops, const void * data, size_t size)
{
    (*ops)->write_buffer(ops, data, size);
}

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_PANEL_SPI_H*/


/******************************* (END OF FILE) *********************************/



