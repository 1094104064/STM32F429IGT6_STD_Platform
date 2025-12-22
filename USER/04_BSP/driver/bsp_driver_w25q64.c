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

static w25q64_log_level_t log_level = W25Q64_LOG_NONE;

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

struct w25q64_error {
    uint8_t code;
    const char * message;
};

static const struct w25q64_error error[] = {
    { 0,    "No Error"                     },
    { 1,    "Write Enable Error"           },
    { 2,    "Write Disable Error"          },
    { 3,    "Wait For Write End Error"     },
    { 4,    "SPI Transfer Error"           },
};



/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool w25q64_init                 (struct w25q64_driver * self);
static bool w25q64_read_id              (struct w25q64_driver * self, uint32_t * id);
static bool w25q64_write_enable         (struct w25q64_driver * self);
static bool w25q64_wait_busy   (struct w25q64_driver * self);
static int  w25q64_erase_sector         (struct w25q64_driver * self, uint32_t sector_address);
static int  w25q64_erase_block_32k      (struct w25q64_driver * self, uint32_t block_address);
static int  w25q64_erase_block_64k      (struct w25q64_driver * self, uint32_t block_address);
static int  w25q64_erase_chip           (struct w25q64_driver * self);
static int  w25q64_write_page           (struct w25q64_driver * self, uint32_t page_address, const uint8_t * data, uint32_t length);
static bool w25q64_write                (struct w25q64_driver * self, uint32_t address, const uint8_t * data, uint32_t length);
static int  w25q64_read                 (struct w25q64_driver * self, uint32_t address, uint8_t * data, uint32_t length);
/**********************
 *  STATIC VARIABLES
 **********************/


/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_w25q64_link(w25q64_driver_t * drv, const w25q64_handle_t * handle)
{
    ASSERT_NULL(drv);
    ASSERT_NULL(handle);


    drv->handle = handle;

    drv->pf_init               = w25q64_init;
    drv->pf_read_id            = w25q64_read_id;
    drv->pf_write_enable       = w25q64_write_enable;
    drv->pf_wait_for_write_end = w25q64_wait_busy;
    drv->pf_erase_sector       = w25q64_erase_sector;
    drv->pf_erase_block_32k    = w25q64_erase_block_32k;
    drv->pf_erase_block_64k    = w25q64_erase_block_64k;
    drv->pf_erase_chip         = w25q64_erase_chip;
    drv->pf_write_page         = w25q64_write_page;
    drv->pf_write              = w25q64_write;
    drv->pf_read               = w25q64_read;

}


#if W25Q64_DEBUG_ENABLE
void bsp_driver_w25q64_log_init(pf_printf_t cb, w25q64_log_level_t level)
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
    ASSERT_NULL(self->handle->pf_spi_init);
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

static bool w25q64_read_id(struct w25q64_driver * self, uint32_t * id)
{
    bool is_successful = false;
    uint8_t tx_data[4] = {W25Q64_READ_DEVICE_ID_CMD, 0, 0, 0};
    uint8_t rx_data[4] = {0};

    self->handle->pf_spi_cs_low();

    is_successful = self->handle->pf_spi_read_write(tx_data, rx_data, 4);

    self->handle->pf_spi_cs_high();

    *id = rx_data[1] << 16 | rx_data[2] << 8 | rx_data[3];

    if(true != is_successful) {
        LOG_E("w25q64 read id failed");
    }
    else {
        LOG_I("w25q64 id: 0x%06X", *id);
    }

    return is_successful;
}

static bool w25q64_write_enable(struct w25q64_driver * self)
{
    bool is_successful = false;
    uint8_t tx_data = W25Q64_WRITE_ENABLE_CMD;
    
    self->handle->pf_spi_cs_low();

    is_successful = self->handle->pf_spi_read_write(&tx_data, NULL, 1);

    self->handle->pf_spi_cs_high();

    if(true != is_successful) {
        LOG_E("w25q64 write enable failed");
    }

   return is_successful;
}

