/**
  ******************************************************************************
  *
  * @file    bsp_driver_gt911.c
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
#include "bsp_driver_gt911.h"
/**********************
 *      MACROS
 **********************/
#if GT911_DEBUG_ENABLE

static pf_printf_t printf_cb = NULL;

static gt911_log_level_t log_level = GT911_LOG_DEBUG;

#define LOG_OUTPUT(level, fmt, ...) do { \
    if (printf_cb && level <= log_level) { \
        printf_cb("[GT911] " fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_E(fmt, ...) LOG_OUTPUT(GT911_LOG_ERROR, "E: " fmt"\r\n", ##__VA_ARGS__)    /* 用于输出详细的调试信息 */
#define LOG_I(fmt, ...) LOG_OUTPUT(GT911_LOG_INFO,  "I: " fmt"\r\n", ##__VA_ARGS__)    /* 用于记录重要的操作节点或状态变化 */
#define LOG_D(fmt, ...) LOG_OUTPUT(GT911_LOG_DEBUG, "D: " fmt"\r\n", ##__VA_ARGS__)    /* 用于表示发生了错误 */


#define ASSERT_NULL(param)                                                          \
        do {                                                                        \
            if(param == NULL) { LOG_E("NULL pointer: \r\n", #param); while(1); }    \
        } while (0)

#else

#define LOG_E(fmt, ...)
#define LOG_I(fmt, ...)
#define LOG_D(fmt, ...)

#define ASSERT_NULL(param)                                                          \
        do {                                                                        \
            if(param == NULL) { while(1); }                                         \
        } while (0)

#endif
/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void gt911_read_reg(gt911_driver_t * self, uint8_t dev_addr, uint16_t reg_addr, uint8_t * dst, uint16_t len);
static void gt911_write_reg(gt911_driver_t * self, uint8_t dev_addr, uint16_t reg_addr, uint8_t * src, uint16_t len);
static bool gt911_init(gt911_driver_t * self);
static void gt911_reset(gt911_driver_t * self);
static void gt911_set_working_mode(gt911_driver_t * self, uint8_t mode);
static void gt911_get_working_mode(gt911_driver_t * self, uint8_t * mode);
// static void gt911_read_gesture_id(gt911_driver_t * self, uint8_t * gesture_id);
static void gt911_read_touch_data_status(gt911_driver_t * self, uint8_t  * value);
// static void gt911_clr_int(gt911_driver_t * self);
// static void gt911_read_point_x_event_flag(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_read_point_x_low_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_read_point_x_high_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_read_point_y_low_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_read_point_y_high_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_read_point_weight(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_read_point_track_id(gt911_driver_t * self, uint8_t point_idx, uint8_t * value);
// static void gt911_set_touch_threshold(gt911_driver_t * self, uint8_t value);
// static void gt911_set_filter_coeff(gt911_driver_t * self, uint8_t value);
// static void gt911_ctrl(gt911_driver_t * self, uint8_t value);
// static void gt911_time_enter_monitor(gt911_driver_t * self, uint8_t value);
// static void gt911_period_active(gt911_driver_t * self, uint8_t value);
// static void gt911_period_monitor(gt911_driver_t * self, uint8_t value);
static void gt911_distance_left_right(gt911_driver_t * self, uint8_t value);
static void gt911_distance_up_down(gt911_driver_t * self, uint8_t value);
// static void gt911_read_lib_ver_high(gt911_driver_t * self, uint8_t * value);
// static void gt911_read_lib_ver_low(gt911_driver_t * self, uint8_t * value);
// static void gt911_cipher(gt911_driver_t * self, uint8_t * value);
// static void gt911_m_sw1(gt911_driver_t * self, uint8_t value);
// static void gt911_pwr_mode(gt911_driver_t * self, uint8_t value);
static void gt911_read_firmware_version(gt911_driver_t * self, uint8_t * value);
static void gt911_read_product_id(gt911_driver_t * self, uint8_t * value);
// static void gt911_read_release_code_version(gt911_driver_t * self, uint8_t * value);
// static void gt911_mode(gt911_driver_t * self, uint8_t value);
static void gt911_config_gesture(gt911_driver_t * self, gt911_gesture_init_t * init);
static void gt911_touch_calibration(gt911_driver_t * self);
static uint8_t gt911_read_touch_nums(gt911_driver_t * self);
static void gt911_read_touch_coord(gt911_driver_t * self, gt911_touch_t * touchpad);
static void gt911_read_multi_touch_coord(gt911_driver_t * self, gt911_multi_touch_t * touchpad);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_gt911_link(gt911_driver_t * drv, const gt911_handle_t * handle)
{
    ASSERT_NULL(drv);
    ASSERT_NULL(handle);

    drv->handle = handle;

    drv->pf_init                    = gt911_init;
    drv->pf_reset                   = gt911_reset;
    drv->pf_touch_calibration       = gt911_touch_calibration;
    drv->pf_config_gesture          = gt911_config_gesture;
    drv->pf_read_firmware_version   = gt911_read_firmware_version;
    drv->pf_read_product_id         = gt911_read_product_id;
    drv->pf_read_touch_coord        = gt911_read_touch_coord;
    drv->pf_read_multi_touch_coord  = gt911_read_multi_touch_coord;
}

#if GT911_DEBUG_ENABLE
void bso_driver_gt911_log_init(pf_printf_t cb, gt911_log_level_t level)
{
    printf_cb = cb;
    log_level = level;
}
#endif 
/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
  * @brief  Read data from GT911 register
  * @param  self Pointer to driver instance
  * @param  dev_addr Device address
  * @param  reg_addr Register address
  * @param  dst Pointer to destination buffer
  * @param  len Number of bytes to read
  * @retval Component status
  */
static void gt911_read_reg(gt911_driver_t * self, uint8_t dev_addr, uint16_t reg_addr, uint8_t * dst, uint16_t len)
{
    if(self->handle->i2c->type == GT911_I2C_HARDWARE) {
        const struct gt911_hi2c * i2c = &self->handle->i2c->connect.hi2c;
        bool ret = false;

        ret = i2c->pf_start();
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_send_addr(dev_addr);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_write_byte(reg_addr >> 8);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_write_byte(reg_addr & 0xFF);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_start();
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_send_addr(dev_addr | 0x01);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        for(uint16_t i = 0; i < len; i++) {
            if(i == (len - 1)) {
                i2c->pf_ack_en(false);
                i2c->pf_stop();
            }
            dst[i] = i2c->pf_read_byte();
        }
        i2c->pf_ack_en(true);
    }
    else if(self->handle->i2c->type == GT911_I2C_SOFTWARE) {
        const struct gt911_si2c * i2c = &self->handle->i2c->connect.si2c; 

        i2c->pf_start();

        i2c->pf_write_byte(dev_addr << 1);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        i2c->pf_write_byte(reg_addr >> 8);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        i2c->pf_write_byte(reg_addr & 0xFF);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        i2c->pf_start();
        i2c->pf_write_byte((dev_addr << 1) | 0x01);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        for(uint16_t i = 0; i < len; i++) {
            dst[i] = i2c->pf_read_byte();
            if(i == (len - 1)) {
                i2c->pf_generate_nack();
            }
            else {
                i2c->pf_generate_ack();
            }
        }

        i2c->pf_stop();
    }
}

/**
  * @brief  Write data to GT911 register
  * @param  self Pointer to driver instance
  * @param  dev_addr Device address
  * @param  reg_addr Register address
  * @param  src Pointer to source buffer
  * @param  len Number of bytes to write
  * @retval Component status
  */
static void gt911_write_reg(gt911_driver_t * self, uint8_t dev_addr, uint16_t reg_addr, uint8_t * src, uint16_t len)
{
    if(self->handle->i2c->type == GT911_I2C_HARDWARE) {
        const struct gt911_hi2c * i2c = &self->handle->i2c->connect.hi2c;
        bool ret = false;

        ret = i2c->pf_start();
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_send_addr(dev_addr);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_write_byte(reg_addr >> 8);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        ret = i2c->pf_write_byte(reg_addr & 0xFF);
        if(ret != true) {
            i2c->pf_stop();
            return;
        }

        for(uint16_t i = 0; i < len; i++) {
            if(true != i2c->pf_write_byte(src[i])) {
                i2c->pf_stop();
                return;
            }
        }
        i2c->pf_stop();
    }
    else if(self->handle->i2c->type == GT911_I2C_SOFTWARE) {
        const struct gt911_si2c * i2c = &self->handle->i2c->connect.si2c; 

        i2c->pf_start();

        i2c->pf_write_byte(dev_addr << 1);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        i2c->pf_write_byte(reg_addr >> 8);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        i2c->pf_write_byte(reg_addr & 0xFF);
        if(i2c->pf_wait_ack()) {
            i2c->pf_stop();
            return;
        }

        for(uint16_t i = 0; i < len; i++) {
            i2c->pf_write_byte(src[i]);
            if(i2c->pf_wait_ack()) {
                i2c->pf_stop();
                return;
            }
        }

        i2c->pf_stop();
    }
}

/**
  * @brief  Initialize GT911
  * @param  self Pointer to driver instance
  * @retval Component status
  */
static bool gt911_init(gt911_driver_t * self)
{
    ASSERT_NULL(self->handle->i2c);
    ASSERT_NULL(self->handle->ctrl);
    ASSERT_NULL(self->handle->timebase);

    ASSERT_NULL(self->handle->ctrl->pf_write_int_pin);
    ASSERT_NULL(self->handle->ctrl->pf_write_rst_pin);
    ASSERT_NULL(self->handle->ctrl->pf_set_int_pin);
    ASSERT_NULL(self->handle->timebase->pf_delay_ms);

    if(self->handle->i2c->type == GT911_I2C_HARDWARE) {

        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_init);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_start);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_stop);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_send_addr);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_read_byte);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_write_byte);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_ack_en);

        const struct gt911_hi2c * i2c = &self->handle->i2c->connect.hi2c;
        i2c->pf_init();
    }
    else if(self->handle->i2c->type == GT911_I2C_SOFTWARE) {

        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_init);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_start);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_stop);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_wait_ack);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_generate_ack);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_generate_nack);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_read_byte);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_write_byte);

        const struct gt911_si2c * i2c = &self->handle->i2c->connect.si2c; 
        i2c->pf_init();
    }

    uint8_t info[5] = {0};

    gt911_reset(self);

    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_CHIP_ID_REG, info, 3);

    if((info[0] == '9') &&
       (info[1] == '1') &&
       (info[2] == '1')) {

        return true;
    }
    else {
        return false;
    }
}

