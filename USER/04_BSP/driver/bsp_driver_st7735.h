/**
  ******************************************************************************
  *
  * @file    bsp_driver_st7735.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_st7735 module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_DRIVER_ST7735_H
#define _BSP_DRIVER_ST7735_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct st7735_oper_info {
    uint16_t width;
    uint16_t height;
    uint8_t rotated;
};

struct st7735_oper_spi {
    void (* pf_transmit_8bit)(uint8_t data);
    void (* pf_transmit_16bit)(uint16_t data, uint32_t size);
    void (* pf_transmit_dma_8bit)(uint8_t * buf, uint32_t size);
    void (* pf_transmit_dma_16bit)(uint16_t * buf, uint32_t size);
};


struct st7735_oper_ctrl {
    void (* pf_cs_high)(void);
    void (* pf_cs_low)(void);
    void (* pf_dc_high)(void);
    void (* pf_dc_low)(void);
    void (* pf_rst_high)(void);
    void (* pf_rst_low)(void);
};

struct st7735_oper_backlight {
    void (* pf_on)(void);
    void (* pf_off)(void);
    void (* pf_set)(uint8_t brightness);
};


struct st7735_oper {
    struct st7735_oper_info * oper_info;
    struct st7735_oper_spi *  oper_spi;
    struct st7735_oper_ctrl * oper_ctrl;
    struct st7735_oper_backlight * oper_backlight;

    void (* pf_delay_ms)(uint32_t ms);
};

struct st7735_driver {

    struct st7735_oper * oper;

    void (* pf_write_data)(struct st7735_driver * self, uint8_t byte);
    void (* pf_write_command)(struct st7735_driver * self, uint8_t cmd);
    bool (* pf_init)(struct st7735_driver * self);
    void (* pf_set_cursor)(struct st7735_driver * self, uint16_t x, uint16_t y);
    void (* pf_set_window)(struct st7735_driver * self, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void (* pf_put_pixel)(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t color);
    void (* pf_fill_rect)(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void (* pf_fill_screen)(struct st7735_driver * self, uint16_t color);
    void (* pf_copy_buffer)(struct st7735_driver * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t * data);

    void (* pf_backlight_on)(struct st7735_driver * self);
    void (* pf_backlight_off)(struct st7735_driver * self);
    void (* pf_backlight_set)(struct st7735_driver * self, uint8_t brightness);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_st7735_link(    struct st7735_driver * self, 
                                struct st7735_oper * oper,
                                struct st7735_oper_info * info,
                                struct st7735_oper_spi * spi,
                                struct st7735_oper_ctrl * ctrl,
                                struct st7735_oper_backlight * backlight);
/**********************
 *      MACROS
 **********************/



