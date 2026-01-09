#include "stm32f4xx.h"
#include "../../adapter/panel_spi.h"

static void spi_start_transmit(struct panel_spi_ops ** ops)
{
    // Implement SPI transmission start for STM32
}

static void spi_stop_transmit(struct panel_spi_ops ** ops)
{
    // Implement SPI transmission stop for STM32
}

static void spi_write_data_8bit(struct panel_spi_ops ** ops, uint8_t data)
{
    // Implement SPI 8-bit data write for STM32
}

static void spi_write_command(struct panel_spi_ops ** ops, uint8_t data)
{
    // Implement SPI command write for STM32
}

static void spi_write_data_16bit(struct panel_spi_ops ** ops, uint16_t data)
{
    // Implement SPI 16-bit data write for STM32
}

static void spi_write_buffer(struct panel_spi_ops ** ops, const void * data, size_t size)
{
    // Implement SPI buffer write for STM32
}


static const struct panel_spi_ops spi_ops = {
    .start_transmit   = spi_start_transmit,
    .stop_transmit    = spi_stop_transmit,
    .write_data_8bit  = spi_write_data_8bit,
    .write_command    = spi_write_command,
    .write_data_16bit = spi_write_data_16bit,
    .write_buffer     = spi_write_buffer,
};


const struct panel_spi_ops * ud_st7735_get_spi(void)
{
    return &spi_ops;
}

const struct panel_spi_ops * ud_st7789_get_spi(void)
{
    return &spi_ops;
}