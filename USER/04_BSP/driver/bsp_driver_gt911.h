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

#define GT911_DEBUG_ENABLE 1

/**********************
 *      TYPEDEFS
 **********************/
typedef int (* pf_printf_t)(const char* format, ...);

typedef enum 
{
    GT911_LOG_NONE = 0,
    GT911_LOG_ERROR,
    GT911_LOG_INFO,
    GT911_LOG_DEBUG,
} gt911_log_level_t;

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


typedef struct gt911_gesture_init gt911_gesture_init_t;
typedef struct gt911_touch gt911_touch_t;
typedef struct gt911_multi_touch gt911_multi_touch_t;
typedef struct gt911_ctrl gt911_ctrl_t;
typedef struct gt911_timebase gt911_timebase_t;
typedef struct gt911_i2c gt911_i2c_t;
typedef struct gt911_handle gt911_handle_t;
typedef struct gt911_driver gt911_driver_t;

struct gt911_gesture_init
{
    uint32_t  distance_left_right;
    uint32_t  distance_up_down;
    uint8_t   refresh_rate;
    uint8_t   gesture_threshold;
    uint8_t   gain;
};

struct gt911_touch
{
    bool        is_pressed;
    uint32_t    coord_x;
    uint32_t    coord_y;
};

struct gt911_multi_touch 
{
    uint8_t     touch_nums;
    uint32_t    coord_x[GT911_TOUCH_MAX];
    uint32_t    coord_y[GT911_TOUCH_MAX];
    uint32_t    touch_weight[GT911_TOUCH_MAX];
    uint32_t    touch_track_id[GT911_TOUCH_MAX];
};

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
    void (* pf_touch_calibration)       (gt911_driver_t * self);
    void (* pf_config_gesture)          (gt911_driver_t * self, gt911_gesture_init_t * gesture_init);
    void (* pf_read_firmware_version)   (gt911_driver_t * self, uint8_t * version);
    void (* pf_read_product_id)         (gt911_driver_t * self, uint8_t * id);
    void (* pf_read_touch_coord)        (gt911_driver_t * self, gt911_touch_t * touchpad);
    void (* pf_read_multi_touch_coord)  (gt911_driver_t * self, gt911_multi_touch_t * multi_touch);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_gt911_link(gt911_driver_t * drv, const gt911_handle_t * handle);

#if GT911_DEBUG_ENABLE
void bsp_driver_gt911_log_init(pf_printf_t cb, gt911_log_level_t level);
#endif  
/**********************
 *      MACROS
 **********************/

/* 7bit address */
#define GT911_I2C_ADDRESS               0x5DU

/* Touch GT911 IDs: "911" */
#define GT911_ID                        0x00313139U
#define GT911_ID1                       0x39U
#define GT911_ID2                       0x31U
#define GT911_ID3                       0x31U

/* Possible values of GT911_DEV_MODE_REG */
#define GT911_DEV_MODE_WORKING          0x00U
#define GT911_DEV_MODE_FACTORY          0x04U

/* Possible values of GT911_GEST_ID_REG */
#define GT911_GEST_ID_NO_GESTURE        0x00U
#define GT911_GEST_ID_SWIPE_RIGHT       0xAAU
#define GT911_GEST_ID_SWIPE_LEFT        0xBBU
#define GT911_GEST_ID_SWIPE_DOWN        0xABU
#define GT911_GEST_ID_SWIPE_UP          0xBAU
#define GT911_GEST_ID_DOUBLE_TAP        0xCCU

/* Values Pn_XH and Pn_YH related */
#define GT911_TOUCH_EVT_FLAG_PRESS_DOWN 0x00U
#define GT911_TOUCH_EVT_FLAG_LIFT_UP    0x01U
#define GT911_TOUCH_EVT_FLAG_CONTACT    0x02U
#define GT911_TOUCH_EVT_FLAG_NO_EVENT   0x03U

/* Possible values of GT911_MSW1_REG(Module_Switch1) */
#define GT911_M_SW1_INTERRUPT_RISING    0x00U
#define GT911_M_SW1_INTERRUPT_FALLING   0x01U
#define GT911_M_SW1_INTERRUPT_LOW       0x02U
#define GT911_M_SW1_INTERRUPT_HIGH      0x03U

/* Mask for reading the MSW1 register without INT trigger */
#define GT911_M_SW1_DATA_MASK           0xFCU


