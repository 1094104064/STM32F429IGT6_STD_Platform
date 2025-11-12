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

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      MACROS
 **********************/


//#if LOG_ENABLE
//    #define assert_msg(expr, msg)                                           \
//        do {                                                                \
//            if((expr)) {                                                    \
//                pr_fatal("Asserted at expression: %s (%s)", #expr, msg);    \
//                while(1);                                                   \
//            }                                                               \
//        } while(0)                                                          
//#else
//    #define assert_msg(expr, msg)  do{}while(0)
//#endif

//#define assert_null(p) assert_msg(p == NULL, "NULL pointer")





#ifdef __cplusplus
}
#endif

#endif /*_USER_CONF_H*/


/******************************* (END OF FILE) *********************************/

