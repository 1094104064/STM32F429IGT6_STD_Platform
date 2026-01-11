/**
  ******************************************************************************
  *
  * @file    touchpad_ctrl.h
  * @author  
  * @brief   Header file of touchpad_ctrl module.
  *
  ******************************************************************************
  **/

#ifndef _TOUCHPAD_CTRL_H
#define _TOUCHPAD_CTRL_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
struct touchpad_ctrl_ops
{
    void (* reset) (const struct touchpad_ctrl_ops ** ops);
    void (* set_callback) (const struct touchpad_ctrl_ops ** ops, void (* irq_cb)(void));
};
/**********************
 *      TYPEDEFS
 **********************/
typedef void (* touchpad_irq_callback_t)(void);
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void touchpad_ctrl_reset(const struct touchpad_ctrl_ops ** ops)
{
    (*ops)->reset(ops);
}

static inline void touchpad_ctrl_set_callback(const struct touchpad_ctrl_ops ** ops, touchpad_irq_callback_t irq_cb)
{
    (*ops)->set_callback(ops, irq_cb);
}


/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_TOUCHPAD_CTRL_H*/


/******************************* (END OF FILE) *********************************/


