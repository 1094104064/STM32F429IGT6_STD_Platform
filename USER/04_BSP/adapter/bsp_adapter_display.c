/**
  ******************************************************************************
  *
  * @file    bsp_adapter_display.c
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
#include "bsp_adapter_display.h"
#include "bsp_wrapper_display.h"
#include "bsp_driver_lcd.h"
#include "bsp_driver_st7735.h"
#include "bsp_port_lcd.h"
#include "bsp_port_st7735.h"
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
static int bsp_adapter_display_init(struct display_wrapper * self);
static void bsp_adapter_backlight_on(struct display_wrapper * self);
static void bsp_adapter_backlight_off(struct display_wrapper * self);
static void bsp_adapter_display_put_pixel(struct display_wrapper * self, uint16_t x, uint16_t y, uint32_t color);
static void bsp_adapter_display_fill_rect(struct display_wrapper * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
static void bsp_adapter_display_fill_screen(struct display_wrapper * self, uint32_t color);
static void bsp_adapter_display_copy_buffer(struct display_wrapper * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
/**********************
 *  STATIC VARIABLES
 **********************/

static const char * const display_lcd = "rgb-lcd";
static const char * const display_st7735 = "st7735";

static struct lcd_driver lcd_drv;
static struct lcd_oper lcd_ops = {
    .pf_layer_config = bsp_port_lcd_layer_config,
    .pf_backlight_on = bsp_port_lcd_backlight_on,
    .pf_backlight_off = bsp_port_lcd_backlight_off,
    .pf_put_pixel = bsp_port_lcd_put_pixel,
    .pf_fast_fill_rect = bsp_port_lcd_fill_rectangle,
    .pf_fast_fill_screen = bsp_port_lcd_fill_screen,
    .pf_copy_buffer = bsp_port_lcd_copy_buffer,
};

struct lcd_layer lcd_fb = {0};

static const struct lcd_panel simple_panel = {
    .name = "simple_panel",

    .active_width = 480,
    .active_height = 272,

    .hsync_width = LCD_DEFAULT_HSW,
    .hfront_porch = LCD_DEFAULT_HFP,
    .hback_porch = LCD_DEFAULT_HBP,
    .vsync_width = LCD_DEFAULT_VSW,
    .vfront_porch = LCD_DEFAULT_VFP,
    .vback_porch = LCD_DEFAULT_VBP,
};

const struct lcd_panel * panels = &simple_panel;

