/**
  ******************************************************************************
  *
  * @file    panel_rgb.h
  * @author  
  * @brief   Header file of panel_rgb module.
  *
  ******************************************************************************
  **/
  
#ifndef _PANEL_RGB_H
#define _PANEL_RGB_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stddef.h>
/*********************
 *      DEFINES
 *********************/
struct panel_rgb_layer
{
    uint8_t color_mode;
    const void * address;
};

struct panel_rgb_ops
{
    void (* create_layer)  (const struct panel_rgb_ops ** ops, struct panel_rgb_layer * layer);
    void (* fill_data)       (const struct panel_rgb_ops ** ops, 
                              uint16_t x,
                              uint16_t y,
                              uint16_t width,
                              uint16_t height,
                              const void * dst, 
                              uint32_t color);
    void (* copy_buffer)     (const struct panel_rgb_ops ** ops, 
                              uint16_t x,
                              uint16_t y,
                              uint16_t width,
                              uint16_t height,
                              const void * src,
                              const void * dst);
};
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

static inline void panel_rgb_create_layer(const struct panel_rgb_ops ** ops, struct panel_rgb_layer * layer)
{
    (*ops)->create_layer(ops, layer);
}

static inline void panel_rgb_fill_data(const struct panel_rgb_ops ** ops, 
                                       uint16_t x,
                                       uint16_t y,
                                       uint16_t width,
                                       uint16_t height,
                                       const void * dst, 
                                       uint32_t color)
{
    (*ops)->fill_data(ops, x, y, width, height, dst, color);
}

static inline void panel_rgb_copy_buffer(const struct panel_rgb_ops ** ops,
                                         uint16_t x,
                                         uint16_t y,
                                         uint16_t width,
                                         uint16_t height,
                                         const void * src,
                                         const void * dst)
{
    (*ops)->copy_buffer(ops, x, y, width, height, src, dst);
}


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_PANEL_RGB_H*/


/******************************* (END OF FILE) *********************************/


