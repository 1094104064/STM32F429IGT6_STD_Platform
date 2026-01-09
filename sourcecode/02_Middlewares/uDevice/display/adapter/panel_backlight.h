/**
  ******************************************************************************
  *
  * @file    panel_backlight.h
  * @author  
  * @brief   Header file of panel backlight module.
  *
  ******************************************************************************
  **/

#ifndef _PANEL_BACKLIGHT_H
#define _PANEL_BACKLIGHT_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
struct panel_backlight_ops
{
    void    (* enable)         (struct panel_backlight_ops ** ops, bool state);
    void    (* set_brightness) (struct panel_backlight_ops ** ops, uint8_t brightness);
    uint8_t (* get_brightness) (struct panel_backlight_ops ** ops);
};

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void panel_backlight_enable(struct panel_backlight_ops ** ops, bool state)
{
    (*ops)->enable(ops, state);
}

static inline void panel_backlight_set_brightness(struct panel_backlight_ops ** ops, uint8_t brightness)
{
    (*ops)->set_brightness(ops, brightness);
}

static inline uint8_t panel_backlight_get_brightness(struct panel_backlight_ops ** ops)
{
    return (*ops)->get_brightness(ops);
}


/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_PANEL_BACKLIGHT_H*/


/******************************* (END OF FILE) *********************************/