static struct st7735_driver st7735_drv;
static struct st7735_oper st7735_ops = {
    .pf_delay_ms = bsp_port_st7735_delay_ms,
};
static struct st7735_oper_spi st7735_spi = {
    .pf_transmit_8bit = bsp_port_st7735_transmit_8bit,
    .pf_transmit_16bit = bsp_port_st7735_transmit_16bit,
    .pf_transmit_dma_8bit = bsp_port_st7735_transmit_dma_8bit,
    .pf_transmit_dma_16bit = bsp_port_st7735_transmit_dma_16bit,
};
static struct st7735_oper_ctrl st7735_ctrl = {
    .pf_cs_high = bsp_port_st7735_cs_high,
    .pf_cs_low = bsp_port_st7735_cs_low,
    .pf_dc_high = bsp_port_st7735_dc_high,
    .pf_dc_low = bsp_port_st7735_dc_low,
    .pf_rst_high = bsp_port_st7735_rst_high,
    .pf_rst_low = bsp_port_st7735_rst_low,
};
static struct st7735_oper_backlight st7735_backlight = {
    .pf_on = bsp_port_st7735_backlight_on,
    .pf_off = bsp_port_st7735_backlight_off,
};
static struct st7735_oper_info st7735_info = {
    .width = 160,
    .height = 128,
    .rotated = 0,
};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_adapter_display_register(void)
{
#if 0
    struct display_wrapper lcd_wrap = {
        .name               = display_lcd,
        .user_data          = NULL,

        .width              = simple_panel.active_width,
        .height             = simple_panel.active_height,

        .pf_init            = bsp_adapter_display_init,
        .pf_backlight_on    = bsp_adapter_backlight_on,
        .pf_backlight_off   = bsp_adapter_backlight_off,
        .pf_put_pixel       = bsp_adapter_display_put_pixel,
        .pf_fill_rect       = bsp_adapter_display_fill_rect,
        .pf_fill_screen     = bsp_adapter_display_fill_screen,
        .pf_copy_buffer     = bsp_adapter_display_copy_buffer,
    };

    bsp_wrapper_display_link(&lcd_wrap);
#endif

    struct display_wrapper st7735_wrap = {
        .name = display_st7735,
        .user_data = NULL,

        .width = st7735_info.width,
        .height = st7735_info.height,

        .pf_init = bsp_adapter_display_init,
        .pf_backlight_on = bsp_adapter_backlight_on,
        .pf_backlight_off = bsp_adapter_backlight_off,
        .pf_put_pixel = bsp_adapter_display_put_pixel,
        .pf_fill_rect = bsp_adapter_display_fill_rect,
        .pf_fill_screen = bsp_adapter_display_fill_screen,
        .pf_copy_buffer = bsp_adapter_display_copy_buffer,
    };

    bsp_wrapper_display_link(&st7735_wrap);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int bsp_adapter_display_init(struct display_wrapper * self)
{
    if(self->name == display_lcd) {
        bsp_driver_lcd_link(&lcd_drv, &lcd_ops, panels, &lcd_fb);

        if( lcd_drv.pf_init          == NULL || lcd_drv.pf_backlight_on == NULL ||
            lcd_drv.pf_backlight_off == NULL || lcd_drv.pf_put_pixel    == NULL ||
            lcd_drv.pf_fill_rect     == NULL || lcd_drv.pf_fill_screen  == NULL ||
            lcd_drv.pf_copy_buffer   == NULL ) {
            return 1;
        }

        if(lcd_drv.pf_init(&lcd_drv) == false) {
            return 2;
        }

        pr_info("\r\n\tname \t: %s\r\n"
                "\tWidth \t: %d\r\n"
                "\tHeight \t: %d\r\n"
                "\tHSW \t: %d\r\n"
                "\tHFP \t: %d\r\n"
                "\tHBP \t: %d\r\n"
                "\tVSW \t: %d\r\n"
                "\tVFP \t: %d\r\n"
                "\tVBP \t: %d",
                panels->name,
                panels->active_width,
                panels->active_height,
                panels->hsync_width,
                panels->hfront_porch,
                panels->hback_porch,
                panels->vsync_width,
                panels->vfront_porch,
                panels->vback_porch);

    }
    else if(self->name == display_st7735) {
        bsp_driver_st7735_link( &st7735_drv, 
                                &st7735_ops, 
                                &st7735_info, 
                                &st7735_spi, 
                                &st7735_ctrl, 
                                &st7735_backlight   );
        
        if(st7735_drv.pf_init           == NULL || st7735_drv.pf_backlight_on   == NULL ||
           st7735_drv.pf_backlight_off  == NULL || st7735_drv.pf_put_pixel      == NULL ||
           st7735_drv.pf_fill_rect      == NULL || st7735_drv.pf_fill_screen    == NULL ||
           st7735_drv.pf_copy_buffer    == NULL ) {
            return 1;
        }

        if(st7735_drv.pf_init(&st7735_drv) == false) {
            return 2;
        }



    }


    return 0;
}

static void bsp_adapter_backlight_on(struct display_wrapper * self)
{
    if(self->name == display_lcd) {
        lcd_drv.pf_backlight_on(&lcd_drv);
    }
    else if(self->name == display_st7735) {
        st7735_drv.pf_backlight_on(&st7735_drv);
    } else {
        pr_error("Unsupported display type: %s", self->name);
    }
}

static void bsp_adapter_backlight_off(struct display_wrapper * self)
{
    if(self->name == display_lcd) {
        lcd_drv.pf_backlight_off(&lcd_drv);
    }
    else if(self->name == display_st7735) {
        st7735_drv.pf_backlight_off(&st7735_drv);
    } else {
        pr_error("Unsupported display type: %s", self->name);
    }
}

static void bsp_adapter_display_put_pixel(struct display_wrapper * self, uint16_t x, uint16_t y, uint32_t color)
{
    if(self->name == display_lcd) {
        lcd_drv.pf_put_pixel(&lcd_drv, x, y, color);
    }
    else if(self->name == display_st7735) {
        st7735_drv.pf_put_pixel(&st7735_drv, x, y, (uint16_t)color);
    } else {
        pr_error("Unsupported display type: %s", self->name);
    }
}

static void bsp_adapter_display_fill_rect(struct display_wrapper * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    if(self->name == display_lcd) {
        lcd_drv.pf_fill_rect(&lcd_drv, x, y, width, height, color);
    }
    else if(self->name == display_st7735) {
        st7735_drv.pf_fill_rect(&st7735_drv, x, y, width, height, (uint16_t)color);
    } else {
        pr_error("Unsupported display type: %s", self->name);
    }
}

static void bsp_adapter_display_fill_screen(struct display_wrapper * self, uint32_t color)
{
    if(self->name == display_lcd) {
        lcd_drv.pf_fill_screen(&lcd_drv, color);
    }
    else if(self->name == display_st7735) {
        st7735_drv.pf_fill_screen(&st7735_drv, (uint16_t)color);
    } else {
        pr_error("Unsupported display type: %s", self->name);
    }
}

static void bsp_adapter_display_copy_buffer(struct display_wrapper * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    if(self->name == display_lcd) {
        lcd_drv.pf_copy_buffer(&lcd_drv, x, y, width, height, data);
    }
    else if(self->name == display_st7735) {
        st7735_drv.pf_copy_buffer(&st7735_drv, x, y, width, height, (uint16_t *)data);
    } else {
        pr_error("Unsupported display type: %s", self->name);
    }
}


/******************************* (END OF FILE) *********************************/

