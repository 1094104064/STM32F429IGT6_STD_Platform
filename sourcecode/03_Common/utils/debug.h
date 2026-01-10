/**
  ******************************************************************************
  *
  * @file    debug.h
  * @author  
  * @brief   Header file of debug module.
  *
  ******************************************************************************
  **/
  
#ifndef _DEBUG_H
#define _DEBUG_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/
#define LOG_LEVEL_TRACE   0     /* 最低级别，用于最详细的跟踪信息 */
#define LOG_LEVEL_DEBUG   1     /* 调试信息，开发阶段使用 */
#define LOG_LEVEL_INFO    2     /* 重要状态信息，测试阶段使用 */
#define LOG_LEVEL_WARN    3     /* 警告信息，提示潜在问题 */
#define LOG_LEVEL_ERROR   4     /* 错误信息，功能异常但可恢复 */
#define LOG_LEVEL_FATAL   5     /* 严重错误，系统可能崩溃 */
#define LOG_LEVEL_NONE    6     /* 不输出日志 */

#define LOG_LEVEL LOG_LEVEL_TRACE
/**********************
 *      TYPEDEFS
 **********************/
typedef void (* dbg_putc_cb_t)(char c);
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void    debug_init(dbg_putc_cb_t putc_cb);
void    debug_putc(char c);
int     debug_puts(const char *fmt, ...);


/**********************
 *      MACROS
 **********************/
#ifdef USING_LOG
    #define LOG_ENABLE  1
#else
    #define LOG_ENABLE  0
#endif

#if LOG_ENABLE
    #define pr_log(fmt, ...)        debug_puts(fmt, ##__VA_ARGS__)
#else
    #define pr_log(fmt, ...)        do {} while (0)
#endif

#ifndef pr_trace
    #if LOG_LEVEL <= LOG_LEVEL_TRACE
        #define pr_trace(fmt, ...)      pr_log("[Trace] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
        #define pr_trace(fmt, ...)      do {} while (0)
    #endif
#endif

#ifndef pr_debug
    #if LOG_LEVEL <= LOG_LEVEL_DEBUG
        #define pr_debug(fmt, ...)      pr_log("[Debug] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
        #define pr_debug(fmt, ...)      do {} while (0)
    #endif
#endif

#ifndef pr_info
    #if LOG_LEVEL <= LOG_LEVEL_INFO
        #define pr_info(fmt, ...)       pr_log("[Info ] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
        #define pr_info(fmt, ...)       do {} while (0)
    #endif
#endif

#ifndef pr_warn
    #if LOG_LEVEL <= LOG_LEVEL_WARN
        #define pr_warn(fmt, ...)       pr_log("[Warn ] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
        #define pr_warn(fmt, ...)       do {} while (0)
    #endif
#endif

#ifndef pr_error
    #if LOG_LEVEL <= LOG_LEVEL_ERROR
        #define pr_error(fmt, ...)      pr_log("[Error] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
        #define pr_error(fmt, ...)      do {} while (0)
    #endif
#endif

#ifndef pr_fatal
    #if LOG_LEVEL <= LOG_LEVEL_FATAL
        #define pr_fatal(fmt, ...)      do { pr_log("[Fatal] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); while(1); } while(0)
    #else
        #define pr_fatal(fmt, ...)      do {} while (0)
    #endif
#endif

#ifndef pr_user
    #define pr_user(fmt, ...)           pr_log("[User] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifndef ERROR_HANDLER
    #define ERROR_HANDLER(msg)         do { pr_fatal(msg); } while(0)
#endif

#ifndef ASSERT_MSG
    #define ASSERT_MSG(expr, msg)      do { if((expr)) { ERROR_HANDLER(msg); } } while(0)
#endif

#ifndef ASSERT_NULL
    #define ASSERT_NULL(ptr)            ASSERT_MSG((ptr) == NULL, "NULL pointer")
#endif


#ifdef __cplusplus
}
#endif

#endif /*_DEBUG_H*/


/******************************* (END OF FILE) *********************************/


