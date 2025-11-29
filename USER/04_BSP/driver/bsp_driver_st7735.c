/**
  ******************************************************************************
  *
  * @file    bsp_driver_st7735.c
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
#include "bsp_driver_st7735.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void st7735_write_data       (st7735_driver_t * self, uint8_t byte);
static void st7735_write_command    (st7735_driver_t * self, uint8_t cmd);
static bool st7735_init             (st7735_driver_t * self);
static void st7735_set_cursor       (st7735_driver_t * self, uint16_t x, uint16_t y);
static void st7735_set_window       (st7735_driver_t * self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void st7735_put_pixel        (st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t color);
static void st7735_fill_rect        (st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
static void st7735_fill_screen      (st7735_driver_t * self, uint16_t color);
static void st7735_copy_8bit_buffer (st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t * data);
static void st7735_copy_16bit_buffer(st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data);
static void st7735_backlight_on     (st7735_driver_t * self);
static void st7735_backlight_off    (st7735_driver_t * self);
static void st7735_backlight_set    (st7735_driver_t * self, uint8_t brightness);
static void st7735_rotated_set(st7735_driver_t * self, uint16_t rotated);
/**********************
 *  STATIC VARIABLES
 **********************/
static const uint8_t st7735_frame_rate_ctrl1_params[] = {0x05, 0x3C, 0x3C};
static const uint8_t st7735_frame_rate_ctrl2_params[] = {0x05, 0x3C, 0x3C};
static const uint8_t st7735_frame_rate_ctrl3_params[] = {0x05, 0x3C, 0x3C, 0x05, 0x3C, 0x3C};
static const uint8_t st7735_frame_inversion_ctrl_params[] = {0x03};
static const uint8_t st7735_pwr_ctrl1_params[] = {0x28, 0x08, 0x04};
static const uint8_t st7735_pwr_ctrl2_params[] = {0xC0};
static const uint8_t st7735_pwr_ctrl3_params[] = {0x0D, 0x00};
static const uint8_t st7735_pwr_ctrl4_params[] = {0x8D, 0x2A};
static const uint8_t st7735_pwr_ctrl5_params[] = {0x8D, 0xEE};
static const uint8_t st7735_vcomh_vcoml_ctrl1_params[] = {0x1A};
static const uint8_t st7735_pv_gamma_ctrl_params[] = {0x04, 0x22, 0x07, 0x0A, 0x2E, 0x30, 0x25, 0x2A, 0x28, 0x26, 0x2E, 0x3A, 0x00, 0x01, 0x03, 0x13};
static const uint8_t st7735_nv_gamma_ctrl_params[] = {0x04, 0x16, 0x06, 0x0D, 0x2D, 0x26, 0x23, 0x27, 0x27, 0x25, 0x2D, 0x3B, 0x01, 0x00, 0x04, 0x13};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_driver_st7735_link(st7735_driver_t * drv, const st7735_handle_t * handle)
{
    st7735_assert_null(drv);
    st7735_assert_null(handle);

    if (drv == NULL || handle == NULL) {
        return;
    }

    drv->handle                 = handle;

    drv->pf_write_data         = st7735_write_data;
    drv->pf_write_command      = st7735_write_command;
    drv->pf_init               = st7735_init;
    drv->pf_set_cursor         = st7735_set_cursor;
    drv->pf_set_window         = st7735_set_window;
    drv->pf_put_pixel          = st7735_put_pixel;
    drv->pf_fill_rect          = st7735_fill_rect;
    drv->pf_fill_screen        = st7735_fill_screen;
    drv->pf_copy_buffer        = st7735_copy_16bit_buffer;
    drv->pf_backlight_on       = st7735_backlight_on;
    drv->pf_backlight_off      = st7735_backlight_off;
    drv->pf_backlight_set      = st7735_backlight_set;
    drv->pf_rotated_set        = st7735_rotated_set;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void st7735_write_data(st7735_driver_t * self, uint8_t byte)
{
    self->handle->pf_spi_transmit_8bit(byte);
}

static void st7735_write_command(st7735_driver_t * self, uint8_t cmd)
{
    self->handle->pf_dc_low();
    st7735_write_data(self, cmd);
    self->handle->pf_dc_high();
}

static bool st7735_init(st7735_driver_t * self)
{
    st7735_assert_null(self->handle->pf_delay_ms);
    st7735_assert_null(self->handle->pf_cs_high);
    st7735_assert_null(self->handle->pf_cs_low);
    st7735_assert_null(self->handle->pf_dc_high);
    st7735_assert_null(self->handle->pf_dc_low);
    st7735_assert_null(self->handle->pf_rst_high);
    st7735_assert_null(self->handle->pf_rst_low);
    st7735_assert_null(self->handle->pf_spi_transmit_8bit);
    st7735_assert_null(self->handle->pf_spi_transmit_16bit);
    st7735_assert_null(self->handle->pf_spi_dma_transmit_16bit);
    st7735_assert_null(self->handle->pf_backlight_on);
    st7735_assert_null(self->handle->pf_backlight_off);
    st7735_assert_null(self->handle->pf_backlight_set);

    if(self->handle->pf_delay_ms                == NULL ||
       self->handle->pf_cs_high                 == NULL ||
       self->handle->pf_cs_low                  == NULL ||
       self->handle->pf_dc_high                 == NULL ||
       self->handle->pf_dc_low                  == NULL ||
       self->handle->pf_rst_high                == NULL ||
       self->handle->pf_rst_low                 == NULL ||
       self->handle->pf_spi_transmit_8bit       == NULL || 
       self->handle->pf_spi_transmit_16bit      == NULL ||
       self->handle->pf_spi_dma_transmit_16bit  == NULL ||
       self->handle->pf_backlight_on            == NULL ||
       self->handle->pf_backlight_off           == NULL ||
       self->handle->pf_backlight_set           == NULL) {
        return false;
    }


    self->width     = self->handle->pf_get_width();
    self->height    = self->handle->pf_get_height();
    self->rotated   = 0;

    uint32_t i = 0;

    self->handle->pf_cs_high();
    self->handle->pf_delay_ms(10);

    self->handle->pf_rst_high();
    self->handle->pf_delay_ms(120);

    self->handle->pf_cs_low();

    st7735_write_command(self, 0x01); // Software reset
    self->handle->pf_delay_ms(150);

    st7735_write_command(self, 0x11); // Sleep out
    self->handle->pf_delay_ms(500);

    st7735_write_command(self, ST7735_FRAME_RATE_CTRL1);
    for(i = 0; i < sizeof(st7735_frame_rate_ctrl1_params) / sizeof(st7735_frame_rate_ctrl1_params[0]); i++)
        st7735_write_data(self, st7735_frame_rate_ctrl1_params[i]);

    st7735_write_command(self, ST7735_FRAME_RATE_CTRL2);
    for(i = 0; i < sizeof(st7735_frame_rate_ctrl2_params) / sizeof(st7735_frame_rate_ctrl2_params[0]); i++)
        st7735_write_data(self, st7735_frame_rate_ctrl2_params[i]);

    st7735_write_command(self, ST7735_FRAME_RATE_CTRL3);
    for(i = 0; i < sizeof(st7735_frame_rate_ctrl3_params) / sizeof(st7735_frame_rate_ctrl3_params[0]); i++)
        st7735_write_data(self, st7735_frame_rate_ctrl3_params[i]);

    st7735_write_command(self, ST7735_FRAME_INVERSION_CTRL);
    for(i = 0; i < sizeof(st7735_frame_inversion_ctrl_params) / sizeof(st7735_frame_inversion_ctrl_params[0]); i++)
        st7735_write_data(self, st7735_frame_inversion_ctrl_params[i]);

    st7735_write_command(self, ST7735_PWR_CTRL1);
    for(i = 0; i < sizeof(st7735_pwr_ctrl1_params) / sizeof(st7735_pwr_ctrl1_params[0]); i++)
        st7735_write_data(self, st7735_pwr_ctrl1_params[i]);

    st7735_write_command(self, ST7735_PWR_CTRL2);
    for(i = 0; i < sizeof(st7735_pwr_ctrl2_params) / sizeof(st7735_pwr_ctrl2_params[0]); i++)
        st7735_write_data(self, st7735_pwr_ctrl2_params[i]);

    st7735_write_command(self, ST7735_PWR_CTRL3);
    for(i = 0; i < sizeof(st7735_pwr_ctrl3_params) / sizeof(st7735_pwr_ctrl3_params[0]); i++)
        st7735_write_data(self, st7735_pwr_ctrl3_params[i]);

    st7735_write_command(self, ST7735_PWR_CTRL4);
    for(i = 0; i < sizeof(st7735_pwr_ctrl4_params) / sizeof(st7735_pwr_ctrl4_params[0]); i++)
        st7735_write_data(self, st7735_pwr_ctrl4_params[i]);

    st7735_write_command(self, ST7735_PWR_CTRL5);
    for(i = 0; i < sizeof(st7735_pwr_ctrl5_params) / sizeof(st7735_pwr_ctrl5_params[0]); i++)
        st7735_write_data(self, st7735_pwr_ctrl5_params[i]);

    st7735_write_command(self, ST7735_VCOMH_VCOML_CTRL1);
    for(i = 0; i < sizeof(st7735_vcomh_vcoml_ctrl1_params) / sizeof(st7735_vcomh_vcoml_ctrl1_params[0]); i++)
        st7735_write_data(self, st7735_vcomh_vcoml_ctrl1_params[i]);

    st7735_write_command(self, ST7735_COLOR_MODE);
    st7735_write_data(self, ST7735_FORMAT_RBG565);

    st7735_write_command(self, ST7735_PV_GAMMA_CTRL);
    for(i = 0; i < sizeof(st7735_pv_gamma_ctrl_params) / sizeof(st7735_pv_gamma_ctrl_params[0]); i++)
        st7735_write_data(self, st7735_pv_gamma_ctrl_params[i]);

    st7735_write_command(self, ST7735_NV_GAMMA_CTRL);
    for(i = 0; i < sizeof(st7735_nv_gamma_ctrl_params) / sizeof(st7735_nv_gamma_ctrl_params[0]); i++)
        st7735_write_data(self, st7735_nv_gamma_ctrl_params[i]);

    st7735_write_command(self, ST7735_MADCTL);

    if(self->rotated == 0) {
        st7735_write_data(self, ST7735_ORIENTATION_LANDSCAPE);
    }
    else if(self->rotated == 90) {
        st7735_write_data(self, ST7735_ORIENTATION_PORTRAIT);
    }
    else if(self->rotated == 180) {
        st7735_write_data(self, ST7735_ORIENTATION_LANDSCAPE_ROT180);
    }
    else if(self->rotated == 270) {
        st7735_write_data(self, ST7735_ORIENTATION_PORTRAIT_ROT180);
    }


    st7735_write_command(self, ST7735_DISPLAY_ON); // Display on
    self->handle->pf_delay_ms(100);

    self->handle->pf_cs_high();

    st7735_dbg("st7735 init successfully");

    return true;
}

static void st7735_set_cursor(st7735_driver_t * self, uint16_t x, uint16_t y)
{
    self->handle->pf_cs_low();

    st7735_write_command(self, ST7735_CASET);
    st7735_write_data(self, x >> 8);
    st7735_write_data(self, x & 0xFF);

    st7735_write_command(self, ST7735_RASET);
    st7735_write_data(self, y >> 8);
    st7735_write_data(self, y & 0xFF);

    st7735_write_command(self, ST7735_WRITE_RAM);

    self->handle->pf_cs_high();
}

static void st7735_set_window(st7735_driver_t * self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    self->handle->pf_cs_low();

    st7735_write_command(self, ST7735_CASET);
    st7735_write_data(self, x1 >> 8);
    st7735_write_data(self, x1 & 0xFF);
    st7735_write_data(self, x2 >> 8);
    st7735_write_data(self, x2 & 0xFF);

    st7735_write_command(self, ST7735_RASET);
    st7735_write_data(self, y1 >> 8);
    st7735_write_data(self, y1 & 0xFF);
    st7735_write_data(self, y2 >> 8);
    st7735_write_data(self, y2 & 0xFF);

    st7735_write_command(self, ST7735_WRITE_RAM);

    self->handle->pf_cs_high();
}

static void st7735_put_pixel(st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t color)
{
    st7735_set_cursor(self, x, y);

    self->handle->pf_cs_low();

    st7735_write_data(self, color >> 8);
    st7735_write_data(self, color & 0xFF);

    self->handle->pf_cs_high();
}

static void st7735_fill_rect(st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint32_t i = 0;
    uint32_t size = width * height;

    st7735_set_window(self, x, y, x + width - 1, y + height - 1);

    self->handle->pf_cs_low();

    if(self->handle->pf_spi_transmit_16bit) {
        self->handle->pf_spi_transmit_16bit(color, size);
    } 
    else {
        for(i = 0; i < size; i++) {
            st7735_write_data(self, color >> 8);
            st7735_write_data(self, color & 0xFF);
        }
    }

    self->handle->pf_cs_high();
}

static void st7735_fill_screen(st7735_driver_t * self, uint16_t color)
{
    st7735_fill_rect(self, 0, 0, self->width - 1, self->height - 1, color);
}

static void st7735_copy_8bit_buffer(st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t * data)
{
    st7735_set_window(self, x, y, x + width - 1, y + height - 1);

    self->handle->pf_cs_low();

    self->handle->pf_spi_dma_transmit_8bit(data, width * height * 2);

    self->handle->pf_cs_high();
}

static void st7735_copy_16bit_buffer(st7735_driver_t * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data)
{
    st7735_set_window(self, x, y, x + width - 1, y + height - 1);

    self->handle->pf_cs_low();

    self->handle->pf_spi_dma_transmit_16bit(data, width * height);

    self->handle->pf_cs_high();
}

static void st7735_backlight_on(st7735_driver_t * self)
{
    self->handle->pf_backlight_on();
}

static void st7735_backlight_off(st7735_driver_t * self)
{
    self->handle->pf_backlight_off();
}

static void st7735_backlight_set(st7735_driver_t * self, uint8_t brightness)
{
    self->handle->pf_backlight_set(brightness);
}

static void st7735_rotated_set(st7735_driver_t * self, uint16_t rotated)
{
    self->rotated = rotated;
}

/******************************* (END OF FILE) *********************************/

