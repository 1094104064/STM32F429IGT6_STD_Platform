/**
  ******************************************************************************
  *
  * @file    ud_display.c
  * @author  
  * @brief   
  *
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  **/
  
  
/*********************
 *      INCLUDES
 *********************/
#include "./ud_display.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
struct ud_display_ops ** ud_display_create(int type)
{

}

void ud_display_init()
{

}

void ud_display_draw_point(struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint32_t color)
{

}

void ud_display_draw_line(struct ud_display_ops ** ops, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{

}

void ud_display_draw_block(struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{

}

void ud_display_clear(struct ud_display_ops ** ops, uint32_t color)
{

}

void ud_display_draw_circle(struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t radius, uint32_t color)
{

}

void ud_display_draw_triangle(struct ud_display_ops ** ops, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color)
{

}

void ud_display_draw_rectangle(struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{

}

void ud_display_draw_ellipse(struct ud_display_ops ** ops, int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color)
{

}

void ud_display_draw_arc(struct ud_display_ops ** ops, uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color)
{

}

void ud_display_flush(struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const void * buffer)
{

}

void ud_display_enable_backlight(struct ud_display_ops ** ops, bool state)
{

}

void ud_display_set_backlight_brightness(struct ud_display_ops ** ops, uint8_t brightness)
{

}

uint8_t ud_display_get_backlight_brightness(struct ud_display_ops ** ops)
{

    return 0;
}

void ud_display_ioctl(struct ud_display_ops ** ops, unsigned int cmd, void * arg)
{

}


void ud_display_draw_rgb_grad(struct ud_display_ops ** ops, uint16_t grid_size)
{

}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