/**
  * @brief  Reset GT911
  * @param  self Pointer to driver instance
  */
static void gt911_reset(gt911_driver_t * self)
{
    self->handle->ctrl->pf_set_int_pin(GT911_PIN_OUTPUT);
    self->handle->ctrl->pf_write_int_pin(GT911_PIN_LOW);
    self->handle->ctrl->pf_write_rst_pin(GT911_PIN_HIGH);
    self->handle->timebase->pf_delay_ms(10);

    self->handle->ctrl->pf_write_rst_pin(GT911_PIN_LOW);
    self->handle->timebase->pf_delay_ms(25);
    self->handle->ctrl->pf_write_rst_pin(GT911_PIN_HIGH);
    self->handle->timebase->pf_delay_ms(120);

    self->handle->ctrl->pf_set_int_pin(GT911_PIN_INPUT);
    self->handle->timebase->pf_delay_ms(30);
}

/**
  * @brief  Set GT911 working mode
  * @param  self Pointer to driver instance
  * @param  mode Value to write to GT911_DEV_MODE_REG register
  * @retval Component status
  */
static void gt911_set_working_mode(gt911_driver_t * self, uint8_t mode)
{
  uint8_t tmp;

  gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_DEV_MODE_REG, &tmp, 1U);

    tmp &= ~GT911_DEV_MODE_BIT_MASK;
    tmp |= mode << GT911_DEV_MODE_BIT_POSITION;

   gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_DEV_MODE_REG, &tmp, 1U);
}

