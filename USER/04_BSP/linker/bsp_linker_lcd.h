/**
  ******************************************************************************
  *
  * @file    bsp_linker_lcd.h
  * @author  
  * @brief   Header file of bsp_linker_lcd module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_LINKER_LCD_H
#define _BSP_LINKER_LCD_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void        bsp_linker_lcd_simple_panel_init   (void);
void        bsp_linker_lcd_backlight_on        (void);
void        bsp_linker_lcd_backlight_off       (void);
void        bsp_linker_lcd_backlight_set       (uint8_t brightness);
void        bsp_linker_lcd_switch_layer        (uint8_t layerx);
void        bsp_linker_lcd_put_pixel           (uint16_t x, uint16_t y, uint32_t color, uint16_t rotated);
void        bsp_linker_lcd_fill_rectangle      (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void        bsp_linker_lcd_fill_screen         (uint32_t color);
void        bsp_linker_lcd_copy_buffer         (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * data);
uint16_t    bsp_linker_lcd_get_width           (void);
uint16_t    bsp_linker_lcd_get_height          (void);
uint32_t    bsp_linker_lcd_get_layer_address   (void);


/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_BSP_LINKER_LCD_H*/


/******************************* (END OF FILE) *********************************/


