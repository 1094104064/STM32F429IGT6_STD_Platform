/**
  ******************************************************************************
  *
  * @file    bsp_adapter_sflash.c
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
#include "bsp_adapter_sflash.h"
#include "bsp_wrapper_sflash.h"
#include "bsp_driver_w25q64.h"
#include "bsp_port_w25q64.h"
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
static int bsp_adapter_sflash_init(struct sflash_wrapper * self);
static void bsp_adapter_sflash_get_device_id(struct sflash_wrapper * self, uint32_t * id);
static void bsp_adapter_sflash_read(struct sflash_wrapper * self, uint32_t address, uint8_t * data, uint32_t length);
static void bsp_adapter_sflash_write(struct sflash_wrapper * self, uint32_t address, const uint8_t * data, uint32_t length);
static void bsp_adapter_sflash_erase(struct sflash_wrapper * self, uint32_t address, uint32_t length);
static void bsp_adapter_sflash_erase_chip(struct sflash_wrapper * self);
/**********************
 *  STATIC VARIABLES
 **********************/
static struct w25q64_driver w25q64_drv;
static struct w25q64_oper w25q64_ops = {
    .pf_spi_read_write = bsp_port_w25q64_read_write,
    .pf_spi_cs_high = bsp_port_w25q64_cs_high,
    .pf_spi_cs_low = bsp_port_w25q64_cs_low
};


/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_sflash_register(void)
{
    struct sflash_wrapper w25q64_wrapper = {

        .pf_init            = bsp_adapter_sflash_init,
        .pf_get_device_id   = bsp_adapter_sflash_get_device_id,
        .pf_read            = bsp_adapter_sflash_read,
        .pf_write           = bsp_adapter_sflash_write,
        .pf_erase           = bsp_adapter_sflash_erase,
        .pf_erase_chip      = bsp_adapter_sflash_erase_chip,
    };

    bsp_wrapper_sflash_link(&w25q64_wrapper, "w25q64", NULL);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/

static int bsp_adapter_sflash_init(struct sflash_wrapper * self)
{
    bsp_driver_w25q64_link(&w25q64_drv, &w25q64_ops);

    if(w25q64_drv.pf_init               == NULL || w25q64_drv.pf_read_id            == NULL || 
        w25q64_drv.pf_erase_sector      == NULL || w25q64_drv.pf_erase_block_32k    == NULL || 
        w25q64_drv.pf_erase_block_64k   == NULL || w25q64_drv.pf_erase_chip         == NULL ||
        w25q64_drv.pf_write_page        == NULL || w25q64_drv.pf_write              == NULL ||
        w25q64_drv.pf_write             == NULL) {
        return 1;
    }

    if(w25q64_drv.pf_init(&w25q64_drv) == false) {
        return 2;
    }

    return 0;
}

static void bsp_adapter_sflash_get_device_id(struct sflash_wrapper * self, uint32_t * id)
{
    w25q64_drv.pf_read_id(&w25q64_drv, id);
}

static void bsp_adapter_sflash_read(struct sflash_wrapper * self, uint32_t address, uint8_t * data, uint32_t length)
{
    w25q64_drv.pf_read(&w25q64_drv, address, data, length);
}

static void bsp_adapter_sflash_erase(struct sflash_wrapper * self, uint32_t address, uint32_t length)
{
    if(length <= 4 * 1024) {
        w25q64_drv.pf_erase_sector(&w25q64_drv, address);
    }
    else if(length <= 32 * 1024) {
        w25q64_drv.pf_erase_block_32k(&w25q64_drv, address);
    }
    else if(length <= 64 * 1024) {
        w25q64_drv.pf_erase_block_64k(&w25q64_drv, address);
    }
    else {
        w25q64_drv.pf_erase_chip(&w25q64_drv);
    }
}

static void bsp_adapter_sflash_write(struct sflash_wrapper * self, uint32_t address, const uint8_t * data, uint32_t length)
{
    w25q64_drv.pf_write(&w25q64_drv, address, data, length);
}

static void bsp_adapter_sflash_erase_chip(struct sflash_wrapper * self)
{
    w25q64_drv.pf_erase_chip(&w25q64_drv);
}

/******************************* (END OF FILE) *********************************/