/**
  * @brief  Get GT911 working mode
  * @param  self Pointer to driver instance
  * @param  mode pointer to the pValue of GT911_DEV_MODE_REG register
  * @retval Component status
  */
static void gt911_get_working_mode(gt911_driver_t * self, uint8_t * mode)
{
    uint8_t tmp;

    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_DEV_MODE_REG, &tmp, 1U);

    *mode = (tmp & GT911_DEV_MODE_BIT_MASK) >> GT911_DEV_MODE_BIT_POSITION;
}

/**
  * @brief  Get GT911 gesture ID
  * @param  self Pointer to driver instance
  * @param  gesture_id pointer to the pValue of GT911_GEST_ID_REG register
  * @retval Component status
  */
// static void gt911_read_gesture_id(gt911_driver_t * self, uint8_t * gesture_id)
// {
//     gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_GEST_ID_REG, gesture_id, 1);
// }

/**
  * @brief  Get GT911 Touch Data Status
  * @param  self Pointer to driver instance
  * @param  value pointer to the pValue of Touch Data Status register
  * @retval Component status
  */
static void gt911_read_touch_data_status(gt911_driver_t * self, uint8_t  * value)
{
    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_TD_STAT_REG, value, 1);

    if(GT911_TOUCH_READY(*value)) {

        *value = GT911_TOUCH_COUNT(*value);

        uint8_t clear = 0U;
        gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TD_STAT_REG, &clear, 1);
    }
    else {
        *value = 0U;
    }
}

/**
  * @brief  Get GT911 Clear Interrupt/Buffer Status
  * @param  self Pointer to driver instance
  * @retval Component status
  */
// static void gt911_clr_int(gt911_driver_t * self)
// {
//     uint8_t write_data = 0U;
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TD_STAT_REG, &write_data, 1);
// }

/**
  * @brief  Get GT911 point X Event Flag
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_XH_REG register
  * @retval Component status
  */
