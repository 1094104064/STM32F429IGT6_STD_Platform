/**
  ******************************************************************************
  *
  * @file    bsp_adapter_w25q64.c
  * @author  
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
#include "bsp_wrapper_sflash.h"
#include "bsp_adapter_w25q64.h"
#include "bsp_driver_w25q64.h"
#include "bsp_linker_w25q64.h"
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

/**********************
 *  STATIC VARIABLES
 **********************/
static w25q64_driver_t gs_w25q64_drvier;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int ops_w25q64_init(void)
{
    static const w25q64_handle_t handle = {
        .pf_spi_init        = bsp_linker_w25q64_spi_init,
        .pf_spi_read_write  = bsp_linker_w25q64_spi_read_write,
        .pf_spi_cs_high     = bsp_linker_w25q64_spi_cs_high,
        .pf_spi_cs_low      = bsp_linker_w25q64_spi_cs_low,
    };

    bsp_driver_w25q64_link(&gs_w25q64_drvier, &handle);

    if(false == gs_w25q64_drvier.pf_init(&gs_w25q64_drvier)) {
        return 2;
    }

    return 0;
}

static bool ops_w25q64_read_jedec_id(uint32_t * id)
{
    return gs_w25q64_drvier.pf_read_id(&gs_w25q64_drvier, id);
}

static bool ops_w25q64_read(uint32_t address, uint8_t * dst, uint32_t length)
{
    if(0 != gs_w25q64_drvier.pf_read(&gs_w25q64_drvier, address, dst, length)) {
        return false;
    }
    return true;
}

static bool ops_w25q64_fast_read(uint32_t address, uint8_t * dst, uint32_t length)
{
    return true;
}

static bool ops_w25q64_erase_chip(void)
{
    if(0 != gs_w25q64_drvier.pf_erase_chip(&gs_w25q64_drvier)) {
        return false;
    }
    return true;
}

static bool ops_w25q64_write(uint32_t address, const uint8_t * src, uint32_t length)
{
    return gs_w25q64_drvier.pf_write(&gs_w25q64_drvier, address, src, length);
}

static bool ops_w25q64_erase_sector(uint32_t address, uint32_t length)
{
    if(0 != gs_w25q64_drvier.pf_erase_sector(&gs_w25q64_drvier, address)) {
        return false;
    }
    return true;
}

static bool ops_w25q64_erase_block_32k(uint32_t address, uint32_t length)
{
    if(0 != gs_w25q64_drvier.pf_erase_block_32k(&gs_w25q64_drvier, address)) {
        return false;
    }
    return true;
}

static bool ops_w25q64_erase_block_64k(uint32_t address, uint32_t length)
{
    if(0 != gs_w25q64_drvier.pf_erase_block_64k(&gs_w25q64_drvier, address)) {
        return false;
    }
    return true;
}

static const sflash_ops_t w25q64_ops = {
    .pf_init                = ops_w25q64_init,
    .pf_read_jedec_id       = ops_w25q64_read_jedec_id,
    .pf_read                = ops_w25q64_read,
    .pf_fast_read           = ops_w25q64_fast_read,
    .pf_erase_chip          = ops_w25q64_erase_chip,
    .pf_write               = ops_w25q64_write,
    .eraser = {
        { W25Q64_SECTOR_SIZE,    ops_w25q64_erase_sector    },
        { W25Q64_BLOCK_32K_SIZE, ops_w25q64_erase_block_32k },
        { W25Q64_BLOCK_64K_SIZE, ops_w25q64_erase_block_64k },
    },
};

void InitW25Q64(void)
{
    sflash_obj_t * w25q64_obj = NULL;

    w25q64_obj = wrp_sflash.obj_create(&w25q64_ops, "w25q64", NULL);
    
    if (w25q64_obj == NULL) {
        for (;;) ;
    }

    wrp_sflash.init(w25q64_obj);
}

/******************************* (END OF FILE) *********************************/



 







