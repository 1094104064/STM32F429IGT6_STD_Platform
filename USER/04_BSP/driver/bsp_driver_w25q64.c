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
#if W25Q64_DEBUG_ENABLE


static pf_printf_t printf_cb = NULL;

static w25q_log_level_t log_level = W25Q64_LOG_NONE;

#define LOG_OUTPUT(level, fmt, ...) do { \
    if (printf_cb && level <= log_level) { \
        printf_cb("[W25Q64] " fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_E(fmt, ...) LOG_OUTPUT(W25Q64_LOG_ERROR, "E: " fmt"\r\n", ##__VA_ARGS__)    /* 用于输出详细的调试信息 */
#define LOG_I(fmt, ...) LOG_OUTPUT(W25Q64_LOG_INFO,  "I: " fmt"\r\n", ##__VA_ARGS__)    /* 用于记录重要的操作节点或状态变化 */
#define LOG_D(fmt, ...) LOG_OUTPUT(W25Q64_LOG_DEBUG, "D: " fmt"\r\n", ##__VA_ARGS__)    /* 用于表示发生了错误 */


#define ASSERT_NULL(param)                                                          \
        do {                                                                        \
            if(param == NULL) { LOG_E("NULL pointer: \r\n", #param); while(1); }    \
        } while (0)

#else

#define LOG_E(fmt, ...)
#define LOG_I(fmt, ...)
#define LOG_D(fmt, ...)

#define ASSERT_NULL(param)                                                          \
        do {                                                                        \
            if(param == NULL) { while(1); }                                         \
        } while (0)

#endif
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
void bsp_driver_w25q64_link(w25q64_driver_t * drv, w25q64_handle_t * handle)
{
    ASSERT_NULL(drv);
    ASSERT_NULL(handle);


    drv->handle = handle;

    drv->pf_init               = w25q64_init;
    drv->pf_read_id            = w25q64_read_id;
    drv->pf_write_enable       = w25q64_write_enable;
    drv->pf_wait_for_write_end = w25q64_wait_for_write_end;
    drv->pf_erase_sector       = w25q64_erase_sector;
    drv->pf_erase_block_32k    = w25q64_erase_block_32k;
    drv->pf_erase_block_64k    = w25q64_erase_block_64k;
    drv->pf_erase_chip         = w25q64_erase_chip;
    drv->pf_write_page         = w25q64_write_page;
    drv->pf_write              = w25q64_write;
    drv->pf_read               = w25q64_read;

}


#if W25Q64_DEBUG_ENABLE
void bsp_driver_w25q64_log_init(pf_printf_t cb, w25q_log_level_t level)
{
    printf_cb = cb;
    log_level = level;
}
#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool w25q64_init(struct w25q64_driver * self)
{
    ASSERT_NULL(self->handle->pf_spi_read_write);
    ASSERT_NULL(self->handle->pf_spi_cs_high);
    ASSERT_NULL(self->handle->pf_spi_cs_low);


    uint32_t jedec_id = (W25Q64_JEDEC_MF_ID << 16) | (W25Q64_JEDEC_DIE_ID << 8) | W25Q64_JEDEC_PN_ID;
    uint32_t id = 0;

    self->pf_read_id(self, &id);

    if(id != jedec_id) {
        LOG_E("w25q64 init failed");
        return false;
    }

    LOG_I("w25q64 init successfully");

    return true;
}

static void w25q64_read_id(struct w25q64_driver * self, uint32_t * id)
{
    uint8_t tx_data[4] = {W25Q64_READ_DEVICE_ID_CMD, 0, 0, 0};
    uint8_t rx_data[4] = {0};

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, rx_data, 4)) {
        LOG_E("w25q64 read id failed");
    }

    self->handle->pf_spi_cs_high();

    *id = rx_data[1] << 16 | rx_data[2] << 8 | rx_data[3];

    LOG_I("w25q64 id: 0x%06X", *id);
}

static void w25q64_write_enable(struct w25q64_driver * self)
{
    uint8_t tx_data = W25Q64_WRITE_ENABLE_CMD;
    
    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(&tx_data, NULL, 1)) {
        LOG_E("w25q64 write enable failed");
    }

    self->handle->pf_spi_cs_high();
}

static void w25q64_write_disable(struct w25q64_driver * self)
{
    uint8_t tx_data = W25Q64_WRITE_DISABLE_CMD;
    
    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(&tx_data, NULL, 1)) {
        LOG_E("w25q64 write disable failed");
    }

    self->handle->pf_spi_cs_high();
}

static void w25q64_wait_for_write_end(struct w25q64_driver * self)
{
    uint8_t tx_data[] = {W25Q64_READ_STATUS_REG_CMD, 0};
    uint8_t rx_data = 0;

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(&tx_data[0], &rx_data, 1)) {
        LOG_E("w25q64 wait for write end failed");
    }

    do {

        if(true != self->handle->pf_spi_read_write(&tx_data[1], &rx_data, 1)) {
            LOG_E("w25q64 wait for write end failed");
        }

    } while(rx_data & 0x01);

    self->handle->pf_spi_cs_high();
}

