/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_sflash.c
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
#include "bsp_wrapper_sflash.h"
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
static struct sflash_wrapper sflash_wrappers[SFLASH_MAX_NUM];
static uint8_t current_sflash_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_wrapper_sflash_link(struct sflash_wrapper * self)
{
    uint8_t idx = 0;

    current_sflash_idx++;

    if (current_sflash_idx < SFLASH_MAX_NUM) {
        idx = current_sflash_idx;
    }
    else {
        current_sflash_idx = 0;
    }

    memset(&sflash_wrappers[idx], 0, sizeof(struct sflash_wrapper));

    sflash_wrappers[idx] = *self;
    sflash_wrappers[idx].idx = idx;
}

bool bsp_wrapper_sflash_init(void)
{
    struct sflash_wrapper *self = &sflash_wrappers[current_sflash_idx];

    if( self->pf_init == NULL  || self->pf_get_device_id == NULL ||
        self->pf_erase == NULL || self->pf_read == NULL          ||
        self->pf_write == NULL || self->pf_erase_chip == NULL ) {
        return false;
    }

    if(self->pf_init(self) != 0) {
        return false;
    }

    pr_info("sflash initialized successfully");

    return true;
}


void bsp_wrapper_sflash_deinit(void)
{
    // De-initialization code for serial flash
}

void bsp_wrapper_sflash_get_device_id(uint32_t * id)
{
    struct sflash_wrapper *self = &sflash_wrappers[current_sflash_idx];

    self->pf_get_device_id(self, id);
}

void bsp_wrapper_sflash_read(uint32_t address, uint8_t *data, uint32_t length)
{
    struct sflash_wrapper *self = &sflash_wrappers[current_sflash_idx];

    self->pf_read(self, address, data, length);
}

void bsp_wrapper_sflash_erase(uint32_t address, uint32_t length)
{
    struct sflash_wrapper *self = &sflash_wrappers[current_sflash_idx];

    self->pf_erase(self, address, length);
}


void bsp_wrapper_sflash_write(uint32_t address, const uint8_t *data, uint32_t length)
{
    struct sflash_wrapper *self = &sflash_wrappers[current_sflash_idx];

    self->pf_write(self, address, data, length);
}

void bsp_wrapper_sflash_erase_write(uint32_t address, const uint8_t *data, uint32_t length)
{
    bsp_wrapper_sflash_erase(address, length);
    bsp_wrapper_sflash_write(address, data, length);
}


void bsp_wrapper_sflash_chip_erase(void)
{
    struct sflash_wrapper *self = &sflash_wrappers[current_sflash_idx];

    self->pf_erase_chip(self);
}


void bsp_wrapper_sflash_test(void)
{
    uint8_t write_buf[256] = {0};
    uint8_t read_buf[256] = {0};
    uint32_t chip_id = {0};
    uint32_t address = 0x000000;

    bsp_wrapper_sflash_get_device_id(&chip_id);
    pr_info("sflash chip id: 0x%X", chip_id);

    memset(write_buf, 0xAA, sizeof(write_buf));

    bsp_wrapper_sflash_erase(address, sizeof(write_buf));
    bsp_wrapper_sflash_write(address, write_buf, sizeof(write_buf));
    bsp_wrapper_sflash_read(address, read_buf, sizeof(read_buf));

    if (memcmp(write_buf, read_buf, sizeof(write_buf)) == 0) {
        pr_info("sflash read/write test passed");
    } else {
        pr_info("sflash read/write test failed");
    }

}





/******************************* (END OF FILE) *********************************/
