/**
  ******************************************************************************
  *
  * @file    bsp_driver_w25q64.c
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
#include "bsp_driver_w25q64.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool w25q64_init(struct w25q64_driver * self);
static void w25q64_read_id(struct w25q64_driver * self, uint32_t * id);
static void w25q64_write_enable(struct w25q64_driver * self);
static void w25q64_wait_for_write_end(struct w25q64_driver * self);
static void w25q64_erase_sector(struct w25q64_driver * self, uint32_t sector_address);
static void w25q64_erase_block_32k(struct w25q64_driver * self, uint32_t block_address);
static void w25q64_erase_block_64k(struct w25q64_driver * self, uint32_t block_address);
static void w25q64_erase_chip(struct w25q64_driver * self);
static void w25q64_write_page(struct w25q64_driver * self, uint32_t page_address, const uint8_t * data, uint32_t length);
static void w25q64_write(struct w25q64_driver * self, uint32_t address, const uint8_t * data, uint32_t length);
static void w25q64_read(struct w25q64_driver * self, uint32_t address, uint8_t * data, uint32_t length);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_w25q64_link(struct w25q64_driver * self, struct w25q64_oper * oper)
{
    if(self == NULL || oper == NULL) {
        return;
    }

    self->oper = oper;

    self->pf_init               = w25q64_init;
    self->pf_read_id            = w25q64_read_id;
    self->pf_write_enable       = w25q64_write_enable;
    self->pf_wait_for_write_end = w25q64_wait_for_write_end;
    self->pf_erase_sector       = w25q64_erase_sector;
    self->pf_erase_block_32k    = w25q64_erase_block_32k;
    self->pf_erase_block_64k    = w25q64_erase_block_64k;
    self->pf_erase_chip         = w25q64_erase_chip;
    self->pf_write_page         = w25q64_write_page;
    self->pf_write              = w25q64_write;
    self->pf_read               = w25q64_read;

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool w25q64_init(struct w25q64_driver * self)
{
    if( self->oper->pf_spi_read_write   == NULL ||
        self->oper->pf_spi_cs_high      == NULL ||
        self->oper->pf_spi_cs_low       == NULL) {
        return false;
    }

    return true;
}

static void w25q64_read_id(struct w25q64_driver * self, uint32_t * id)
{
    uint8_t tx_data[4] = {0x9F, 0, 0, 0};
    uint8_t rx_data[4] = {0};

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(tx_data, rx_data, 4);
    self->oper->pf_spi_cs_high();

    *id = rx_data[1] << 16 | rx_data[2] << 8 | rx_data[3];
}

static void w25q64_write_enable(struct w25q64_driver * self)
{
    uint8_t tx_data = 0x06;
    
    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(&tx_data, NULL, 1);
    self->oper->pf_spi_cs_high();

}

static void w25q64_wait_for_write_end(struct w25q64_driver * self)
{
    uint8_t tx_data[] = {0x05, 0};
    uint8_t rx_data = 0;

    self->oper->pf_spi_cs_low();

    self->oper->pf_spi_read_write(&tx_data[0], &rx_data, 1);

    do {
        self->oper->pf_spi_read_write(&tx_data[1], &rx_data, 1);
    } while(rx_data & 0x01);

    self->oper->pf_spi_cs_high();
}

static void w25q64_erase_sector(struct w25q64_driver * self, uint32_t sector_address)
{
    uint8_t tx_data[4] = {0x20, (sector_address >> 16) & 0xFF, 
                                (sector_address >> 8) & 0xFF, 
                                sector_address & 0xFF};

    w25q64_write_enable(self);

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(tx_data, NULL, 4);
    self->oper->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);

}

static void w25q64_erase_block_32k(struct w25q64_driver * self, uint32_t block_address)
{
    uint8_t tx_data[4] = {0x52, (block_address >> 16) & 0xFF, 
                                (block_address >> 8) & 0xFF, 
                                block_address & 0xFF};

    w25q64_write_enable(self);

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(tx_data, NULL, 4);
    self->oper->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);
}

static void w25q64_erase_block_64k(struct w25q64_driver * self, uint32_t block_address)
{
    uint8_t tx_data[4] = {0xD8, (block_address >> 16) & 0xFF, 
                                (block_address >> 8) & 0xFF, 
                                block_address & 0xFF};

    w25q64_write_enable(self);

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(tx_data, NULL, 4);
    self->oper->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);
}

static void w25q64_erase_chip(struct w25q64_driver * self)
{
    uint8_t tx_data = 0xC7;
    
    w25q64_write_enable(self);

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(&tx_data, NULL, 1);
    self->oper->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);
}

static void w25q64_write_page(struct w25q64_driver * self, uint32_t page_address, const uint8_t * data, uint32_t length)
{
    uint8_t tx_data[4] = {0x02, (page_address >> 16) & 0xFF, (page_address >> 8) & 0xFF, page_address & 0xFF};

    w25q64_write_enable(self);

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(tx_data, NULL, 4);
    self->oper->pf_spi_read_write((uint8_t *)data, NULL, length);
    self->oper->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);
}

static void w25q64_write(struct w25q64_driver * self, uint32_t address, const uint8_t * data, uint32_t length)
{
    uint32_t page_size = 256;
    uint32_t page_addr = address & 0xFFFFFF00;
    uint32_t page_offset = address & 0xFF;
    uint32_t page_remain = page_size - page_offset;

    if(page_remain >= length) {
        w25q64_write_page(self, page_addr, data, length);
    } else {
        w25q64_write_page(self, page_addr, data, page_remain);
        w25q64_write(self, page_addr + page_size, data + page_remain, length - page_remain);
    }
}

static void w25q64_read(struct w25q64_driver * self, uint32_t address, uint8_t * data, uint32_t length)
{
    uint8_t tx_data[4] = {0x03, (address >> 16) & 0xFF, (address >> 8) & 0xFF, address & 0xFF};

    self->oper->pf_spi_cs_low();
    self->oper->pf_spi_read_write(tx_data, NULL, 4);
    self->oper->pf_spi_read_write(NULL, data, length);
    self->oper->pf_spi_cs_high();
}

/******************************* (END OF FILE) *********************************/

