/**
  ******************************************************************************
  *
  * @file    ud_touch.h
  * @author  
  * @brief   Header file of template module.
  *
  ******************************************************************************
  **/
  
#ifndef _UD_TOUCH_H
#define _UD_TOUCH_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
/**********************
 *      TYPEDEFS
 **********************/
typedef struct ud_touch_ops touch_ops_t;
typedef struct ud_touch_point touch_point_t;

typedef void (* touch_prepare_fn_t) (const touch_ops_t ** ops);
typedef void (* touch_read_data_fn_t) (const touch_ops_t ** ops, touch_point_t * points, uint8_t point_num);
typedef void (* touch_ioctrl_fn_t)(const touch_ops_t ** ops, unsigned int cmd, void * arg);
/*********************
 *      DEFINES
 *********************/
struct ud_touch_point
{
    uint16_t x;
    uint16_t y;
    uint8_t id;
    bool is_pressed;
};

struct ud_touch_ops
{
    touch_prepare_fn_t prepare;
    touch_read_data_fn_t    read_data;
    touch_ioctrl_fn_t ioctrl;
};
 
struct ud_touch
{
    const struct ud_touch_ops ** ops;
    struct ud_touch_point * point_list;
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/




/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_UD_TOUCH_H*/


/******************************* (END OF FILE) *********************************/


