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

#if W25Q64_DEBUG_ENABLE

    #define w25q64_dbg(fmt, ...)         printf("%s [%d] : " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

    #define w25q64_assert_null(param)                                                            \
            do {                                                                                \
                if(param == NULL) { w25q64_dbg("NULL pointer: %s", #param); while(1); }          \
            } while (0)    
#else
    #define w25q64_dbg(fmt, ...)             do {} while (0)
    #define w25q64_assert_null(param)        do {} while (0)
#endif

/**********************
 *      TYPEDEFS
 **********************/


struct w25q64_oper {
    void (* pf_spi_read_write)(uint8_t * src, uint8_t * dst, uint32_t size);
    void (* pf_spi_cs_low)(void);
    void (* pf_spi_cs_high)(void);
};


struct w25q64_driver {
    struct w25q64_oper * oper;

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
void bsp_driver_w25q64_link(struct w25q64_driver * self, struct w25q64_oper * oper);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_W25Q64_H*/


/******************************* (END OF FILE) *********************************/

