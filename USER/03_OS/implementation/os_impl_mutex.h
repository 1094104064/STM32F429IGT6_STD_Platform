/**
  ******************************************************************************
  *
  * @file    os_impl_mutex.h
  * @author  
  * @brief   Header file of os_impl_mutex module.
  *
  ******************************************************************************
  **/
  
#ifndef _OS_IMPL_MUTEX_H
#define _OS_IMPL_MUTEX_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
#include "user_conf.h"
#include "os_macro.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
#ifdef USING_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"


#endif

/**********************
*  GLOBAL PROTOTYPES
 **********************/



/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_OS_IMPL_MUTEX_H*/


/******************************* (END OF FILE) *********************************/


