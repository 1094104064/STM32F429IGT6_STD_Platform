/**
  ******************************************************************************
  *
  * @file    user_conf.h
  * @author  Jamin
  * @brief   Header file of user_conf module.
  *
  ******************************************************************************
  **/
  
#ifndef _USER_CONF_H
#define _USER_CONF_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      DEFINES
 *********************/
#define CFG_USE_LOG                 1
 
#define CFG_USE_ELOG                0
 
#define CFG_USE_FREERTOS            0

#define CFG_USE_LVGL                0
#define CFG_USE_LVGL_DISP           1
#define CFG_USE_LVGL_INDEV          1
#define CFG_USE_LVGL_BENCHMARK      1
#define CFG_USE_LVGL_WIDGETS        2
#define CFG_USE_LVGL_DEMO           0

#define CFG_USE_FATFS               0

/**********************
 *      MACROS
 **********************/
#if CFG_USE_LOG
    #define USING_LOG
#endif
 
#if CFG_USE_ELOG
    #define USING_ELOG
#endif
 
#if CFG_USE_FREERTOS
    #define USING_FREERTOS
#endif

#if CFG_USE_LVGL
    #define USING_LVGL
    
    #if CFG_USE_LVGL_DISP
        #define USING_LVGL_DISP
    #endif
    
    #if CFG_USE_LVGL_INDEV
        #define USING_LVGL_INDEV
    #endif
    
    #if CFG_USE_LVGL_DEMO == 1
        #define USING_LVGL_DEMO_BENCHMARK
    #elif CFG_USE_LVGL_DEMO == 2
        #define USING_LVGL_DEMO_WIDGETS
    #else
        #define NO_USING_LVGL_DEMO
    #endif
    
#endif

#if CFG_USE_FATFS
    #define USING_FATFS
#endif
/**********************
 *      TYPEDEFS
 **********************/
 

#ifdef __cplusplus
}
#endif

#endif /*_USER_CONF_H*/


/******************************* (END OF FILE) *********************************/

