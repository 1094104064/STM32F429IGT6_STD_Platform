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
static void st7735_write_data(struct st7735_driver * self, uint8_t byte);
static void st7735_write_command(struct st7735_driver * self, uint8_t cmd);
static bool st7735_init(struct st7735_driver * self);
static void st7735_set_cursor(struct st7735_driver * self, uint16_t x, uint16_t y);
static void st7735_set_window(struct st7735_driver * self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void st7735_put_pixel(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t color);
static void st7735_fill_rect(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
static void st7735_fill_screen(struct st7735_driver * self, uint16_t color);
static void st7735_copy_buffer(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data);
static void st7735_backlight_on(struct st7735_driver * self);
static void st7735_backlight_off(struct st7735_driver * self);
static void st7735_backlight_set(struct st7735_driver * self, uint8_t brightness);
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
void bsp_driver_st7735_link(    struct st7735_driver * self, 
                                struct st7735_oper * oper,
                                struct st7735_oper_info * info,
                                struct st7735_oper_spi * spi,
                                struct st7735_oper_ctrl * ctrl,
                                struct st7735_oper_backlight * backlight)
{
    st7735_assert_null(self);
    st7735_assert_null(oper);
    st7735_assert_null(info);
    st7735_assert_null(spi);
    st7735_assert_null(ctrl);
    st7735_assert_null(backlight);

    if(self == NULL || oper == NULL || info == NULL || spi == NULL || ctrl == NULL || backlight == NULL)
        return;

    self->oper                  = oper;
    self->oper->oper_info       = info;
    self->oper->oper_spi        = spi;
    self->oper->oper_ctrl       = ctrl;
    self->oper->oper_backlight  = backlight;


    self->pf_write_data         = st7735_write_data;
    self->pf_write_command      = st7735_write_command;
    self->pf_init               = st7735_init;
    self->pf_set_cursor         = st7735_set_cursor;
    self->pf_set_window         = st7735_set_window;
    self->pf_put_pixel          = st7735_put_pixel;
    self->pf_fill_rect          = st7735_fill_rect;
    self->pf_fill_screen        = st7735_fill_screen;
    self->pf_copy_buffer        = st7735_copy_buffer;
    self->pf_backlight_on       = st7735_backlight_on;
    self->pf_backlight_off      = st7735_backlight_off;
    self->pf_backlight_set      = st7735_backlight_set;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void st7735_write_data(struct st7735_driver * self, uint8_t byte)
{
    self->oper->oper_spi->pf_transmit_8bit(byte);
}

static void st7735_write_command(struct st7735_driver * self, uint8_t cmd)
{
    self->oper->oper_ctrl->pf_dc_low();
    st7735_write_data(self, cmd);
    self->oper->oper_ctrl->pf_dc_high();
}


static bool st7735_init(struct st7735_driver * self)
{
    st7735_assert_null(self->oper->pf_delay_ms);
    st7735_assert_null(self->oper->oper_ctrl->pf_cs_high);
    st7735_assert_null(self->oper->oper_ctrl->pf_cs_low);
    st7735_assert_null(self->oper->oper_ctrl->pf_dc_high);
    st7735_assert_null(self->oper->oper_ctrl->pf_dc_low);
    st7735_assert_null(self->oper->oper_ctrl->pf_rst_high);
    st7735_assert_null(self->oper->oper_ctrl->pf_rst_low);
    st7735_assert_null(self->oper->oper_spi->pf_transmit_8bit);
    st7735_assert_null(self->oper->oper_spi->pf_transmit_16bit);
    st7735_assert_null(self->oper->oper_spi->pf_transmit_dma_16bit);
    st7735_assert_null(self->oper->oper_backlight->pf_on);
    st7735_assert_null(self->oper->oper_backlight->pf_off);
    st7735_assert_null(self->oper->oper_backlight->pf_set);

    if(self->oper->pf_delay_ms                      == NULL ||
       self->oper->oper_ctrl->pf_cs_high            == NULL ||
       self->oper->oper_ctrl->pf_cs_low             == NULL ||
       self->oper->oper_ctrl->pf_dc_high            == NULL ||
       self->oper->oper_ctrl->pf_dc_low             == NULL ||
       self->oper->oper_ctrl->pf_rst_high           == NULL ||
       self->oper->oper_ctrl->pf_rst_low            == NULL ||
       self->oper->oper_spi->pf_transmit_8bit       == NULL || 
       self->oper->oper_spi->pf_transmit_16bit      == NULL ||
       self->oper->oper_spi->pf_transmit_dma_16bit  == NULL ||
       self->oper->oper_backlight->pf_on            == NULL ||
       self->oper->oper_backlight->pf_off           == NULL ||
       self->oper->oper_backlight->pf_set           == NULL) {
        return false;
    }


    uint32_t i = 0;

    self->oper->oper_ctrl->pf_rst_low();
    self->oper->pf_delay_ms(10);
    self->oper->oper_ctrl->pf_rst_high();
    self->oper->pf_delay_ms(120);

    self->oper->oper_ctrl->pf_cs_low();

    st7735_write_command(self, 0x01); // Software reset
    self->oper->pf_delay_ms(150);

    st7735_write_command(self, 0x11); // Sleep out
    self->oper->pf_delay_ms(500);

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
    if(self->oper->oper_info->width > self->oper->oper_info->height) {
        if(self->oper->oper_info->rotated) {
            st7735_write_data(self, ST7735_ORIENTATION_LANDSCAPE_ROT180);
        }
        else {
            st7735_write_data(self, ST7735_ORIENTATION_LANDSCAPE);
        }
    }
    else {
        if(self->oper->oper_info->rotated) {
            st7735_write_data(self, ST7735_ORIENTATION_PORTRAIT_ROT180);
        }
        else {
            st7735_write_data(self, ST7735_ORIENTATION_PORTRAIT);
        }
    }


    st7735_write_command(self, ST7735_DISPLAY_ON); // Display on
    self->oper->pf_delay_ms(100);

    self->oper->oper_ctrl->pf_cs_high();

    st7735_dbg("st7735 init successfully");

    return true;
}

static void st7735_set_cursor(struct st7735_driver * self, uint16_t x, uint16_t y)
{
    self->oper->oper_ctrl->pf_cs_low();

    st7735_write_command(self, ST7735_CASET);
    st7735_write_data(self, x >> 8);
    st7735_write_data(self, x & 0xFF);

    st7735_write_command(self, ST7735_RASET);
    st7735_write_data(self, y >> 8);
    st7735_write_data(self, y & 0xFF);

    st7735_write_command(self, ST7735_WRITE_RAM);

    self->oper->oper_ctrl->pf_cs_high();
}

static void st7735_set_window(struct st7735_driver * self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    self->oper->oper_ctrl->pf_cs_low();

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

    self->oper->oper_ctrl->pf_cs_high();
}

static void st7735_put_pixel(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t color)
{
    st7735_set_cursor(self, x, y);

    self->oper->oper_ctrl->pf_cs_low();

    st7735_write_data(self, color >> 8);
    st7735_write_data(self, color & 0xFF);

    self->oper->oper_ctrl->pf_cs_high();
}

static void st7735_fill_rect(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint32_t i = 0;
    uint32_t size = width * height;

    st7735_set_window(self, x, y, x + width - 1, y + height - 1);

    self->oper->oper_ctrl->pf_cs_low();

    if(self->oper->oper_spi->pf_transmit_16bit) {
        self->oper->oper_spi->pf_transmit_16bit(color, size);
    } 
    else {
        for(i = 0; i < size; i++) {
            st7735_write_data(self, color >> 8);
            st7735_write_data(self, color & 0xFF);
        }
    }

    self->oper->oper_ctrl->pf_cs_high();
}

static void st7735_fill_screen(struct st7735_driver * self, uint16_t color)
{
    st7735_fill_rect(self, 0, 0, self->oper->oper_info->width - 1, self->oper->oper_info->height - 1, color);
}

static void st7735_copy_buffer(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data)
{
    st7735_set_window(self, x, y, x + width - 1, y + height - 1);

    self->oper->oper_ctrl->pf_cs_low();

    self->oper->oper_spi->pf_transmit_dma_16bit(data, width * height);

    self->oper->oper_ctrl->pf_cs_high();
}

static void st7735_backlight_on(struct st7735_driver * self)
{
    self->oper->oper_backlight->pf_on();
}

static void st7735_backlight_off(struct st7735_driver * self)
{
    self->oper->oper_backlight->pf_off();
}

static void st7735_backlight_set(struct st7735_driver * self, uint8_t brightness)
{
    self->oper->oper_backlight->pf_set(brightness);
}


/******************************* (END OF FILE) *********************************/