// static void gt911_read_point_x_event_flag(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_XH_REG, value, 1);
//         *value &= GT911_P1_XH_EF_BIT_MASK;
//         *value = *value >> GT911_P1_XH_EF_BIT_POSITION;
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_XH_REG, value, 1);
//         *value &= GT911_P2_XH_EF_BIT_MASK;
//         *value = *value >> GT911_P2_XH_EF_BIT_POSITION;
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_XH_REG, value, 1);
//         *value &= GT911_P3_XH_EF_BIT_MASK;
//         *value = *value >> GT911_P3_XH_EF_BIT_POSITION;
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_XH_REG, value, 1);
//         *value &= GT911_P4_XH_EF_BIT_MASK;
//         *value = *value >> GT911_P4_XH_EF_BIT_POSITION;
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_XH_REG, value, 1);
//         *value &= GT911_P5_XH_EF_BIT_MASK;
//         *value = *value >> GT911_P5_XH_EF_BIT_POSITION;
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Get GT911 point X Low Touch Position
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_XL_REG register
  * @retval Component status
  */
// static void gt911_read_point_x_low_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_XL_REG, value, 1);
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_XL_REG, value, 1);
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_XL_REG, value, 1);
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_XL_REG, value, 1);
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_XL_REG, value, 1);
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Get GT911 point X High Touch Position
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_XH_REG register
  * @retval Component status
  */
// static void gt911_read_point_x_high_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_XH_REG, value, 1);
//         *value &= GT911_P1_XH_TP_BIT_MASK;
//         *value = *value >> GT911_P1_XH_TP_BIT_POSITION;
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_XH_REG, value, 1);
//         *value &= GT911_P2_XH_TP_BIT_MASK;
//         *value = *value >> GT911_P2_XH_TP_BIT_POSITION;
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_XH_REG, value, 1);
//         *value &= GT911_P3_XH_TP_BIT_MASK;
//         *value = *value >> GT911_P3_XH_TP_BIT_POSITION;
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_XH_REG, value, 1);
//         *value &= GT911_P4_XH_TP_BIT_MASK;
//         *value = *value >> GT911_P4_XH_TP_BIT_POSITION;
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_XH_REG, value, 1);
//         *value &= GT911_P5_XH_TP_BIT_MASK;
//         *value = *value >> GT911_P5_XH_TP_BIT_POSITION;
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Get GT911 point Y Low Touch Position
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_YL_REG register
  * @retval Component status
  */
// static void gt911_read_point_y_low_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_YL_REG, value, 1);
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_YL_REG, value, 1);
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_YL_REG, value, 1);
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_YL_REG, value, 1);
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_YL_REG, value, 1);
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Get GT911 point Y High Touch Position
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_YH_REG register
  * @retval Component status
  */
// static void gt911_read_point_y_high_pos(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_YH_REG, value, 1U);
//         *value &= GT911_P1_YH_TP_BIT_MASK;
//         *value = *value >> GT911_P1_YH_TP_BIT_POSITION;
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_YH_REG, value, 1U);
//         *value &= GT911_P2_YH_TP_BIT_MASK;
//         *value = *value >> GT911_P2_YH_TP_BIT_POSITION;
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_YH_REG, value, 1U);
//         *value &= GT911_P3_YH_TP_BIT_MASK;
//         *value = *value >> GT911_P3_YH_TP_BIT_POSITION;
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_YH_REG, value, 1U);
//         *value &= GT911_P4_YH_TP_BIT_MASK;
//         *value = *value >> GT911_P4_YH_TP_BIT_POSITION;
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_YH_REG, value, 1U);
//         *value &= GT911_P5_YH_TP_BIT_MASK;
//         *value = *value >> GT911_P5_YH_TP_BIT_POSITION;
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Get GT911 point Touch pressure
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_WEIGHTL_REG register
  * @retval Component status
  */
// static void gt911_read_point_weight(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_WEIGHTL_REG, value, 2U);
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_WEIGHTL_REG, value, 2U);
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_WEIGHTL_REG, value, 2U);
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_WEIGHTL_REG, value, 2U);
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_WEIGHTL_REG, value, 2U);
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Get GT911 point track ID
  * @param  self Pointer to driver instance
  * @param  point_idx Point index
  * @param  value pointer to the pValue of GT911_P1_TID_REG register
  * @retval Component status
  */
// static void gt911_read_point_track_id(gt911_driver_t * self, uint8_t point_idx, uint8_t * value)
// {
//     switch (point_idx)
//     {
//     case 1:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_TID_REG, value, 1U);
//         *value &= GT911_P1_TID_BIT_MASK;
//         *value = *value >> GT911_P1_TID_BIT_POSITION;
//         break;

//     case 2:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P2_TID_REG, value, 1U);
//         *value &= GT911_P2_TID_BIT_MASK;
//         *value = *value >> GT911_P2_TID_BIT_POSITION;
//         break;

//     case 3:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P3_TID_REG, value, 1U);
//         *value &= GT911_P3_TID_BIT_MASK;
//         *value = *value >> GT911_P3_TID_BIT_POSITION;
//         break;

