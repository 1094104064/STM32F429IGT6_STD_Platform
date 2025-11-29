/**
  ******************************************************************************
  *
  * @file    bsp_driver_w25q64.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_w25q64 module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_DRIVER_W25Q64_H
#define _BSP_DRIVER_W25Q64_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/

#define W25Q64_DEBUG_ENABLE 0


/**********************
 *      TYPEDEFS
 **********************/
typedef int (* pf_printf_t)(const char* format, ...);

typedef enum 
{
    W25Q64_LOG_NONE = 0,
    W25Q64_LOG_ERROR,
    W25Q64_LOG_INFO,
    W25Q64_LOG_DEBUG,
} w25q_log_level_t;

typedef struct w25q64_handle w25q64_handle_t;
typedef struct w25q64_driver w25q64_driver_t;

struct w25q64_handle 
{
    bool (* pf_spi_read_write)(uint8_t * src, uint8_t * dst, uint32_t size);
    void (* pf_spi_cs_low)(void);
    void (* pf_spi_cs_high)(void);
};


struct w25q64_driver 
{
    const w25q64_handle_t * handle;

    bool (* pf_init)(struct w25q64_driver * self);
    void (* pf_read_id)(struct w25q64_driver * self, uint32_t * id);
    void (* pf_write_enable)(struct w25q64_driver * self);
    void (* pf_wait_for_write_end)(struct w25q64_driver * self);
    void (* pf_erase_sector)(struct w25q64_driver * self, uint32_t sector_address);
    void (* pf_erase_block_32k)(struct w25q64_driver * self, uint32_t block_address);
    void (* pf_erase_block_64k)(struct w25q64_driver * self, uint32_t block_address);
    void (* pf_erase_chip)(struct w25q64_driver * self);
    void (* pf_write_page)(struct w25q64_driver * self, uint32_t page_address, const uint8_t * data, uint32_t length);
    void (* pf_write)(struct w25q64_driver * self, uint32_t address, const uint8_t * data, uint32_t length);
    void (* pf_read)(struct w25q64_driver * self, uint32_t address, uint8_t * data, uint32_t length);
};


/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_w25q64_link(w25q64_driver_t * drv, w25q64_handle_t * handle);

#if W25Q64_DEBUG_ENABLE
void bsp_driver_w25q64_log_init(pf_printf_t cb, w25q_log_level_t level);
#endif  
/**********************
 *      MACROS
 **********************/
#define W25Q64_PAGE_SIZE                    256
#define W25Q64_SECTOR_SIZE                  4096
#define W25Q64_BLOCK_32K_SIZE               32768
#define W25Q64_BLOCK_64K_SIZE               65536
#define W25Q64_TOTAL_SIZE                   8388608  /* 8MB */

#define W25Q64_JEDEC_MF_ID                  0xEFU
#define W25Q64_JEDEC_DIE_ID                 0x40U
#define W25Q64_JEDEC_PN_ID                  0x17U

#define W25Q64_WRITE_ENABLE_CMD             0x06U 
#define W25Q64_WRITE_DISABLE_CMD            0x04U
#define W25Q64_READ_STATUS_REG_CMD          0x05U
#define W25Q64_WRITE_STATUS_REG_CMD         0x01U
#define W25Q64_READ_DATA_CMD                0x03U
#define W25Q64_FAST_READ_CMD                0x0BU
#define W25Q64_PAGE_PROGRAM_CMD             0x02U
#define W25Q64_SECTOR_ERASE_CMD             0x20U
#define W25Q64_BLOCK_32K_ERASE_CMD          0x52U
#define W25Q64_BLOCK_64K_ERASE_CMD          0xD8U
#define W25Q64_CHIP_ERASE_CMD               0xC7U
#define W25Q64_READ_DEVICE_ID_CMD           0x9FU


#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_W25Q64_H*/


/******************************* (END OF FILE) *********************************/

