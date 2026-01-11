/**
  ******************************************************************************
  *
  * @file    ud_w25qxx.h
  * @author  
  * @brief   Header file of w25qxx module.
  *
  ******************************************************************************
  **/

#ifndef _UD_W25QXX_H
#define _UD_W25QXX_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../core/ud_sflash.h"
#include "../adapter/flash_spi.h"
/*********************
 *      DEFINES
 *********************/
#define W25QXX_PAGE_SIZE                    256
#define W25QXX_SECTOR_SIZE                  4096
#define W25QXX_BLOCK_32K_SIZE               32768
#define W25QXX_BLOCK_64K_SIZE               65536
#define W25Q64_TOTAL_SIZE                   8388608  /* 8MB */
#define W25Q128_TOTAL_SIZE                  16777216  /* 16MB */

#define W25Q64_JEDEC_MF_ID                  0xEFU
#define W25Q64_JEDEC_DIE_ID                 0x40U
#define W25Q64_JEDEC_PN_ID                  0x17U

#define W25QXX_WRITE_ENABLE_CMD             0x06U 
#define W25QXX_WRITE_DISABLE_CMD            0x04U
#define W25QXX_READ_STATUS_REG_CMD          0x05U
#define W25QXX_WRITE_STATUS_REG_CMD         0x01U
#define W25QXX_READ_DATA_CMD                0x03U
#define W25QXX_FAST_READ_CMD                0x0BU
#define W25QXX_PAGE_PROGRAM_CMD             0x02U
#define W25QXX_SECTOR_ERASE_CMD             0x20U
#define W25QXX_BLOCK_32K_ERASE_CMD          0x52U
#define W25QXX_BLOCK_64K_ERASE_CMD          0xD8U
#define W25QXX_CHIP_ERASE_CMD               0xC7U
#define W25QXX_READ_DEVICE_ID_CMD           0x9FU

struct ud_w25qxx
{
    const struct ud_sflash_ops * flash_ops;
    const struct flash_spi_ops ** spi_ops;
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_UD_W25QXX_H*/


/******************************* (END OF FILE) *********************************/