static bool w25q64_write_disable(struct w25q64_driver * self)
{
    bool is_successful = false;
    uint8_t tx_data = W25Q64_WRITE_DISABLE_CMD;
    
    self->handle->pf_spi_cs_low();

    is_successful = self->handle->pf_spi_read_write(&tx_data, NULL, 1);

    self->handle->pf_spi_cs_high();

    if(true != is_successful) {
        LOG_E("w25q64 write disable failed");
    }

    return is_successful;
}

static bool w25q64_wait_busy(struct w25q64_driver * self)
{
    bool is_successful = false;
    uint8_t tx_data[] = {W25Q64_READ_STATUS_REG_CMD, 0};
    uint8_t rx_data = 0;

    self->handle->pf_spi_cs_low();

    is_successful = self->handle->pf_spi_read_write(&tx_data[0], &rx_data, 1);

    if(true != is_successful) {
        LOG_E("w25q64 wait for write end failed");
        self->handle->pf_spi_cs_high();
        return is_successful;
    }

    do {

        is_successful = self->handle->pf_spi_read_write(&tx_data[1], &rx_data, 1);

        if(true != is_successful) {
            LOG_E("w25q64 wait for write end failed");
            self->handle->pf_spi_cs_high();
            return is_successful;
        }

    } while(rx_data & 0x01);

    self->handle->pf_spi_cs_high();

    return is_successful;
}

static int w25q64_erase_sector(struct w25q64_driver * self, uint32_t sector_address)
{
    uint8_t error_code = error[0].code;
    uint8_t tx_data[4] = {0x20, (sector_address >> 16) & 0xFF, 
                                (sector_address >> 8) & 0xFF, 
                                sector_address & 0xFF};

    if(true != w25q64_write_enable(self)) {
        error_code = error[1].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_low();

   if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        self->handle->pf_spi_cs_high();
        error_code = error[4].code;
        goto error_handle;
   }
   else {
        self->handle->pf_spi_cs_high();
   }


    if(true != w25q64_wait_busy(self)) {
        error_code = error[3].code;
        goto error_handle;
    }

    if(true != w25q64_write_disable(self)) {
        error_code = error[2].code;
        goto error_handle;
    }

    LOG_D("w25q64 erase sector 0x%06X", sector_address);

    return error_code;

error_handle:
    LOG_E("w25q64 erase sector failed, error code: %d, %s", error_code, error[error_code].message);
    return error_code;
}

static int w25q64_erase_block_32k(struct w25q64_driver * self, uint32_t block_address)
{
    uint8_t error_code = error[0].code;
    uint8_t tx_data[4] = {0x52, (block_address >> 16) & 0xFF, 
                                (block_address >> 8) & 0xFF, 
                                block_address & 0xFF};

    if(true != w25q64_write_enable(self)) {
        error_code = error[1].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        self->handle->pf_spi_cs_high();
        error_code = error[4].code;
        goto error_handle;
    }
    else {
        self->handle->pf_spi_cs_high();
    }

    if(true != w25q64_wait_busy(self)) {
        error_code = error[3].code;
        goto error_handle;
    }

    if(true != w25q64_write_disable(self)) {
        error_code = error[2].code;
        goto error_handle;
    }

    LOG_D("w25q64 erase block 32k 0x%06X", block_address);

    return error_code;

error_handle:
    LOG_E("w25q64 erase block 32k failed, error code: %d, %s", error_code, error[error_code].message);
    return error_code;
}

static int w25q64_erase_block_64k(struct w25q64_driver * self, uint32_t block_address)
{
    uint8_t error_code = error[0].code;
    uint8_t tx_data[4] = {0xD8, (block_address >> 16) & 0xFF, 
                                (block_address >> 8) & 0xFF, 
                                block_address & 0xFF};

    if(true != w25q64_write_enable(self)) {
        error_code = error[1].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        self->handle->pf_spi_cs_high();
        error_code = error[4].code;
        goto error_handle;
    }
    else {
        self->handle->pf_spi_cs_high();
    }


    if(true != w25q64_wait_busy(self)) {
        error_code = error[3].code;
        goto error_handle;
    }

    if(true != w25q64_write_disable(self)) {
        error_code = error[2].code;
        goto error_handle;
    }

    LOG_D("w25q64 erase block 64k 0x%06X", block_address);

    return error_code;

error_handle:
    LOG_E("w25q64 erase block 64k failed, error code: %d, %s", error_code, error[error_code].message);
    return error_code;
}

