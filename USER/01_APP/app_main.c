/**
  ******************************************************************************
  *
  * @file    app_main.c
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
light_obj_t * g_blue_led;
light_obj_t * g_yellow_led;
/**********************
 *  STATIC PROTOTYPES
 **********************/


#if OS_ENABLE
static void _task_init(void * pvParameters);
#endif
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 



void app_main(void)
{
    delay_init();
    debug_init(230400);
    mm_init();

    g_blue_led = wrp_light.obj_create(&blue_led_ops, "Blue LED", NULL);
    g_yellow_led = wrp_light.obj_create(&yellow_led_ops, "Yellow LED", NULL);

    wrp_light.init(g_blue_led);
    wrp_light.init(g_yellow_led);

    int * ptr = mm_alloc(512);
    pr_info("ptr = %x", ptr);

#if ELOG_ENABLE
    elog_init();
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_start();
#endif

#if LVGL_ENABLE
    lv_init();
    lv_port_disp_init();
    // lv_port_indev_init();

    #if LV_USE_DEMO_WIDGETS
        lv_demo_widgets();
    #endif

    #if LV_USE_DEMO_BENCHMARK
        lv_demo_benchmark();
    #endif
#endif
    
    
    static uint32_t ElapsTick;

#if OS_ENABLE
    static TaskHandle_t StartupTaskHandle = {0};

    xTaskCreate(_task_init, "task_init", 512, &ElapsTick, tskIDLE_PRIORITY + 1, &StartupTaskHandle);
    vTaskStartScheduler();
#else

    for(;;) {

        ElapsTick = STD_SYSTICK_Get();

        pr_info("ElapsTick = %d", ElapsTick);

        delay_s(2);
    }

#endif
}



/**********************
 *   STATIC FUNCTIONS
 **********************/
static void _core_init(void)
{
    /* Initialize core components here */
    
//    STD_USART1_Init();
//    STD_USART1_NVIC_Init();
//    
//    

//    STD_GPIO_BspInit();

//    STD_DMA_ClockCmd(DMA1, ENABLE);
//    STD_DMA_ClockCmd(DMA2, ENABLE);
//    STD_DMA2D_ClockCmd(ENABLE);

//    STD_FMC_SDRAM_Init();
//    // STD_LTDC_Init();
//    // STD_LTDC_LayerInit();

//    STD_SPI1_Init();
//    STD_SPI3_Init();
}



#if OS_ENABLE
static void _task_init(void * pvParameters)
{
    /* Initialize tasks here */
    for(;;) {

        *(uint32_t *)pvParameters = xTaskGetTickCount();

        pr_info("ElapsTick = %d", *(uint32_t *)pvParameters);

        vTaskDelay(2000);
    }

}
#endif

/******************************* (END OF FILE) *********************************/