#define GT911_TOUCH_READY(status)       ((status) & 0x80U)                    // 是否有新数据
#define GT911_TOUCH_COUNT(status)       ((status) & 0x0FU)                    // 有效触点数 0~5
#define GT911_HAS_KEY(status)           ((status) & 0x10U)                    // 是否有按键（极少用）
#define GT911_IS_LARGE_TOUCH(status)    ((status) & 0x20U)                    // 大面积触摸标志（可忽略）

/* Current mode register of the GT911 (R/W) */
#define GT911_DEV_MODE_REG         0x00U

/* Gesture ID register */
#define GT911_GEST_ID_REG          0x814BU

/* Gesture mode enabled */
#define GT911_GESTURE_EN           0x8U

/* Gesture Coordinates registers */
#define GT911_START_X_L            0x814DU
#define GT911_START_X_H            0x814EU
#define GT911_START_Y_L            0x814FU
#define GT911_START_Y_H            0x8150U
#define GT911_END_X_L              0x8151U
#define GT911_END_X_H              0x8152U
#define GT911_END_Y_L              0x8153U
#define GT911_END_Y_H              0x8154U
#define GT911_WEIGHT_L             0x8155U
#define GT911_WEIGHT_H             0x8156U
#define GT911_HEIGHT_L             0x8157U
#define GT911_HEIGHT_H             0x8158U


/* Touch Data Status register : gives number of active touch points (0..5) */
#define GT911_TD_STAT_REG          0x814EU

/* P1 X, Y coordinates, weight(Point size) and track-id registers */
#define GT911_P1_XL_REG            0x8150U
#define GT911_P1_XH_REG            0x8151U
#define GT911_P1_YL_REG            0x8152U
#define GT911_P1_YH_REG            0x8153U
#define GT911_P1_WEIGHTL_REG       0x8154U
#define GT911_P1_WEIGHTH_REG       0x8155U
#define GT911_P1_TID_REG           0x8157U

/* P2 X, Y coordinates, weight and tid registers */
#define GT911_P2_XL_REG            0x8158U
#define GT911_P2_XH_REG            0x8159U
#define GT911_P2_YL_REG            0x815AU
#define GT911_P2_YH_REG            0x815BU
#define GT911_P2_WEIGHTL_REG       0x815CU
#define GT911_P2_WEIGHTH_REG       0x815DU
#define GT911_P2_TID_REG           0x815FU

/* P3 X, Y coordinates, weight and tid registers */
#define GT911_P3_XL_REG            0x8160U
#define GT911_P3_XH_REG            0x8161U
#define GT911_P3_YL_REG            0x8162U
#define GT911_P3_YH_REG            0x8163U
#define GT911_P3_WEIGHTL_REG       0x8164U
#define GT911_P3_WEIGHTH_REG       0x8165U
#define GT911_P3_TID_REG           0x8167U

/* P4 X, Y coordinates, weight and tid registers */
#define GT911_P4_XL_REG            0x8168U
#define GT911_P4_XH_REG            0x8169U
#define GT911_P4_YL_REG            0x816AU
#define GT911_P4_YH_REG            0x816BU
#define GT911_P4_WEIGHTL_REG       0x816CU
#define GT911_P4_WEIGHTH_REG       0x816DU
#define GT911_P4_TID_REG           0x816FU

/* P5 X, Y coordinates, weight and tid registers */
#define GT911_P5_XL_REG            0x8170U
#define GT911_P5_XH_REG            0x8171U
#define GT911_P5_YL_REG            0x8172U
#define GT911_P5_YH_REG            0x8173U
#define GT911_P5_WEIGHTL_REG       0x8174U
#define GT911_P5_WEIGHTH_REG       0x8175U
#define GT911_P5_TID_REG           0x8177U

/* Threshold for touch detection */
#define GT911_TH_GROUP_REG         0x80U

/* Filter function coefficients */
#define GT911_TH_DIFF_REG          0x85U

/* Control register */
#define GT911_CTRL_REG             0x86U

/* The time period of switching from Active mode to Monitor mode when there is no touching */
#define GT911_TIMEENTERMONITOR_REG 0x87U

/* Report rate in Active mode */
#define GT911_PERIODACTIVE_REG     0x88U

/* Report rate in Monitor mode */
#define GT911_PERIODMONITOR_REG    0x89U