//     case 4:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P4_TID_REG, value, 1U);
//         *value &= GT911_P4_TID_BIT_MASK;
//         *value = *value >> GT911_P4_TID_BIT_POSITION;
//         break;

//     case 5:
//         gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P5_TID_REG, value, 1U);
//         *value &= GT911_P5_TID_BIT_MASK;
//         *value = *value >> GT911_P5_TID_BIT_POSITION;
//         break;

//     default:
//         break;
//     }
// }

/**
  * @brief  Set GT911 Threshold for touch detection
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_TH_GROUP_REG register
  * @retval Component status
  */
// static void gt911_set_touch_threshold(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TH_GROUP_REG, &value, 1U);
// }

/**
  * @brief  Set GT911 Filter function coefficient
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_TH_DIFF_REG register
  * @retval Component status
  */
// static void gt911_set_filter_coeff(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TH_DIFF_REG, &value, 1);
// }

/**
  * @brief  Control the Switch between Active and Monitoring Mode
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_CTRL_REG register
  * @retval Component status
  */
// static void gt911_ctrl(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_CTRL_REG, &value, 1);
// }

/**
  * @brief  Set the time period of switching from Active mode to Monitor
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_TIMEENTERMONITOR_REG register
  * @retval Component status
  */
// static void gt911_time_enter_monitor(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TIMEENTERMONITOR_REG, &value, 1);
// }

/**
  * @brief  Set rate in Active mode
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_PERIODACTIVE_REG register
  * @retval Component status
  */
// static void gt911_period_active(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_PERIODACTIVE_REG, &value, 1);
// }

/**
  * @brief  Set rate in Monitor mode
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_PERIODMONITOR_REG register
  * @retval Component status
  */
// static void gt911_period_monitor(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_PERIODMONITOR_REG, &value, 1);
// }

/**
  * @brief  Set Minimum distance while Moving Left and Moving Right gesture
  * @param  self Pointer to driver instance
  * @param  value Value to write to GT911_DIS_GESTURE_REG register
  * @retval Component status
  */
static void gt911_distance_left_right(gt911_driver_t * self, uint8_t value)
{
    uint8_t r_data;
    uint8_t tmp_value;

    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_DIS_GESTURE_REG, &r_data, 1);

    tmp_value = ((r_data & GT911_DISTANCE_UD_BIT_MASK) | (value & GT911_DISTANCE_LR_BIT_MASK));

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_DIS_GESTURE_REG, &tmp_value, 1);
}

/**
  * @brief  Set Minimum distance while Moving Up and Moving Down gesture
  * @param  pCtx Pointer to component context
  * @param  value Value to write to GT911_DIS_GESTURE_REG register
  * @retval Component status
  */
static void gt911_distance_up_down(gt911_driver_t * self, uint8_t value)
{
    uint8_t r_data;
    uint8_t tmp_value;

    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_DIS_GESTURE_REG, &r_data, 1U);

    tmp_value = ((value & GT911_DISTANCE_UD_BIT_MASK) | (r_data & GT911_DISTANCE_LR_BIT_MASK));

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_DIS_GESTURE_REG, &tmp_value, 1U);

}

/**
  * @brief  Read High 8-bit of LIB Version info
  * @param  pCtx Pointer to component context
  * @param  pValue Pointer to GT911_LIB_VER_H_REG register pValue
  * @retval Component status
  */
// static void gt911_read_lib_ver_high(gt911_driver_t * self, uint8_t * value)
// {
//     gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_LIB_VER_H_REG, value, 1U);
// }

/**
  * @brief  Read Low 8-bit of LIB Version info
  * @param  pCtx Pointer to component context
  * @param  pValue Pointer to GT911_LIB_VER_L_REG register pValue
  * @retval Component status
  */
// static void gt911_read_lib_ver_low(gt911_driver_t * self, uint8_t * value)
// {
//     gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_LIB_VER_L_REG, value, 1U);
// }

/**
  * @brief  Read status of cipher register
  * @param  pCtx Pointer to component context
  * @param  pValue Pointer to GT911_CIPHER_REG register pValue
  * @retval Component status
  */
// static void gt911_cipher(gt911_driver_t * self, uint8_t * value)
// {
//     gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_CIPHER_REG, value, 1U);
// }

/**
  * @brief  Select Interrupt (polling or trigger) mode
  * @param  pCtx Pointer to component context
  * @param  value Value to write to GT911_MSW1_REG register
  * @retval Component status
  */
// static void gt911_m_sw1(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_MSW1_REG, &value, 1U);
// }

/**
  * @brief  Select Current power mode
  * @param  pCtx Pointer to component context
  * @param  value Value to write to GT911_PWR_MODE_REG register
  * @retval Component status
  */
// static void gt911_pwr_mode(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_PWR_MODE_REG, &value, 1U);
// }

