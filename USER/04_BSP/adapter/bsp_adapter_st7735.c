/**
  ******************************************************************************
  *
  * @file    bsp_adapter_st7735.c
  * @author  
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
#include "bsp_wrapper_display.h"
#include "bsp_adapter_st7735.h"
#include "bsp_driver_st7735.h"
#include "bsp_linker_st7735.h"
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

/**********************
 *  STATIC VARIABLES
 **********************/
static st7735_driver_t gs_st7735_driver;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int ops_st7735_init(void)
{
    static const st7735_handle_t st7735_handle = {

        .pf_spi_transmit_8bit       = NULL,
        .pf_spi_transmit_16bit      = NULL,
        .pf_spi_dma_transmit_8bit   = NULL,
        .pf_spi_dma_transmit_16bit  = NULL,

        // .pf_cs_high                 = bsp_linker_st7735_cs_high,
        // .pf_cs_low                  = bsp_linker_st7735_cs_low,
        // .pf_dc_high                 = bsp_linker_st7735_dc_high,
        // .pf_dc_low                  = bsp_linker_st7735_dc_low,
        // .pf_rst_high                = bsp_linker_st7735_rst_high,
        // .pf_rst_low                 = bsp_linker_st7735_rst_low,

        // .pf_backlight_on            = bsp_linker_st7735_backlight_on,
        // .pf_backlight_off           = bsp_linker_st7735_backlight_off,
        // .pf_backlight_set           = bsp_linker_st7735_backlight_set,

        // .pf_get_width               = bsp_linker_st7735_get_width,
        // .pf_get_height              = bsp_linker_st7735_get_height,
        
        // .pf_delay_ms                = bsp_linker_st7735_delay_ms,
    };

    bsp_driver_st7735_link(&gs_st7735_driver, &st7735_handle);

    if (gs_st7735_driver.pf_init(&gs_st7735_driver) == false) {
        return 2;
    }

    return 0;
}

static void ops_st7735_backlight_on(void)
{
    
}

static void ops_st7735_backlight_off(void)
{
    
}

static void ops_st7735_backlight_set(uint8_t brightness)
{
    
}

static void ops_st7735_put_pixel(uint16_t x, uint16_t y, uint32_t color)
{
    
}

static void ops_st7735_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    
}

static void ops_st7735_fill_screen(uint32_t color)
{
    
}

static void ops_st7735_copy_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data)
{
    
}

static uint16_t ops_st7735_get_width(void)
{
    return gs_st7735_driver.width;
}

static uint16_t ops_st7735_get_height(void)
{
    return gs_st7735_driver.height;
}

static const display_ops_t st7735_ops = {
    .pf_init                = ops_st7735_init,
    .pf_backlight_on        = ops_st7735_backlight_on,
    .pf_backlight_off       = ops_st7735_backlight_off,
    .pf_backlight_set       = ops_st7735_backlight_set,
    .pf_put_pixel           = ops_st7735_put_pixel,
    .pf_fill_rect           = ops_st7735_fill_rect,
    .pf_fill_screen         = ops_st7735_fill_screen,
    .pf_copy_buffer         = ops_st7735_copy_buffer,
    .pf_get_width           = ops_st7735_get_width,
    .pf_get_height          = ops_st7735_get_height,
};

void InitST7735(void)
{
    display_obj_t * st7735_obj = NULL;

    st7735_obj = wrp_display.obj_create(&st7735_ops, "ST7735", NULL);

    if(st7735_obj == NULL)
    {
        for(;;);
    }

    wrp_display.init(st7735_obj);
}


/******************************* (END OF FILE) *********************************/



 







