
#include "./ud_gt9xx.h"


void ud_gt9xx_prepare()
{

}

static void gt9xx_reset(const struct ud_touch_ops ** ops)
{
    const struct ud_gt9xx * self = (const struct ud_gt9xx *)ops;

    touchpad_ctrl_reset(self->ctrl_ops);
}

static void gt9xx_read_reg(const struct ud_touch_ops ** ops, uint16_t reg, uint8_t read_len, uint8_t * read_buf)
{
    const struct ud_gt9xx * self = (const struct ud_gt9xx *)ops;

    struct touchpad_i2c_msg msgs[2];
    uint8_t reg_w[2] = {(uint8_t)(reg >> 8), (uint8_t)reg};

    msgs[0].addr  = GT9XX_ADDRESS_HIGH;         /* Slave address */
    msgs[0].flags = TOUCHPAD_I2C_WR;            /* Write flag */
    msgs[0].buf   = reg_w;                      /* Slave register address */
    msgs[0].len   = 2;                          /* Number of bytes sent */

    msgs[1].addr  = GT9XX_ADDRESS_HIGH;         /* Slave address */
    msgs[1].flags = TOUCHPAD_I2C_RD;            /* Read flag */
    msgs[1].buf   = read_buf;                   /* Read data pointer */
    msgs[1].len   = read_len;                   /* Number of bytes read */

    touchpad_i2c_transfer(self->i2c_ops, msgs, 2);
}

static void gt9xx_write_reg(const struct ud_touch_ops ** ops, uint16_t reg, uint8_t write_data)
{
    const struct ud_gt9xx * self = (const struct ud_gt9xx *)ops;

    struct touchpad_i2c_msg msgs;
    uint8_t buf[3] = {(uint8_t)(reg >> 8), (uint8_t)reg, write_data};

    msgs.addr  = GT9XX_ADDRESS_HIGH;        /* slave address */
    msgs.flags = TOUCHPAD_I2C_WR;           /* write flag */
    msgs.buf   = buf;                       /* Send data pointer */
    msgs.len   = 3;

    touchpad_i2c_transfer(self->i2c_ops, &msgs, 1);
}

static void gt9xx_get_product_id(uint32_t * id)
{

}

static void gt9xx_get_info()
{
    
}

static void gt9xx_set_irq_callback(const struct ud_touch_ops ** ops, touchpad_irq_callback_t irq_cb)
{
    const struct ud_gt9xx * self = (const struct ud_gt9xx *)ops;

    touchpad_ctrl_set_callback(self->ctrl_ops, irq_cb);
}

void ud_gt9xx_ioctrl(const touch_ops_t ** ops, unsigned int cmd, void * arg)
{

}

void ud_gt9xx_read_touch_data(const struct ud_touch_ops ** ops, struct ud_touch_point * points)
{
    uint8_t touch_buf[2 + GT9XX_MAX_TOUCH_POINTS * 8] = {0};
    const struct ud_gt9xx * self = (const struct ud_gt9xx *)ops;

    gt9xx_read_reg(ops, GT9XX_READ_STATUS, 1, touch_buf);

    if((touch_buf[0] & 0x80) != 0) {
        uint8_t touch_num = touch_buf[0] & 0x0F;
        if(touch_num > GT9XX_MAX_TOUCH_POINTS)
            touch_num = GT9XX_MAX_TOUCH_POINTS;

        gt9xx_read_reg(ops, GT9XX_POINT1_REG, 8 * touch_num, &touch_buf[2]);

        for(uint8_t i = 0; i < touch_num; i++) {
            points[i].id = touch_buf[2 + i * 8 + 2] >> 4;
            points[i].x  = ((uint16_t)(touch_buf[2 + i * 8 + 3] & 0x0F) << 8) | touch_buf[2 + i * 8 + 2];
            points[i].y  = ((uint16_t)(touch_buf[2 + i * 8 + 5] & 0x0F) << 8) | touch_buf[2 + i * 8 + 4];
            points[i].is_pressed = true;
        }

        for(uint8_t i = touch_num; i < GT9XX_MAX_TOUCH_POINTS; i++) {
            points[i].is_pressed = false;
        }
        
        gt9xx_write_reg(ops, 0x814e, 0); //clear tp interrupt
    }
    else {
        for(uint8_t i = 0; i < GT9XX_MAX_TOUCH_POINTS; i++) {
            points[i].is_pressed = false;
        }
    }
}