/* Maximum offset while Moving Left and Moving Right gesture */
#define GT911_OFFSET_LR_REG        0x92U

/* Maximum offset while Moving Up and Moving Down gesture */
#define GT911_OFFSET_UD_REG        0x93U

/* Minimum distance while moving gesture */
#define GT911_DIS_GESTURE_REG      0x8071U

/* High 8-bit of LIB Version info */
#define GT911_LIB_VER_H_REG        0xA1U

/* Low 8-bit of LIB Version info */
#define GT911_LIB_VER_L_REG        0xA2U

/* Chip Selecting */
#define GT911_CIPHER_REG           0xA3U

/* Module_Switch1 register for Interrupt */
#define GT911_MSW1_REG             0x804DU

/* Current power mode the GT911 system is in (R) */
#define GT911_PWR_MODE_REG         0xA5U

/* GT911 firmware version */
#define GT911_FIRMID_REG           0x8144U

/* GT911 Chip identification register */
#define GT911_CHIP_ID_REG          0x8140U

/* Release code version */
#define GT911_RELEASE_CODE_ID_REG  0xAFU

/* Current operating mode the GT911 system is in (R) */
#define GT911_COMMAND_REG          0x8040U

/* Coordinates report rate (= 5+N ms) */
#define GT911_REFRESH_RATE_REG     0x8056U
#define GT911_REFRESH_RATE_MSK     0x0FU

/* Version number configuration */
#define GT911_CONFIG_VERS_REG      0x8047U

/* Checksum configuration register */
#define GT911_CONFIG_CHKSUM_REG    0x80FFU

/* Configuration update flag register */
#define GT911_CONFIG_FRESH_REG     0x8100U

/* Command check register */
#define GT911_COMMAND_CHK_REG      0x8046U

/* Gesture configuration registers */
#define GT911_GESTURE_PRESS_TIME   0x8072U
#define GT911_GESTURE_TIME_ABORT   0x00U

#define GT911_GESTURE_SLOPE_ADJUST 0x8073U
#define GT911_GESTURE_ADJUST_VAL   0x00U

#define GT911_GESTURE_CTRL_REG     0x8074U
#define GT911_GESTURE_INVALID_TIM  0x0FU

#define GT911_GESTURE_SWITCH1_REG  0x8075U
#define GT911_GESTURE_SWITCH2_REG  0x8076U
#define GT911_GESTURE_SWITCH1_VAL  0x00U
#define GT911_GESTURE_SWITCH2_VAL  0x00U

#define GT911_GESTURE_REFRESH_REG  0x8077U

#define GT911_GESTURE_TH_REG       0x8078U



/* DEVICE_MODE */
#define   GT911_DEV_MODE_BIT_MASK                   0x70U
#define   GT911_DEV_MODE_BIT_POSITION               4U

/*  GEST_ID */
#define   GT911_GEST_ID_BIT_MASK                    0xFFU
#define   GT911_GEST_ID_BIT_POSITION                0U

/* First Event Flag */
#define   GT911_P1_XH_EF_BIT_MASK                   0xC0U
#define   GT911_P1_XH_EF_BIT_POSITION               6U

/* First Touch X Position */
#define   GT911_P1_XL_TP_BIT_MASK                   0xFFU
#define   GT911_P1_XL_TP_BIT_POSITION               0U

/* First Touch X Position */
#define   GT911_P1_XH_TP_BIT_MASK                   0x0FU
#define   GT911_P1_XH_TP_BIT_POSITION               1U

/* First Touch Y Position */
#define   GT911_P1_YL_TP_BIT_MASK                   0xFFU
#define   GT911_P1_YL_TP_BIT_POSITION               2U

/* First Touch Y Position */
#define   GT911_P1_YH_TP_BIT_MASK                   0x0FU
#define   GT911_P1_YH_TP_BIT_POSITION               3U

/* First Touch Weight(pressure) */
#define   GT911_P1_WEIGHT_BIT_MASK                  0xFFFU
#define   GT911_P1_WEIGHT_BIT_POSITION              4U

/* First Touch track-ID */
#define   GT911_P1_TID_BIT_MASK                     0xFFU
#define   GT911_P1_TID_BIT_POSITION                 7U

/* Second Event Flag */
#define   GT911_P2_XH_EF_BIT_MASK                   0xC0U
#define   GT911_P2_XH_EF_BIT_POSITION               6U