#define ST7735_NOP                          0x00U  /* No Operation: NOP                           */
#define ST7735_SW_RESET                     0x01U  /* Software reset: SWRESET                     */
#define ST7735_READ_ID                      0x04U  /* Read Display ID: RDDID                      */
#define ST7735_READ_STATUS                  0x09U  /* Read Display Statu: RDDST                   */
#define ST7735_READ_POWER_MODE              0x0AU  /* Read Display Power: RDDPM                   */
#define ST7735_READ_MADCTL                  0x0BU  /* Read Display: RDDMADCTL                     */
#define ST7735_READ_PIXEL_FORMAT            0x0CU  /* Read Display Pixel: RDDCOLMOD               */  
#define ST7735_READ_IMAGE_MODE              0x0DU  /* Read Display Image: RDDIM                   */
#define ST7735_READ_SIGNAL_MODE             0x0EU  /* Read Display Signal: RDDSM                  */           
#define ST7735_SLEEP_IN                     0x10U  /* Sleep in & booster off: SLPIN               */ 
#define ST7735_SLEEP_OUT                    0x11U  /* Sleep out & booster on: SLPOUT              */
#define ST7735_PARTIAL_DISPLAY_ON           0x12U  /* Partial mode on: PTLON                      */ 
#define ST7735_NORMAL_DISPLAY_OFF           0x13U  /* Partial off (Normal): NORON                 */
#define ST7735_DISPLAY_INVERSION_OFF        0x20U  /* Display inversion off: INVOFF               */
#define ST7735_DISPLAY_INVERSION_ON         0x21U  /* Display inversion on: INVON                 */
#define ST7735_GAMMA_SET                    0x26U  /* Gamma curve select: GAMSET                  */
#define ST7735_DISPLAY_OFF                  0x28U  /* Display off: DISPOFF                        */
#define ST7735_DISPLAY_ON                   0x29U  /* Display on: DISPON                          */
#define ST7735_CASET                        0x2AU  /* Column address set: CASET                   */ 
#define ST7735_RASET                        0x2BU  /* Row address set: RASET                      */
#define ST7735_WRITE_RAM                    0x2CU  /* Memory write: RAMWR                         */  
#define ST7735_RGBSET                       0x2DU  /* LUT for 4k,65k,262k color: RGBSET           */
#define ST7735_READ_RAM                     0x2EU  /* Memory read: RAMRD                          */
#define ST7735_PTLAR                        0x30U  /* Partial start/end address set: PTLAR        */ 
#define ST7735_TE_LINE_OFF                  0x34U  /* Tearing effect line off: TEOFF              */ 
#define ST7735_TE_LINE_ON                   0x35U  /* Tearing effect mode set & on: TEON          */ 
#define ST7735_MADCTL                       0x36U  /* Memory data access control: MADCTL          */ 
#define ST7735_IDLE_MODE_OFF                0x38U  /* Idle mode off: IDMOFF                       */ 
#define ST7735_IDLE_MODE_ON                 0x39U  /* Idle mode on: IDMON                         */ 
#define ST7735_COLOR_MODE                   0x3AU  /* Interface pixel format: COLMOD              */
#define ST7735_FRAME_RATE_CTRL1             0xB1U  /* In normal mode (Full colors): FRMCTR1       */
#define ST7735_FRAME_RATE_CTRL2             0xB2U  /* In Idle mode (8-colors): FRMCTR2            */   
#define ST7735_FRAME_RATE_CTRL3             0xB3U  /* In partial mode + Full colors: FRMCTR3      */
#define ST7735_FRAME_INVERSION_CTRL         0xB4U  /* Display inversion control: INVCTR           */
#define ST7735_DISPLAY_SETTING              0xB6U  /* Display function setting                    */
#define ST7735_PWR_CTRL1                    0xC0U  /* Power control setting: PWCTR1               */ 
#define ST7735_PWR_CTRL2                    0xC1U  /* Power control setting: PWCTR2               */ 
#define ST7735_PWR_CTRL3                    0xC2U  /* In normal mode (Full colors): PWCTR3        */
#define ST7735_PWR_CTRL4                    0xC3U  /* In Idle mode (8-colors): PWCTR4             */ 
#define ST7735_PWR_CTRL5                    0xC4U  /* In partial mode + Full colors: PWCTR5       */
#define ST7735_VCOMH_VCOML_CTRL1            0xC5U  /* VCOM control 1: VMCTR1                      */ 
#define ST7735_VMOF_CTRL                    0xC7U  /* Set VCOM offset control: VMOFCTR            */ 
#define ST7735_WRID2                        0xD1U  /* Set LCM version code: WRID2                 */ 
#define ST7735_WRID3                        0xD2U  /* Customer Project code: WRID3                */ 
#define ST7735_NV_CTRL1                     0xD9U  /* NVM control status: NVCTR1                  */
#define ST7735_READ_ID1                     0xDAU  /* Read ID1: RDID1                             */ 
#define ST7735_READ_ID2                     0xDBU  /* Read ID2: RDID2                             */ 
#define ST7735_READ_ID3                     0xDCU  /* Read ID3: RDID3                             */ 
#define ST7735_NV_CTRL2                     0xDEU  /* NVM Read Command: NVCTR2                    */ 
#define ST7735_NV_CTRL3                     0xDFU  /* NVM Write Command: NVCTR3                   */
#define ST7735_PV_GAMMA_CTRL                0xE0U  /* Set Gamma adjustment (+ polarity): GAMCTRP1 */
#define ST7735_NV_GAMMA_CTRL                0xE1U  /* Set Gamma adjustment (- polarity): GAMCTRN1 */
#define ST7735_EXT_CTRL                     0xF0U  /* Extension command control                   */
#define ST7735_PWR_CTRL6                    0xFCU  /* In partial mode + Idle mode: PWCTR6         */
#define ST7735_VCOM4_LEVEL                  0xFFU  /* VCOM 4 level control                        */

#define ST7735_FORMAT_RBG444                0x03U /* Pixel format chosen is RGB444 : 12 bpp */   
#define ST7735_FORMAT_RBG565                0x05U /* Pixel format chosen is RGB565 : 16 bpp */
#define ST7735_FORMAT_RBG666                0x06U /* Pixel format chosen is RGB666 : 18 bpp */

#define ST7735_ORIENTATION_PORTRAIT         0x00U /* Portrait orientation choice of LCD screen               */
#define ST7735_ORIENTATION_PORTRAIT_ROT180  0xC0U /* Portrait rotated 180� orientation choice of LCD screen  */
#define ST7735_ORIENTATION_LANDSCAPE        0x70U /* Landscape orientation choice of LCD screen              */
#define ST7735_ORIENTATION_LANDSCAPE_ROT180 0xA0U /* Landscape rotated 180� orientation choice of LCD screen */
 



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_ST7735_H*/


/******************************* (END OF FILE) *********************************/

