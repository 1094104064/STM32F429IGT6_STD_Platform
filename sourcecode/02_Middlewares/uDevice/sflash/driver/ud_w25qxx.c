#include "./ud_w25qxx.h"




static void w25qxx_write_enable(const struct ud_sflash_ops ** ops)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data = W25QXX_WRITE_ENABLE_CMD;

    flash_spi_select_chip(self->spi_ops);
    flash_spi_read_write(self->spi_ops, (uint8_t *)&tx_data, NULL, 1);
    flash_spi_deselect_chip(self->spi_ops);
}

static void w25qxx_write_disable(const struct ud_sflash_ops ** ops)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data = W25QXX_WRITE_DISABLE_CMD;

    flash_spi_select_chip(self->spi_ops);
    flash_spi_read_write(self->spi_ops, (uint8_t *)&tx_data, NULL, 1);
    flash_spi_deselect_chip(self->spi_ops);
}

static void w25qxx_wait_busy(const struct ud_sflash_ops ** ops)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data = W25QXX_READ_STATUS_REG_CMD;
    uint8_t rx_data = 0;

    flash_spi_select_chip(self->spi_ops);

    flash_spi_read_write(self->spi_ops, (uint8_t *)&tx_data, NULL, 1);

    do {
        flash_spi_read_write(self->spi_ops, NULL, (uint8_t *)&rx_data, 1);
    } while (rx_data & 0x01); /* Check the BUSY bit */

    flash_spi_deselect_chip(self->spi_ops);
}

void ud_w25qxx_get_jedec_id(const const struct ud_sflash_ops ** ops, uint32_t * jedec_id)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data[4] = {W25QXX_READ_DEVICE_ID_CMD, 0, 0, 0};
    uint8_t rx_data[4] = {0};

    flash_spi_select_chip(self->spi_ops);

    flash_spi_read_write(self->spi_ops, tx_data, rx_data, 4);

    flash_spi_deselect_chip(self->spi_ops);

    *jedec_id = (uint32_t)(rx_data[1] << 16U) | (uint32_t)(rx_data[2] << 8U) | (uint32_t)(rx_data[3]);
}

void ud_w25qxx_read(const struct ud_sflash_ops ** ops, uint32_t address, uint8_t * dst, uint32_t length)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data[4];

    flash_spi_select_chip(self->spi_ops);

    tx_data[0] = W25QXX_READ_DATA_CMD;
    tx_data[1] = (uint8_t)((address >> 16U) & 0xFFU);
    tx_data[2] = (uint8_t)((address >> 8U) & 0xFFU);
    tx_data[3] = (uint8_t)(address & 0xFFU);

    flash_spi_read_write(self->spi_ops, tx_data, NULL, 4);
    flash_spi_read_write(self->spi_ops, NULL, dst, length);

    flash_spi_deselect_chip(self->spi_ops);
}

static void w25qxx_page_program(const struct ud_sflash_ops ** ops, uint32_t page_address, const uint8_t * src, uint32_t length)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data[4];

    w25qxx_write_enable(ops);

    flash_spi_select_chip(self->spi_ops);

    tx_data[0] = W25QXX_PAGE_PROGRAM_CMD;
    tx_data[1] = (uint8_t)((page_address >> 16U) & 0xFFU);
    tx_data[2] = (uint8_t)((page_address >> 8U) & 0xFFU);
    tx_data[3] = (uint8_t)(page_address & 0xFFU);

    flash_spi_read_write(self->spi_ops, tx_data, NULL, 4);
    flash_spi_read_write(self->spi_ops, (uint8_t *)src, NULL, length);

    flash_spi_deselect_chip(self->spi_ops);

    w25qxx_wait_busy(ops);

    w25qxx_write_disable(ops);
}

void ud_w25qxx_write(const struct ud_sflash_ops ** ops, uint32_t address, const uint8_t * src, uint32_t length)
{
    uint32_t bytes_to_write;
    uint32_t current_address = address;
    const uint8_t * current_src = src;
    uint32_t remaining_bytes = length;

    while (remaining_bytes > 0U) {
        /* Calculate how many bytes to write in the current page */
        uint32_t page_offset = current_address % W25QXX_PAGE_SIZE;
        bytes_to_write = W25QXX_PAGE_SIZE - page_offset;

        if (bytes_to_write > remaining_bytes) {
            bytes_to_write = remaining_bytes;
        }

        w25qxx_page_program(ops, current_address, current_src, bytes_to_write);

        current_address += bytes_to_write;
        current_src += bytes_to_write;
        remaining_bytes -= bytes_to_write;
    }
}

void ud_w25qxx_erase_chip(const struct ud_sflash_ops ** ops)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data = W25QXX_CHIP_ERASE_CMD;

    w25qxx_write_enable(ops);

    flash_spi_select_chip(self->spi_ops);
    flash_spi_read_write(self->spi_ops, (uint8_t *)&tx_data, NULL, 1);
    flash_spi_deselect_chip(self->spi_ops);

    w25qxx_wait_busy(ops);

    w25qxx_write_disable(ops);
}

void ud_w25qxx_erase_sector(const struct ud_sflash_ops ** ops, uint32_t sector_address)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data[4];

    w25qxx_write_enable(ops);

    flash_spi_select_chip(self->spi_ops);

    tx_data[0] = W25QXX_SECTOR_ERASE_CMD;
    tx_data[1] = (uint8_t)((sector_address >> 16U) & 0xFFU);
    tx_data[2] = (uint8_t)((sector_address >> 8U) & 0xFFU);
    tx_data[3] = (uint8_t)(sector_address & 0xFFU);

    flash_spi_read_write(self->spi_ops, tx_data, NULL, 4);

    flash_spi_deselect_chip(self->spi_ops);

    w25qxx_wait_busy(ops);

    w25qxx_write_disable(ops);
}

void ud_w25qxx_erase_block32(const struct ud_sflash_ops ** ops, uint32_t block_address)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data[4];

    w25qxx_write_enable(ops);

    flash_spi_select_chip(self->spi_ops);

    tx_data[0] = W25QXX_BLOCK_32K_ERASE_CMD;
    tx_data[1] = (uint8_t)((block_address >> 16U) & 0xFFU);
    tx_data[2] = (uint8_t)((block_address >> 8U) & 0xFFU);
    tx_data[3] = (uint8_t)(block_address & 0xFFU);

    flash_spi_read_write(self->spi_ops, tx_data, NULL, 4);

    flash_spi_deselect_chip(self->spi_ops);

    w25qxx_wait_busy(ops);

    w25qxx_write_disable(ops);
}

void ud_w25qxx_erase_block64(const struct ud_sflash_ops ** ops, uint32_t block_address)
{
    const struct ud_w25qxx * self = (const struct ud_w25qxx *)ops;
    uint8_t tx_data[4];

    w25qxx_write_enable(ops);

    flash_spi_select_chip(self->spi_ops);

    tx_data[0] = W25QXX_BLOCK_64K_ERASE_CMD;
    tx_data[1] = (uint8_t)((block_address >> 16U) & 0xFFU);
    tx_data[2] = (uint8_t)((block_address >> 8U) & 0xFFU);
    tx_data[3] = (uint8_t)(block_address & 0xFFU);

    flash_spi_read_write(self->spi_ops, tx_data, NULL, 4);

    flash_spi_deselect_chip(self->spi_ops);

    w25qxx_wait_busy(ops);

    w25qxx_write_disable(ops);
}
