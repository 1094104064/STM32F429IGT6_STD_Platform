/**
  ******************************************************************************
  *
  * @file    ud_gt9xx.h
  * @author  
  * @brief   Header file of gt9xx module.
  *
  ******************************************************************************
  **/
  
#ifndef _UD_GT9XX_H
#define _UD_GT9XX_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../core/ud_touch.h"
/*********************
 *      DEFINES
 *********************/
#define GT9XX_ADDRESS_HIGH  0x5D
#define GT9XX_ADDRESS_LOW   0x14

#define GT9XX_COMMAND_REG   0x8040
#define GT9XX_CONFIG_REG    0x8047
#define GT9XX_MOD_SWT_REG   0x804D

#define GT9XX_PRODUCT_ID    0x8140
#define GT9XX_VENDOR_ID     0x814A
#define GT9XX_READ_STATUS   0x814E

#define GT9XX_POINT1_REG    0x814F
#define GT9XX_POINT2_REG    0x8157
#define GT9XX_POINT3_REG    0x815F
#define GT9XX_POINT4_REG    0x8167
#define GT9XX_POINT5_REG    0x816F

#define GT9XX_CHECK_SUM     0x80FF
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/



/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_UD_GT9XX_H*/


/******************************* (END OF FILE) *********************************/


