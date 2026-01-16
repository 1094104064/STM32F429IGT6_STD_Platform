#include "ud_i2c_bit.h"

#define SET_SDA(ops, val)   ops->set_sda(ops->data, val)
#define SET_SCL(ops, val)   ops->set_scl(ops->data, val)
#define GET_SDA(ops)        ops->get_sda(ops->data)
#define GET_SCL(ops)        ops->get_scl(ops->data)

#define SDA_L(ops)          SET_SDA(ops, 0)
#define SDA_H(ops)          SET_SDA(ops, 1)
#define SCL_L(ops)          SET_SCL(ops, 0)

static int8_t SCL_H(const struct ud_i2c_bit_ops * ops)
{
    SET_SCL(ops, 1);

    for(uint32_t t = 0; t < ops->timeout_ms; t++) {
        if(GET_SCL(ops)) {
            return 0;
        }
        ops->delay_us(1000);
    }

    return -1;
}

static void i2c_delay(struct ud_i2c_bit * i2c)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;

    ops->delay_us(2);
}

void i2c_start(struct ud_i2c_bit * i2c)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;

    SDA_H(ops);
    SCL_H(ops);
    i2c_delay(i2c);

    SDA_L(ops);
    i2c_delay(i2c);

    SCL_L(ops);
    i2c_delay(i2c);
}

void i2c_stop(struct ud_i2c_bit * i2c)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;

    SDA_L(ops);
    i2c_delay(i2c);

    SCL_H(ops);
    i2c_delay(i2c);

    SDA_H(ops);
    i2c_delay(i2c);
}

uint8_t i2c_wait_ack(struct ud_i2c_bit * i2c)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;
    uint8_t ack;

    SDA_H(ops);
    i2c_delay(i2c);

    if(SCL_H(ops) < 0) {
        return 1;
    }
    i2c_delay(i2c);

    ack = GET_SDA(ops);
    SCL_L(ops);
    i2c_delay(i2c);

    return ack;
}

uint8_t i2c_write_byte(struct ud_i2c_bit * i2c, uint8_t byte)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;

    for(uint8_t i = 0; i < 8; i++) {
        if(byte & 0x80) {
            SDA_H(ops);
        } else {
            SDA_L(ops);
        }
        i2c_delay(i2c);

        if(SCL_H(ops) < 0) {
            return 1;
        }
        i2c_delay(i2c);

        SCL_L(ops);
        i2c_delay(i2c);

        byte <<= 1;
    }

    return 0;
}

uint8_t i2c_read_byte(struct ud_i2c_bit * i2c, uint8_t ack)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;
    uint8_t byte = 0;

    SDA_H(ops);

    for(uint8_t i = 0; i < 8; i++) {
        byte <<= 1;

        if(SCL_H(ops) < 0) {
            return 0;
        }
        i2c_delay(i2c);

        if(GET_SDA(ops)) {
            byte |= 0x01;
        }

        SCL_L(ops);
        i2c_delay(i2c);
    }

    if(ack) {
        SDA_L(ops);
    } else {
        SDA_H(ops);
    }
    i2c_delay(i2c);

    if(SCL_H(ops) < 0) {
        return 0;
    }
    i2c_delay(i2c);

    SCL_L(ops);
    i2c_delay(i2c);

    return byte;
}

uint8_t i2c_send_ack_or_nack(struct ud_i2c_bit * i2c, uint8_t ack)
{
    const struct ud_i2c_bit_ops * ops = i2c->ops;

    if(ack) {
        SDA_L(ops);
    } else {
        SDA_H(ops);
    }
    i2c_delay(i2c);

    if(SCL_H(ops) < 0) {
        return 1;
    }
    i2c_delay(i2c);

    SCL_L(ops);
    i2c_delay(i2c);

    return 0;
}