/* Second Touch X Position */
#define   GT911_P2_XH_TP_BIT_MASK                   0x0FU
#define   GT911_P2_XH_TP_BIT_POSITION               8U

/* Second Touch X Position */
#define   GT911_P2_XL_TP_BIT_MASK                   0xFFU
#define   GT911_P2_XL_TP_BIT_POSITION               9U

/* Second Touch ID */
#define   GT911_P2_YH_TID_BIT_MASK                  0xF0U
#define   GT911_P2_YH_TID_BIT_POSITION              15U

/* Second Touch Y Position */
#define   GT911_P2_YH_TP_BIT_MASK                   0x0FU
#define   GT911_P2_YH_TP_BIT_POSITION               11U

/* Second Touch Y Position */
#define   GT911_P2_YL_TP_BIT_MASK                   0xFFU
#define   GT911_P2_YL_TP_BIT_POSITION               10U

/* Second Touch Weight(pressure) */
#define   GT911_P2_WEIGHT_BIT_MASK                  0xFFFU
#define   GT911_P2_WEIGHT_BIT_POSITION              12U

/* Second Touch Area */
#define   GT911_P2_TID_BIT_MASK                     0xFFU
#define   GT911_P2_TID_BIT_POSITION                 15U

/* Third Event Flag */
#define   GT911_P3_XH_EF_BIT_MASK                   0xC0U
#define   GT911_P3_XH_EF_BIT_POSITION               16U

/* Third Touch X High Position */
#define   GT911_P3_XH_TP_BIT_MASK                   0x0FU
#define   GT911_P3_XH_TP_BIT_POSITION               17U

/* Third Touch X Low Position */
#define   GT911_P3_XL_TP_BIT_MASK                   0xFFU
#define   GT911_P3_XL_TP_BIT_POSITION               16U

/* Third Touch ID */
#define   GT911_P3_YH_TID_BIT_MASK                  0xF0U
#define   GT911_P3_YH_TID_BIT_POSITION              23U

/* Third Touch Y High Position */
#define   GT911_P3_YH_TP_BIT_MASK                   0x0FU
#define   GT911_P3_YH_TP_BIT_POSITION               18U

/* Third Touch Y Low Position */
#define   GT911_P3_YL_TP_BIT_MASK                   0xFFU
#define   GT911_P3_YL_TP_BIT_POSITION               17U

/* Third Touch Weight(pressure) */
#define   GT911_P3_WEIGHT_BIT_MASK                  0xFFFU
#define   GT911_P3_WEIGHT_BIT_POSITION              19U

/* Third Touch Area */
#define   GT911_P3_TID_BIT_MASK                     0xFFU
#define   GT911_P3_TID_BIT_POSITION                 23U

/* Fourth Event Flag */
#define   GT911_P4_XH_EF_BIT_MASK                   0xC0U
#define   GT911_P4_XH_EF_BIT_POSITION               25

/* Fourth Touch X High Position */
#define   GT911_P4_XH_TP_BIT_MASK                   0x0FU
#define   GT911_P4_XH_TP_BIT_POSITION               25U

/* Fourth Touch X Low Position */
#define   GT911_P4_XL_TP_BIT_MASK                   0xFFU
#define   GT911_P4_XL_TP_BIT_POSITION               24U

/* Fourth Touch ID */
#define   GT911_P4_YH_TID_BIT_MASK                  0xF0U
#define   GT911_P4_YH_TID_BIT_POSITION              31U

/* Fourth Touch Y High Position */
#define   GT911_P4_YH_TP_BIT_MASK                   0x0FU
#define   GT911_P4_YH_TP_BIT_POSITION               27U

/* Fourth Touch Y Low Position */
#define   GT911_P4_YL_TP_BIT_MASK                   0xFFU
#define   GT911_P4_YL_TP_BIT_POSITION               26U

/* Fourth Touch Weight(pressure) */
#define   GT911_P4_WEIGHT_BIT_MASK                  0xFFFU
#define   GT911_P4_WEIGHT_BIT_POSITION              28U

/* Fourth Touch Area */
#define   GT911_P4_TID_BIT_MASK                     0xFFU
#define   GT911_P4_TID_BIT_POSITION                 31U

/* Fifth Event Flag */
#define   GT911_P5_XH_EF_BIT_MASK                   0xC0U
#define   GT911_P5_XH_EF_BIT_POSITION               6U