/**
  * @brief  Read the Firmware Version
  * @param  pCtx Pointer to component context
  * @param  pValue Pointer to GT911_FIRMID_REG register pValue
  * @retval Component status
  */
static void gt911_read_firmware_version(gt911_driver_t * self, uint8_t * value)
{
    /* GT911 Firmware version: low byte, high byte */
    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_FIRMID_REG, value, 2U);
}

/**
  * @brief  Read the Goodix's GT911 ID (4-byte)
  * @param  pCtx Pointer to component context
  * @param  pValue Pointer to GT911_CHIP_ID_REG register pValue
  * @retval Component status
  */
static void gt911_read_product_id(gt911_driver_t * self, uint8_t * value)
{
    /* 4-byte Product ID (ASCII, "911"0x00) */
    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_CHIP_ID_REG, value, 4U);
}

/**
  * @brief  Read the Release code version
  * @param  pCtx Pointer to component context
  * @param  pValue Pointer to GT911_RELEASE_CODE_ID_REG register pValue
  * @retval Component status
  */
// static void gt911_read_release_code_version(gt911_driver_t * self, uint8_t * value)
// {
//     gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_RELEASE_CODE_ID_REG, value, 1U);
// }

/**
  * @brief  Select Current Operating mode
  * @param  pCtx Pointer to component context
  * @param  value Value to write to GT911_COMMAND_REG register
  * @retval Component status
  */
// static void gt911_mode(gt911_driver_t * self, uint8_t value)
// {
//     gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_COMMAND_REG, &value, 1U);
// }

/**
  * @brief  Configure the GT911 gesture
  * @param  self Pointer to driver instance
  * @param  init Gesture init structure pointer
  * @retval Component status
  */
static void gt911_config_gesture(gt911_driver_t * self, gt911_gesture_init_t * init)
{
    uint8_t mode          = GT911_GESTURE_EN;
    uint8_t switch1_value = GT911_GESTURE_SWITCH1_VAL;
    uint8_t switch2_value = GT911_GESTURE_SWITCH2_VAL;
    uint8_t press_time    = GT911_GESTURE_TIME_ABORT;
    uint8_t adjust_value  = GT911_GESTURE_ADJUST_VAL;
    uint8_t control       = (GT911_GESTURE_INVALID_TIM & init->gain);

    gt911_distance_up_down(self, (uint8_t)init->distance_up_down);

    gt911_distance_left_right(self, (uint8_t)init->distance_left_right);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_PRESS_TIME, &press_time, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_SLOPE_ADJUST, &adjust_value, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_CTRL_REG, &control, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_SWITCH1_REG, &switch1_value, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_SWITCH2_REG, &switch2_value, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_REFRESH_REG, &init->refresh_rate, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_GESTURE_TH_REG, &init->gesture_threshold, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_COMMAND_CHK_REG, &mode, 1);

    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_COMMAND_REG, &mode, 1);
}

/**
  * @brief Start TouchScreen calibration phase
  * @param  self Pointer to driver instance
  * @retval Status GT911_OK or GT911_ERROR.
  */
static void gt911_touch_calibration(gt911_driver_t * self)
{
    uint32_t nbr_attempt;
    uint8_t read_data;
    uint8_t end_calibration = 0U;

    /* Switch GT911 back to factory mode to calibrate */
    gt911_set_working_mode(self, GT911_DEV_MODE_FACTORY);

    /* Read back the same register GT911_DEV_MODE_REG */
    gt911_get_working_mode(self, &read_data);

    /* Wait 300 ms */
    self->handle->timebase->pf_delay_ms(300);

    if (read_data == GT911_DEV_MODE_FACTORY) {

        /* Start calibration command */
        read_data = 0x04U;
        gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TD_STAT_REG, &read_data, 1U);

        /* Wait 300 ms */
        self->handle->timebase->pf_delay_ms(300);

        /* 100 attempts to wait switch from factory mode (calibration) to working mode */
        for (nbr_attempt = 0; ((nbr_attempt < 100U) && (end_calibration == 0U)) ; nbr_attempt++) {

            gt911_get_working_mode(self, &read_data);

            if (read_data == GT911_DEV_MODE_WORKING) {

            /* Auto Switch to GT911_DEV_MODE_WORKING : means calibration have ended */
            end_calibration = 1U; /* exit for loop */
            }

            /* Wait 300 ms */
            self->handle->timebase->pf_delay_ms(300);
        }
    }

}

/**
  * @brief  Return if there is touches detected or not.
  *         Try to detect new touches and forget the old ones (reset internal global
  *         variables).
  * @param  self Pointer to driver instance
  * @retval Number of active touches detected (can be 0, 1~5) or GT911_ERROR
  *         in case of error
  */
