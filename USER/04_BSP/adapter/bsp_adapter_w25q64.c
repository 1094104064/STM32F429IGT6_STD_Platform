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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int ops_w25q64_init(void)
{


    return 0;
}

static void ops_w25q64_read_jedec_id(uint32_t * id)
{

}

static bool ops_w25q64_read(uint32_t address, uint8_t * dst, uint32_t length)
{
    return true;
}

static bool ops_w25q64_fast_read(uint32_t address, uint8_t * dst, uint32_t length)
{
    return true;
}

static bool ops_w25q64_erase_chip(void)
{
    return true;
}

static bool ops_w25q64_write(uint32_t address, const uint8_t * src, uint32_t length)
{
    return true;
}

static bool ops_w25q64_erase_sector(uint32_t address, uint32_t length)
{
    return true;
}

static bool ops_w25q64_erase_block_32k(uint32_t address, uint32_t length)
{
    return true;
}

static bool ops_w25q64_erase_block_64k(uint32_t address, uint32_t length)
{
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



 