/* Fifth Touch X High Position */
#define   GT911_P5_XH_TP_BIT_MASK                   0x0FU
#define   GT911_P5_XH_TP_BIT_POSITION               0U

/* Fifth Touch X Low Position */
#define   GT911_P5_XL_TP_BIT_MASK                   0xFFU
#define   GT911_P5_XL_TP_BIT_POSITION               0U

/* Fifth Touch ID */
#define   GT911_P5_YH_TID_BIT_MASK                  0xF0U
#define   GT911_P5_YH_TID_BIT_POSITION              4U

/* Fifth Touch Y High Position */
#define   GT911_P5_YH_TP_BIT_MASK                   0x0FU
#define   GT911_P5_YH_TP_BIT_POSITION               0U

/* Fifth Touch Y Low Position */
#define   GT911_P5_YL_TP_BIT_MASK                   0xFFU
#define   GT911_P5_YL_TP_BIT_POSITION               0U

/* Fifth Touch Weight(pressure) */
#define   GT911_P5_WEIGHT_BIT_MASK                  0xFFFU
#define   GT911_P5_WEIGHT_BIT_POSITION              0U

/* Fifth Touch Area */
#define   GT911_P5_TID_BIT_MASK                     0xFFU
#define   GT911_P5_TID_BIT_POSITION                 4U

/* Threshold for touch detection */
#define   GT911_TH_GROUP_BIT_MASK                   0xFFU
#define   GT911_TH_GROUP_BIT_POSITION               0U

/* Filter function coefficient */
#define   GT911_TH_DIFF_BIT_MASK                    0xFFU
#define   GT911_TH_DIFF_BIT_POSITION                0U

/* CTRL */
#define   GT911_CTRL_BIT_MASK                       0xFFU
#define   GT911_CTRL_BIT_POSITION                   0U

/* TIMEENTERMONITOR */
#define   GT911_TIMEENTERMONITOR_BIT_MASK           0xFFU
#define   GT911_TIMEENTERMONITOR_BIT_POSITION       0U

/* PERIODACTIVE */
#define   GT911_PERIODACTIVE_BIT_MASK               0xFFU
#define   GT911_PERIODACTIVE_BIT_POSITION           0U

/* PERIODMONITOR */
#define   GT911_PERIODMONITOR_BIT_MASK              0xFFU
#define   GT911_PERIODMONITOR_BIT_POSITION          0U

/* DISTANCE_LEFT_RIGHT */
#define   GT911_DISTANCE_LR_BIT_MASK                0x0FU
#define   GT911_DISTANCE_LR_BIT_POSITION            0U

/* DISTANCE_UP_DOWN */
#define   GT911_DISTANCE_UD_BIT_MASK                0xF0U
#define   GT911_DISTANCE_UD_BIT_POSITION            0U

/* LIB_VER_H */
#define   GT911_LIB_VER_H_BIT_MASK                  0xFFU
#define   GT911_LIB_VER_H_BIT_POSITION              0U

/* LIB_VER_L */
#define   GT911_LIB_VER_L_BIT_MASK                  0xFFU
#define   GT911_LIB_VER_L_BIT_POSITION              0U

/* CIPHER */
#define   GT911_CIPHER_BIT_MASK                     0xFFU
#define   GT911_CIPHER_BIT_POSITION                 0U

/* M_SW1 */
#define   GT911_M_SW1_BIT_MASK                      0x03U
#define   GT911_M_SW1_BIT_POSITION                  0U

/* PWR_MODE */
#define   GT911_PWR_MODE_BIT_MASK                   0xFFU
#define   GT911_PWR_MODE_BIT_POSITION               0U

/* FIRMID */
#define   GT911_FIRMID_BIT_MASK                     0xFFU
#define   GT911_FIRMID_BIT_POSITION                 0U

/* FOCALTECH_ID */
#define   GT911_CHIP_ID_BIT_MASK                    0xFFU
#define   GT911_CHIP_ID_BIT_POSITION                0U

/* RELEASE_CODE_ID */
#define   GT911_RC_ID_BIT_MASK                      0xFFU
#define   GT911_RC_ID_BIT_POSITION                  0U

/* STATE */
#define   GT911_STATE_BIT_MASK                      0xFFU
#define   GT911_STATE_BIT_POSITION                  0U







#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_GT911_H*/


/******************************* (END OF FILE) *********************************/

