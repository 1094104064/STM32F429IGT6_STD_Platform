/**
  ******************************************************************************
  *
  * @file    app_init.c
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
#include "app_init.h"
#include "main.h"
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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void app_init_core(void)
{
    /* Initialize core components here */
    STD_DWT_Init();
    STD_USART1_Init();
    STD_USART1_NVIC_Init();
    
    STD_SYSTICK_Init();

    STD_GPIO_BspInit();

    STD_DMA_ClockCmd(DMA1, ENABLE);
    STD_DMA_ClockCmd(DMA2, ENABLE);
    STD_DMA2D_ClockCmd(ENABLE);

    STD_FMC_SDRAM_Init();
    // STD_LTDC_Init();
    // STD_LTDC_LayerInit();

    STD_SPI1_Init();
    STD_SPI3_Init();
}

void app_init_bsp(void)
{
    /* Initialize BSP components here */
    bsp_adapter_delay_register();
    bsp_wrapper_delay_init();

    bsp_adapter_light_register();
    bsp_wrapper_light_set_operation_object("blue_led");
    bsp_wrapper_light_init();
    bsp_wrapper_light_set_operation_object("yellow_led");
    bsp_wrapper_light_init();

    bsp_adapter_display_register();
    bsp_wrapper_display_init();
    bsp_wrapper_display_backlight_on();
    // bsp_wrapper_display_fill_rect(0, 0, 100, 150, DISP_RGB565_ORANGE);
    // bsp_wrapper_display_fill_screen(DISP_RGB565_PURPLE);
    bsp_wrapper_display_draw_grad_rgb565(10);

    bsp_adapter_touchpad_register();
    bsp_wrapper_touchpad_init();

    bsp_adapter_sflash_register();
    bsp_wrapper_sflash_init();
    // bsp_wrapper_sflash_test();
}

void app_init_middleware(void)
{
    /* Initialize middleware components here */
#if 0
    elog_init();
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_start();
#endif

#if 0
    lv_init();
    lv_port_disp_init();
    // lv_port_indev_init();
    // lv_demo_widgets();
    lv_demo_benchmark();
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

