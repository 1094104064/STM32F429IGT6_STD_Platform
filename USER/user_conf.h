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
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define PLATFORM_VERSION "1.1.0"
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      MACROS
 **********************/

#if 1
    #define pr_log(fmt, ...)        printf(fmt, ##__VA_ARGS__)
#else
    #define pr_log(fmt, ...)        do {} while (0)
#endif

#define LOG_LEVEL_TRACE   0     /* 最低级别，用于最详细的跟踪信息 */
#define LOG_LEVEL_DEBUG   1     /* 调试信息，开发阶段使用 */
#define LOG_LEVEL_INFO    2     /* 重要状态信息，测试阶段使用 */
#define LOG_LEVEL_WARN    3     /* 警告信息，提示潜在问题 */
#define LOG_LEVEL_ERROR   4     /* 错误信息，功能异常但可恢复 */
#define LOG_LEVEL_FATAL   5     /* 严重错误，系统可能崩溃 */
#define LOG_LEVEL_NONE    6     /* 不输出日志 */

#define LOG_LEVEL LOG_LEVEL_TRACE


#if LOG_LEVEL <= LOG_LEVEL_TRACE
    #define pr_trace(fmt, ...)      pr_log("[Trace] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define pr_trace(fmt, ...)      do {} while (0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
    #define pr_debug(fmt, ...)      pr_log("[Debug] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define pr_debug(fmt, ...)      do {} while (0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
    #define pr_info(fmt, ...)       pr_log("[Info ] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define pr_info(fmt, ...)       do {} while (0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARN
    #define pr_warn(fmt, ...)       pr_log("[Warn ] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define pr_warn(fmt, ...)       do {} while (0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
    #define pr_error(fmt, ...)      pr_log("[Error] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define pr_error(fmt, ...)      do {} while (0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_FATAL
    #define pr_fatal(fmt, ...)      pr_log("[Fatal] [%s:%d] " fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define pr_fatal(fmt, ...)      do {} while (0)
#endif





#ifdef __cplusplus
}
#endif

#endif /*_USER_CONF_H*/


/******************************* (END OF FILE) *********************************/

