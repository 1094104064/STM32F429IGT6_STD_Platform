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
#include "../adapter/touchpad_i2c.h"
#include "../adapter/touchpad_ctrl.h"
/*********************
 *      DEFINES
 *********************/
#define GT9XX_MAX_TOUCH_POINTS    5

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

struct ud_gt9xx
{
    const struct ud_touch_ops * touch_ops;
    const struct touchpad_i2c_ops ** i2c_ops;
    const struct touchpad_ctrl_ops ** ctrl_ops;
};

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    GT9XX_IOCTL_GET_PRODUCT_ID = 0,
    GT9XX_IOCTL_SET_IRQ_CALLBACK,
    GT9XX_IOCTL_RESET,
} gt9xx_ioctl_cmd_t;
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


