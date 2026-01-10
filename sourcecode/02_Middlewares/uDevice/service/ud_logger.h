/**
  ******************************************************************************
  *
  * @file    ud_logger.h
  * @author  
  * @brief   Header file of logger module.
  *
  ******************************************************************************
  **/

#ifndef _UD_LOGGER_H
#define _UD_LOGGER_H
/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/
#define UD_LOG_LEVEL_TRACE   1
#define UD_LOG_LEVEL_INFO    2
#define UD_LOG_LEVEL_WARNING 3
#define UD_LOG_LEVEL_ERROR   4
#define UD_LOG_LEVEL_FATAL   5
#define UD_LOG_LEVEL_NONE    6
/**********************
 *      TYPEDEFS
 **********************/
typedef void (* ud_log_print_cb_t)(const char * buf);
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void ud_log_print(const char * fmt, ...);
/**********************
 *      MACROS
 **********************/

#if UD_LOG_ENABLE
    #define ud_log(fmt, ...)        ud_log_print(fmt, ##__VA_ARGS__)
#else
    #define ud_log(fmt, ...)        do {} while (0)
#endif


#define ud_log_trace(fmt, ...)    ud_log("[Trace] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ud_log_info(fmt, ...)     ud_log("[Info ] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ud_log_warning(fmt, ...)  ud_log("[Warning] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ud_log_error(fmt, ...)    ud_log("[Error] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ud_log_fatal(fmt, ...)    do { ud_log("[Fatal] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); while(1); } while(0)

#ifdef __cplusplus
extern "C" {
#endif






#ifdef __cplusplus
}
#endif

#endif /*_UD_LOGGER_H*/


/******************************* (END OF FILE) *********************************/


