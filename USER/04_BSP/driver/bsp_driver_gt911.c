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

/*********************
 *      DEFINES
 *********************/
struct gt911_touch 
{
    bool is_pressed;
    uint8_t touch_num;
    uint16_t x[GT911_TOUCH_MAX];
    uint16_t y[GT911_TOUCH_MAX];
};
/**********************
 *   GLOBAL VARIABLES
 **********************/
static volatile struct gt911_touch touchpad;
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void     gt911_read_reg              (gt911_driver_t * self, uint8_t dev_addr, uint16_t reg_addr, uint8_t * dst, uint16_t len);
static void     gt911_write_reg             (gt911_driver_t * self, uint8_t dev_addr, uint16_t reg_addr, uint8_t * src, uint16_t len);
static bool     gt911_init                  (gt911_driver_t * self);
static void     gt911_reset                 (gt911_driver_t * self);
static void     gt911_get_id                (gt911_driver_t * self, uint8_t * id);
static void     gt911_get_resolution        (gt911_driver_t * self, uint16_t * width, uint16_t * height);
static void     gt911_get_firmware_version  (gt911_driver_t * self, uint8_t * version);
static void     gt911_scan                  (gt911_driver_t * self);
static bool     gt911_is_pressed            (gt911_driver_t * self);
static void     gt911_get_coordinates       (gt911_driver_t * self, uint16_t * x, uint16_t * y, uint8_t num);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_gt911_link(gt911_driver_t * drv, const gt911_handle_t * handle)
{
    drv->handle = handle;

    drv->pf_init                    = gt911_init;
    drv->pf_reset                   = gt911_reset;
    drv->pf_get_id                  = gt911_get_id;
    drv->pf_get_resolution          = gt911_get_resolution;
    drv->pf_get_firmware_version    = gt911_get_firmware_version;
    drv->pf_scan                    = gt911_scan;
    drv->pf_is_pressed              = gt911_is_pressed;
    drv->pf_get_coordinates         = gt911_get_coordinates;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
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

        i2c->pf_write_byte(dev_addr);
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
        i2c->pf_write_byte(dev_addr | 0x01);
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

        i2c->pf_write_byte(dev_addr);
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

static bool gt911_init(gt911_driver_t * self)
{
    uint8_t info[16] = {0};

    gt911_reset(self);

    gt911_read_reg(self, GT911_DEV_ADD, GT911_ID_REG, info, 11);

    if(info[0] == '9') {
        gt911_dbg("gt911 init successfully");
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

/******************************* (END OF FILE) *********************************/

