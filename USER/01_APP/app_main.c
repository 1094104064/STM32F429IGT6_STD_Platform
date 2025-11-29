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
display_obj_t * g_lcd;
/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t ElapsTick;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
static void InitMiddleware(void)
{
#ifdef USING_ELOG
    elog_init();
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_start();
#endif

#ifdef USING_LVGL
    lv_init();
    
    #ifdef USING_LVGL_DISP
        lv_port_disp_init();
    #endif
    
    #ifdef USING_LVGL_INDEV
        lv_port_indev_init();
    #endif

    #ifdef USING_LVGL_DEMO_WIDGETS
        lv_demo_widgets();
    #endif

    #ifdef USING_LVGL_DEMO_BENCHMARK
        lv_demo_benchmark();
    #endif
#endif
}

#ifdef USING_FREERTOS
static void StartUpTask(void * pvParameters)
{
    for(;;) {

        ElapsTick = xTaskGetTickCount();

        pr_info("ElapsTick = %d", *(uint32_t *)ElapsTick);

        vTaskDelay(2000);
    }   
    
}
#endif

static void InitOS(void)
{
#ifdef USING_FREERTOS
    static TaskHandle_t StartupTaskHandle = {0};

    xTaskCreate(StartUpTask, "start-up task", 512, NULL, tskIDLE_PRIORITY + 1, &StartupTaskHandle);
    vTaskStartScheduler();
#endif
}

void app_main(void)
{
    delay_init();
    debug_init(230400);
    mm_init();

    InitLED();
    InitLCD();
    InitW25Q64();

    g_blue_led = wrp_light.find(BLUE_LED_NAME);
    g_yellow_led = wrp_light.find(YELLOW_LED_NAME);
    g_lcd = wrp_display.find(LCD_NAME);


    InitMiddleware();
    InitOS();

    for(;;) {

        ElapsTick = STD_SYSTICK_Get();

        pr_info("ElapsTick = %d", ElapsTick);

        delay_s(2);
    }

}



/**********************
 *   STATIC FUNCTIONS
 **********************/



/******************************* (END OF FILE) *********************************/