static uint8_t gt911_read_touch_nums(gt911_driver_t * self)
{
    uint8_t touch_nums = 0;

    gt911_read_touch_data_status(self, &touch_nums);

    if(touch_nums > GT911_TOUCH_MAX) {
        return 0;
    }
    else {
        return touch_nums;
    }
}

/**
  * @brief  Get the touch screen X and Y positions values
  * @param  self Pointer to driver instance
  * @param  touchpad Single Touch structure pointer
  * @retval Component status
  */
static void gt911_read_touch_coord(gt911_driver_t * self, gt911_touch_t * touchpad)
{
    uint8_t touch_nums = 0U;
    uint8_t touch_data[6] = {0};
    uint8_t value = 0U;

    touch_nums = gt911_read_touch_nums(self);

    if ((touch_nums > 0) && (touch_nums <= GT911_TOUCH_MAX)) {
        touchpad->is_pressed = true;
    }
    else {
        touchpad->is_pressed = false;
        return;
    }

    gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_XL_REG, touch_data, (uint16_t)sizeof(touch_data));

    /* Send back first ready X position to caller */
    touchpad->coord_x = (((uint32_t)touch_data[GT911_P1_XH_TP_BIT_POSITION] & GT911_P1_XH_TP_BIT_MASK) << 8U) | ((uint32_t)touch_data[GT911_P1_XL_TP_BIT_POSITION] & GT911_P1_XL_TP_BIT_MASK);
    /* Send back first ready Y position to caller */
    touchpad->coord_y = (((uint32_t)touch_data[GT911_P1_YH_TP_BIT_POSITION] & GT911_P1_YH_TP_BIT_MASK) << 8U) | ((uint32_t)touch_data[GT911_P1_YL_TP_BIT_POSITION] & GT911_P1_YL_TP_BIT_MASK);
    /* Acknowledge the reading of the Touch coordinates */
    gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TD_STAT_REG, &value, 1U);

}

/**
  * @brief  Get the touch screen Xn and Yn positions values in multi-touch mode
  * @param  self Pointer to driver instance
  * @param  touchpad Multi Touch structure pointer
  * @retval Component status
  */
static void gt911_read_multi_touch_coord(gt911_driver_t * self, gt911_multi_touch_t * touchpad)
{
    uint8_t touch_nums = 0;
    uint8_t touch_data[40] = {0};
    uint8_t value = 0U;
    uint32_t idx = 0;

    touch_nums = gt911_read_touch_nums(self);

    if ((touch_nums > 0) && (touch_nums <= GT911_TOUCH_MAX)) {

        touchpad->touch_nums = touch_nums;

        gt911_read_reg(self, GT911_I2C_ADDRESS, GT911_P1_XL_REG, touch_data, (uint16_t)sizeof(touch_data));

        for (idx = 0; idx < touch_nums; idx++) {
            /* Send back first ready X position to caller */
            touchpad->coord_x[idx]          = (((uint32_t)touch_data[(idx * 8U) + GT911_P1_XH_TP_BIT_POSITION] & GT911_P1_XH_TP_BIT_MASK) << 8U) | ((uint32_t)touch_data[(idx * 8U) + GT911_P1_XL_TP_BIT_POSITION] & GT911_P1_XL_TP_BIT_MASK);
            /* Send back first ready Y position to caller */
            touchpad->coord_y[idx]          = (((uint32_t)touch_data[(idx * 8U) + GT911_P1_YH_TP_BIT_POSITION] & GT911_P1_YH_TP_BIT_MASK) << 8U) | ((uint32_t)touch_data[(idx * 8U) + GT911_P1_YL_TP_BIT_POSITION] & GT911_P1_YL_TP_BIT_MASK);
            /* Send back first ready Weight to caller */
            touchpad->touch_weight[idx]     = ((uint32_t)touch_data[(idx * 8U) + GT911_P1_WEIGHT_BIT_POSITION] & GT911_P1_WEIGHT_BIT_MASK);
            /* Send back first ready Area to caller */
            touchpad->touch_track_id[idx]   = ((uint32_t)touch_data[(idx * 8U) + GT911_P1_TID_BIT_POSITION] & GT911_P1_TID_BIT_MASK) >> GT911_P1_TID_BIT_POSITION;
        }

        for (idx = touch_nums; idx < GT911_TOUCH_MAX; idx++) {
            /* set 0 for non-touch point(s) */
            touchpad->coord_x[idx]          = 0U;  
            touchpad->coord_y[idx]          = 0U;
            touchpad->touch_weight[idx]     = 0U;
            touchpad->touch_track_id[idx]   = 0U;
        }

        /* Acknowledge the reading of the Touch coordinates */
        gt911_write_reg(self, GT911_I2C_ADDRESS, GT911_TD_STAT_REG, &value, 1U);
    }
}