static void w25q64_erase_sector(struct w25q64_driver * self, uint32_t sector_address)
{
    uint8_t tx_data[4] = {0x20, (sector_address >> 16) & 0xFF, 
                                (sector_address >> 8) & 0xFF, 
                                sector_address & 0xFF};

    w25q64_write_enable(self);

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        LOG_E("w25q64 erase sector failed");
    }

    self->handle->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);

    w25q64_write_disable(self);

    LOG_D("w25q64 erase sector 0x%06X", sector_address);
}

static void w25q64_erase_block_32k(struct w25q64_driver * self, uint32_t block_address)
{
    uint8_t tx_data[4] = {0x52, (block_address >> 16) & 0xFF, 
                                (block_address >> 8) & 0xFF, 
                                block_address & 0xFF};

    w25q64_write_enable(self);

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        LOG_E("w25q64 erase block 32k failed");
    }

    self->handle->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);

    w25q64_write_disable(self);

    LOG_D("w25q64 erase block 32k 0x%06X", block_address);
}

static void w25q64_erase_block_64k(struct w25q64_driver * self, uint32_t block_address)
{
    uint8_t tx_data[4] = {0xD8, (block_address >> 16) & 0xFF, 
                                (block_address >> 8) & 0xFF, 
                                block_address & 0xFF};

    w25q64_write_enable(self);

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        LOG_E("w25q64 erase block 64k failed");
    }

    self->handle->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);

    w25q64_write_disable(self);

    LOG_D("w25q64 erase block 64k 0x%06X", block_address);
}

static void w25q64_erase_chip(struct w25q64_driver * self)
{
    uint8_t tx_data = 0xC7;
    
    w25q64_write_enable(self);

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(&tx_data, NULL, 1)) {
        LOG_E("w25q64 erase chip failed");
    }

    self->handle->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);

    w25q64_write_disable(self);

    LOG_D("w25q64 erase chip");
}

static void w25q64_write_page(struct w25q64_driver * self, uint32_t page_address, const uint8_t * data, uint32_t length)
{
    uint8_t tx_data[4] = {0x02, (page_address >> 16) & 0xFF, (page_address >> 8) & 0xFF, page_address & 0xFF};

    w25q64_write_enable(self);

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        LOG_E("w25q64 write page failed");
    }

    if(true != self->handle->pf_spi_read_write((uint8_t *)data, NULL, length)) {
        LOG_E("w25q64 write page failed");
    }

    self->handle->pf_spi_cs_high();

    w25q64_wait_for_write_end(self);

    w25q64_write_disable(self);

    LOG_D("w25q64 write page 0x%06X, length: %d", page_address, length);
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

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        LOG_E("w25q64 read failed");
    }

    if(true != self->handle->pf_spi_read_write(NULL, data, length)) {
        LOG_E("w25q64 read failed");
    }

    self->handle->pf_spi_cs_high();

    LOG_D("w25q64 read 0x%06X, length: %d", address, length);
}

/******************************* (END OF FILE) *********************************/

