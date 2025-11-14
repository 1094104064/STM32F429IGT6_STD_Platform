/**
  ******************************************************************************
  *
  * @file    bsp_hal_gt911.h
  * @author  
  * @brief   Header file of bsp_hal_gt911 module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_HAL_GT911_H
#define _BSP_HAL_GT911_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

void    bsp_hal_gt911_si2c_init         (void);
void    bsp_hal_gt911_si2c_start        (void);
void    bsp_hal_gt911_si2c_stop         (void);
uint8_t bsp_hal_gt911_si2c_wait_ack     (void);
void    bsp_hal_gt911_si2c_generate_ack (void);
void    bsp_hal_gt911_si2c_generate_nack(void);
uint8_t bsp_hal_gt911_si2c_read_byte    (void);
void    bsp_hal_gt911_si2c_write_byte   (uint8_t data);
void    bsp_hal_gt911_write_int_pin     (uint8_t new_state);
void    bsp_hal_gt911_write_rst_pin     (uint8_t new_state);
void    bsp_hal_gt911_set_int_pin       (uint8_t mode);
/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_BSP_HAL_GT911_H*/


/******************************* (END OF FILE) *********************************/