#if 0
static bool gt911_init(gt911_driver_t * self)
{
    ASSERT_NULL(self->handle->i2c);
    ASSERT_NULL(self->handle->ctrl);
    ASSERT_NULL(self->handle->timebase);

    ASSERT_NULL(self->handle->ctrl->pf_write_int_pin);
    ASSERT_NULL(self->handle->ctrl->pf_write_rst_pin);
    ASSERT_NULL(self->handle->ctrl->pf_set_int_pin);
    ASSERT_NULL(self->handle->timebase->pf_delay_ms);

    if(self->handle->i2c->type == GT911_I2C_HARDWARE) {

        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_init);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_start);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_stop);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_send_addr);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_read_byte);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_write_byte);
        ASSERT_NULL(self->handle->i2c->connect.hi2c.pf_ack_en);

        const struct gt911_hi2c * i2c = &self->handle->i2c->connect.hi2c;
        i2c->pf_init();
    }
    else if(self->handle->i2c->type == GT911_I2C_SOFTWARE) {

        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_init);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_start);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_stop);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_wait_ack);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_generate_ack);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_generate_nack);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_read_byte);
        ASSERT_NULL(self->handle->i2c->connect.si2c.pf_write_byte);

        const struct gt911_si2c * i2c = &self->handle->i2c->connect.si2c; 
        i2c->pf_init();
    }

    uint8_t info[5] = {0};

    gt911_reset(self);

    gt911_read_reg(self, GT911_DEV_ADD, GT911_ID_REG, info, 3);

    if((info[0] == '9') &&
       (info[1] == '1') &&
       (info[2] == '1')) {

        return true;
    }
    else {
        return false;
    }
}

static void gt911_reset(gt911_driver_t * self)
{
    self->handle->ctrl->pf_set_int_pin(GT911_PIN_OUTPUT);
    self->handle->ctrl->pf_write_int_pin(GT911_PIN_LOW);
    self->handle->ctrl->pf_write_rst_pin(GT911_PIN_HIGH);
    self->handle->timebase->pf_delay_ms(10);

    self->handle->ctrl->pf_write_rst_pin(GT911_PIN_LOW);
    self->handle->timebase->pf_delay_ms(25);
    self->handle->ctrl->pf_write_rst_pin(GT911_PIN_HIGH);
    self->handle->timebase->pf_delay_ms(120);

    self->handle->ctrl->pf_set_int_pin(GT911_PIN_INPUT);
    self->handle->timebase->pf_delay_ms(30);
}

static void gt911_get_id(gt911_driver_t * self, uint8_t * id)
{
    gt911_read_reg(self, GT911_DEV_ADD, GT911_ID_REG, id, 1);
}

static void gt911_get_resolution(gt911_driver_t * self, uint16_t * width, uint16_t * height)
{
    uint8_t data[4] = {0};

    gt911_read_reg(self, GT911_DEV_ADD, GT911_RES_REG, data, 4);

    *width = (data[1] << 8) | data[0];
    *height = (data[3] << 8) | data[2];
}

static void gt911_get_firmware_version(gt911_driver_t * self, uint8_t * version)
{
    gt911_read_reg(self, GT911_DEV_ADD, GT911_FIRMWARE_REG, version, 4);
}

static void gt911_scan(gt911_driver_t * self)
{
    uint8_t i = 0;
    uint8_t touch_data[2 + 8 * GT911_TOUCH_MAX] = {0};
    uint8_t write_data[2] = {0};

    gt911_read_reg(self, GT911_DEV_ADD, GT911_COORD_REG, touch_data, sizeof(touch_data));
    gt911_write_reg(self, GT911_DEV_ADD, GT911_CLEAR_REG, write_data, 1);
    touchpad.touch_num = touch_data[0] & 0x0F;

    if((touchpad.touch_num >= 1) && (touchpad.touch_num <= GT911_TOUCH_MAX)) {
        touchpad.is_pressed = true;

        for(i = 0; i < touchpad.touch_num; i++) {
            uint8_t * coord = &touch_data[2 + i * 8];
            touchpad.x[i] = (coord[1] << 8) | coord[0];
            touchpad.y[i] = (coord[3] << 8) | coord[2];
        }

        for(i = touchpad.touch_num; i < GT911_TOUCH_MAX; i++) {
            touchpad.x[i] = 0;
            touchpad.y[i] = 0;
        }

    } 
    else {
        touchpad.is_pressed = false;
    }
}

static bool gt911_is_pressed(gt911_driver_t * self)
{
    return touchpad.is_pressed;
}

static void gt911_get_coordinates(gt911_driver_t * self, uint16_t * x, uint16_t * y, uint8_t num)
{
    if(self == NULL || x == NULL || y == NULL) {
        return;
    }

    for(uint8_t i = 0; i < num; i++) {
        x[i] = touchpad.x[i];
        y[i] = touchpad.y[i];
    }
}
#endif




/******************************* (END OF FILE) *********************************/

