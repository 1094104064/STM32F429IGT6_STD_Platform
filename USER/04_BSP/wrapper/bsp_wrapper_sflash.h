/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_sflash.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_sflash module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_WRAPPER_SFLASH_H
#define _BSP_WRAPPER_SFLASH_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/
#define SFLASH_MAX_NUM  2
/**********************
 *      TYPEDEFS
 **********************/

struct sflash_wrapper {
    uint8_t idx;
    const char * name;
    void * user_data;

    int (*pf_init)(struct sflash_wrapper * self);
    void (* pf_get_device_id)(struct sflash_wrapper * self, uint32_t * id);
    void (* pf_read)(struct sflash_wrapper * self, uint32_t address, uint8_t * data, uint32_t length);
    void (* pf_erase)(struct sflash_wrapper * self, uint32_t address, uint32_t length);
    void (* pf_write)(struct sflash_wrapper * self, uint32_t address, const uint8_t * data, uint32_t length);
    void (* pf_erase_chip)(struct sflash_wrapper * self);

};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_wrapper_sflash_link(struct sflash_wrapper * self, const char * const name, void * const user_data);
bool bsp_wrapper_sflash_init(void);
void bsp_wrapper_sflash_deinit(void);
void bsp_wrapper_sflash_get_device_id(uint32_t * id);
void bsp_wrapper_sflash_read(uint32_t address, uint8_t *data, uint32_t length);
void bsp_wrapper_sflash_erase(uint32_t address, uint32_t length);
void bsp_wrapper_sflash_write(uint32_t address, const uint8_t *data, uint32_t length);
void bsp_wrapper_sflash_erase_write(uint32_t address, const uint8_t *data, uint32_t length);
void bsp_wrapper_sflash_chip_erase(void);
void bsp_wrapper_sflash_test(void);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_SFLASH_H*/


/******************************* (END OF FILE) *********************************/

