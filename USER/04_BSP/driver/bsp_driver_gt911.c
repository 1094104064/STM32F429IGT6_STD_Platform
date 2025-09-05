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
struct gt911_information {
    uint8_t is_pressed;
    uint8_t touch_num;
    uint16_t x[GT911_TOUCH_MAX];
    uint16_t y[GT911_TOUCH_MAX];
};
/**********************
 *   GLOBAL VARIABLES
 **********************/
static volatile struct gt911_information gt911_info;
/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool gt911_init(struct gt911_driver * self);
static void gt911_reset(struct gt911_driver * self);
static void gt911_read_id(struct gt911_driver * self, uint8_t * id);
static void gt911_read_resolution(struct gt911_driver * self, uint16_t * width, uint16_t * height);
static void gt911_read_firmware_version(struct gt911_driver * self, uint8_t * version);
static void gt911_scan(struct gt911_driver * self);
static uint8_t gt911_is_pressed(struct gt911_driver * self);
static void gt911_get_coordinates(struct gt911_driver * self, uint16_t * x, uint16_t * y, uint8_t num);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_gt911_link( struct gt911_driver * self, 
                            struct gt911_oper * oper,
                            struct gt911_oper_i2c * oper_i2c,
                            struct gt911_oper_ctrl * oper_ctrl)
{
    if(self == NULL || oper == NULL || oper_i2c == NULL || oper_ctrl == NULL) {
        return;
    }

    self->oper                      = oper;
    self->oper->oper_i2c            = oper_i2c;
    self->oper->oper_ctrl           = oper_ctrl;

    self->pf_init                   = gt911_init;
    self->pf_reset                  = gt911_reset;
    self->pf_read_id                = gt911_read_id;
    self->pf_read_resolution        = gt911_read_resolution;
    self->pf_read_firmware_version  = gt911_read_firmware_version;
    self->pf_scan                   = gt911_scan;
    self->pf_is_pressed             = gt911_is_pressed;
    self->pf_get_coordinates        = gt911_get_coordinates;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static bool gt911_init(struct gt911_driver * self)
{
    if( self->oper->pf_delay_ms             == NULL ||

        self->oper->oper_ctrl->pf_int_high  == NULL || 
        self->oper->oper_ctrl->pf_int_low   == NULL || 
        self->oper->oper_ctrl->pf_int_in    == NULL ||
        self->oper->oper_ctrl->pf_int_out   == NULL ||
        self->oper->oper_ctrl->pf_rst_high  == NULL || 
        self->oper->oper_ctrl->pf_rst_low   == NULL ||

        self->oper->oper_i2c->pf_read_reg   == NULL ||
        self->oper->oper_i2c->pf_write_reg  == NULL) {

        return false;
    }

    uint8_t info[16] = {0};

    gt911_reset(self);

    self->oper->oper_i2c->pf_read_reg(GT911_DEV_ADD, GT911_ID_REG, info, 11);

    if(info[0] == '9') {
        return true;
    }
    else {
        return false;
    }
}


static void gt911_reset(struct gt911_driver * self)
{
    self->oper->oper_ctrl->pf_int_out();
    self->oper->oper_ctrl->pf_int_low();
    self->oper->oper_ctrl->pf_rst_high();
    self->oper->pf_delay_ms(10);

    self->oper->oper_ctrl->pf_rst_low();
    self->oper->pf_delay_ms(25);
    self->oper->oper_ctrl->pf_rst_high();
    self->oper->pf_delay_ms(120);

    self->oper->oper_ctrl->pf_int_in();
    self->oper->pf_delay_ms(30);
}

static void gt911_read_id(struct gt911_driver * self, uint8_t * id)
{
    self->oper->oper_i2c->pf_read_reg(GT911_DEV_ADD, GT911_ID_REG, id, 1);
}

static void gt911_read_resolution(struct gt911_driver * self, uint16_t * width, uint16_t * height)
{
    uint8_t data[4] = {0};

    self->oper->oper_i2c->pf_read_reg(GT911_DEV_ADD, GT911_RES_REG, data, 4);

    *width = (data[1] << 8) | data[0];
    *height = (data[3] << 8) | data[2];
}

static void gt911_read_firmware_version(struct gt911_driver * self, uint8_t * version)
{
    self->oper->oper_i2c->pf_read_reg(GT911_DEV_ADD, GT911_FIRMWARE_REG, version, 4);
}

static void gt911_scan(struct gt911_driver * self)
{
    uint8_t i = 0;
    uint8_t touch_data[2 + 8 * GT911_TOUCH_MAX] = {0};
    uint8_t write_data[2] = {0};

    self->oper->oper_i2c->pf_read_reg(GT911_DEV_ADD, GT911_COORD_REG, touch_data, sizeof(touch_data));
    self->oper->oper_i2c->pf_write_reg(GT911_DEV_ADD, GT911_CLEAR_REG, write_data, 1);
    gt911_info.touch_num = touch_data[0] & 0x0F;

    if((gt911_info.touch_num >= 1) && (gt911_info.touch_num <= GT911_TOUCH_MAX)) {
        gt911_info.is_pressed = 1;

        for(i = 0; i < gt911_info.touch_num; i++) {
            uint8_t * coord = &touch_data[2 + i * 8];
            gt911_info.x[i] = (coord[1] << 8) | coord[0];
            gt911_info.y[i] = (coord[3] << 8) | coord[2];
        }

        for(i = gt911_info.touch_num; i < GT911_TOUCH_MAX; i++) {
            gt911_info.x[i] = 0;
            gt911_info.y[i] = 0;
        }

    } 
    else {
        gt911_info.is_pressed = 0;
    }
}

static uint8_t gt911_is_pressed(struct gt911_driver * self)
{
    return gt911_info.is_pressed;
}

static void gt911_get_coordinates(struct gt911_driver * self, uint16_t * x, uint16_t * y, uint8_t num)
{
    if(self == NULL || x == NULL || y == NULL) {
        return;
    }

    for(uint8_t i = 0; i < num; i++) {
        x[i] = gt911_info.x[i];
        y[i] = gt911_info.y[i];
    }
}

/******************************* (END OF FILE) *********************************/

