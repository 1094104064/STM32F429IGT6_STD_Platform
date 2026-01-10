/**
  ******************************************************************************
  *
  * @file    ud_macros.h
  * @author  
  * @brief   Header file of udevice macros module.
  *
  ******************************************************************************
  **/

#ifndef _UD_MACROS_H
#define _UD_MACROS_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> 

#ifdef __GNUC__
    #define UD_CONTAINER_OF(ptr, type, member) ({ \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); })
#else
    #define UD_CONTAINER_OF(ptr, type, member) \
        ((type *)((char *)(ptr) - offsetof(type, member)))
#endif


#ifdef __cplusplus
}
#endif

#endif /*_UD_MACROS_H*/


/******************************* (END OF FILE) *********************************/