static int w25q64_erase_chip(struct w25q64_driver * self)
{
    uint8_t error_code = error[0].code;
    uint8_t tx_data = 0xC7;
    
    if(true != w25q64_write_enable(self)) {
        error_code = error[1].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(&tx_data, NULL, 1)) {
        self->handle->pf_spi_cs_high();
        error_code = error[4].code;
        goto error_handle;
    }
    else {
        self->handle->pf_spi_cs_high();
    }

    if(true != w25q64_wait_busy(self)) {
        error_code = error[3].code;
        goto error_handle;
    }

    if(true != w25q64_write_disable(self)) {
        error_code = error[2].code;
        goto error_handle;
    }

    LOG_D("w25q64 erase chip");

    return error_code;

error_handle:
    LOG_E("w25q64 erase chip failed, error code: %d, %s", error_code, error[error_code].message);
    return error_code;
}

static int w25q64_write_page(struct w25q64_driver * self, uint32_t page_address, const uint8_t * data, uint32_t length)
{
    uint8_t error_code = error[0].code;
    uint8_t tx_data[4] = {0x02, (page_address >> 16) & 0xFF, (page_address >> 8) & 0xFF, page_address & 0xFF};

    if(true != w25q64_write_enable(self)) {
        error_code = error[1].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        self->handle->pf_spi_cs_high();
        error_code = error[4].code;
        goto error_handle;
    }

    if(true != self->handle->pf_spi_read_write((uint8_t *)data, NULL, length)) {
        self->handle->pf_spi_cs_high();
        error_code = error[4].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_high();

    if(true != w25q64_wait_busy(self)) {
        error_code = error[3].code;
        goto error_handle;
    }

    if(true != w25q64_write_disable(self)) {
        error_code = error[2].code;
        goto error_handle;
    }

    LOG_D("w25q64 write page 0x%06X, length: %d", page_address, length);

    return error_code;

error_handle:
    LOG_E("w25q64 write page failed, error code: %d, %s", error_code, error[error_code].message);
    return error_code;
}

static bool w25q64_write(struct w25q64_driver * self, uint32_t address, const uint8_t * data, uint32_t length)
{
    if (self == NULL || data == NULL) {
        return false;
    }

    while (length > 0) {
        // 1. 计算当前页剩余可写的空间
        //    address % W25Q64_PAGE_SIZE 计算出在页内的偏移量
        uint32_t page_offset = address % W25Q64_PAGE_SIZE;
        uint32_t page_remain = W25Q64_PAGE_SIZE - page_offset;

        // 2. 决定本次写入的长度：取“剩余数据长度”和“当前页剩余空间”的较小值
        uint32_t to_write = (length < page_remain) ? length : page_remain;

        // 3. 执行单页（或部分页）写入
        //    注意：这里传入的是当前的 address，而不是页首地址
        if (!w25q64_write_page(self, address, data, to_write)) {
            LOG_E("w25q64 write failed at address 0x%06X", address);
            return false;
        }

        // 4. 更新游标
        address += to_write;
        data    += to_write;
        length  -= to_write;

    }

    return true;
}

static int w25q64_read(struct w25q64_driver * self, uint32_t address, uint8_t * data, uint32_t length)
{
    uint8_t error_code = error[0].code;
    uint8_t tx_data[4] = {0x03, (address >> 16) & 0xFF, (address >> 8) & 0xFF, address & 0xFF};

    self->handle->pf_spi_cs_low();

    if(true != self->handle->pf_spi_read_write(tx_data, NULL, 4)) {
        error_code = error[4].code;
        goto error_handle;
    }

    if(true != self->handle->pf_spi_read_write(NULL, data, length)) {
        error_code = error[4].code;
        goto error_handle;
    }

    self->handle->pf_spi_cs_high();

    LOG_D("w25q64 read 0x%06X, length: %d", address, length);

    return error_code;

error_handle:
    self->handle->pf_spi_cs_high();
    LOG_D("w25q64 read failed, error code: %d, %s", error_code, error[error_code].message);
    return error_code;
}

/******************************* (END OF FILE) *********************************/

