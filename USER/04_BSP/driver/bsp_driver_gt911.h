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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define GT911_TOUCH_MAX 5

#define GT911_DEBUG_ENABLE 0

#if GT911_DEBUG_ENABLE

    #define gt911_dbg(fmt, ...)         printf("%s [%d] : " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

    #define gt911_assert_null(param)                                                            \
            do {                                                                                \
                if(param == NULL) { gt911_dbg("NULL pointer: %s", #param); while(1); }          \
            } while (0)    
#else
    #define gt911_dbg(fmt, ...)             do {} while (0)
    #define gt911_assert_null(param)        do {} while (0)
#endif


/**********************
 *      TYPEDEFS
 **********************/
typedef struct gt911_ctrl gt911_ctrl_t;
typedef struct gt911_timebase gt911_timebase_t;
typedef struct gt911_i2c gt911_i2c_t;
typedef struct gt911_handle gt911_handle_t;
typedef struct gt911_driver gt911_driver_t;

typedef enum {
    GT911_I2C_SOFTWARE,
    GT911_I2C_HARDWARE
} gt911_i2c_type_t;

typedef enum {
    GT911_PIN_LOW,
    GT911_PIN_HIGH
} gt911_pin_state_t;

typedef enum {
    GT911_PIN_INPUT,
    GT911_PIN_OUTPUT
} gt911_pin_mode_t;

struct gt911_si2c
{
    void    (* pf_init)         (void);
    void    (* pf_start)        (void);
    void    (* pf_stop)         (void);
    uint8_t (* pf_wait_ack)     (void);
    void    (* pf_generate_ack) (void);
    void    (* pf_generate_nack)(void);
    uint8_t (* pf_read_byte)    (void);
    void    (* pf_write_byte)   (uint8_t data);
};

struct gt911_hi2c
{
    void    (* pf_init)        (void);
    bool    (* pf_start)       (void);
    void    (* pf_stop)        (void);
    bool    (* pf_send_addr)   (uint8_t addr);
    uint8_t (* pf_read_byte)   (void);
    bool    (* pf_write_byte)  (uint8_t data);
    void    (* pf_ack_en)      (uint8_t new_state);
};

struct gt911_ctrl
{
    void (* pf_write_int_pin) (gt911_pin_state_t pin_state);
    void (* pf_write_rst_pin) (gt911_pin_state_t pin_state);
    void (* pf_set_int_pin)   (gt911_pin_mode_t pin_mode);
};

struct gt911_timebase
{
    void (* pf_delay_ms)(uint32_t ticks);
};


struct gt911_i2c 
{
    gt911_i2c_type_t type;
    union {
        struct gt911_si2c si2c;
        struct gt911_hi2c hi2c;
    } connect;
};


struct gt911_handle 
{
    const gt911_i2c_t *  i2c;
    const gt911_ctrl_t * ctrl;
    const gt911_timebase_t * timebase;
};

struct gt911_driver {
    const gt911_handle_t * handle;

    bool (* pf_init)                    (gt911_driver_t * self);
    void (* pf_reset)                   (gt911_driver_t * self);
    void (* pf_get_id)                  (gt911_driver_t * self, uint8_t * id);
    void (* pf_get_resolution)          (gt911_driver_t * self, uint16_t * width, uint16_t * height);
    void (* pf_get_firmware_version)    (gt911_driver_t * self, uint8_t * version);
    void (* pf_scan)                    (gt911_driver_t * self);
    bool (* pf_is_pressed)              (gt911_driver_t * self);
    void (* pf_get_coordinates)         (gt911_driver_t * self, uint16_t * x, uint16_t * y, uint8_t num);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/

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

