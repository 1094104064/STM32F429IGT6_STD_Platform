/**
  ******************************************************************************
  *
  * @file    bsp_driver_gt911.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_gt911 module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_DRIVER_GT911_H
#define _BSP_DRIVER_GT911_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "user_conf.h"
/*********************
 *      DEFINES
 *********************/
#define GT911_TOUCH_MAX 5
/**********************
 *      TYPEDEFS
 **********************/

struct gt911_oper_i2c {
    void (* pf_write_reg)(uint8_t dev_addr, uint16_t reg_addr, uint8_t * src, uint16_t len);
    void (* pf_read_reg)(uint8_t dev_addr, uint16_t reg_addr, uint8_t * dst, uint16_t len);
};

struct gt911_oper_ctrl {
    void (* pf_int_in)(void);
    void (* pf_int_out)(void);
    void (* pf_int_high)(void);
    void (* pf_int_low)(void);
    void (* pf_rst_high)(void);
    void (* pf_rst_low)(void);
};

struct gt911_oper {
    struct gt911_oper_i2c * oper_i2c;
    struct gt911_oper_ctrl * oper_ctrl;

    void (* pf_delay_ms)(uint32_t ms);
};

struct gt911_driver {
    struct gt911_oper * oper;

    void (* pf_init)(struct gt911_driver * self);
    void (* pf_reset)(struct gt911_driver * self);
    void (* pf_read_id)(struct gt911_driver * self, uint8_t * id);
    void (* pf_read_resolution)(struct gt911_driver * self, uint16_t * width, uint16_t * height);
    void (* pf_read_firmware_version)(struct gt911_driver * self, uint8_t * version);
    void (* pf_scan)(struct gt911_driver * self);
    uint8_t (* pf_is_pressed)(struct gt911_driver * self);
    void (* pf_get_coordinates)(struct gt911_driver * self, uint16_t * x, uint16_t * y, uint8_t num);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_gt911_link( struct gt911_driver * self, 
                            struct gt911_oper * oper,
                            struct gt911_oper_i2c * oper_i2c,
                            struct gt911_oper_ctrl * oper_ctrl);
/**********************
 *      MACROS
 **********************/
#define GT911_DEV_ADD       0xBA
#define GT911_CFG_REG       0x8047
#define GT911_COORD_REG     0x814E
#define GT911_CLEAR_REG     0x814E
#define GT911_COMMAND_REG   0x8040
#define GT911_ID_REG        0x8140
#define GT911_FIRMWARE_REG  0x8144
#define GT911_RES_REG       0x8048


#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_GT911_H*/


/******************************* (END OF FILE) *********************************/

