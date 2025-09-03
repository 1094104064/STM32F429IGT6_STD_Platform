/**
  ******************************************************************************
  *
  * @file    ltdc.c
  * @author  Jamin
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
#include "ltdc.h"
#include "gpio.h"
#include "dma2d.h"
#include "bsp_driver_lcd.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/
extern struct framebuffer_layer layers[2];
extern const struct lcd_panel * panels;
/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/


/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 


void STD_LTDC_Init(void)
{
    STD_GPIO_Init(LTDC_R2_PORT, LTDC_R2_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_R3_PORT, LTDC_R3_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_R4_PORT, LTDC_R4_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_R5_PORT, LTDC_R5_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_R6_PORT, LTDC_R6_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, LTDC_R6_AF);
    STD_GPIO_Init(LTDC_R7_PORT, LTDC_R7_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);

    STD_GPIO_Init(LTDC_G2_PORT, LTDC_G2_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_G3_PORT, LTDC_G3_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_G4_PORT, LTDC_G4_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_G5_PORT, LTDC_G5_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_G6_PORT, LTDC_G6_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_G7_PORT, LTDC_G7_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);

    STD_GPIO_Init(LTDC_B2_PORT, LTDC_B2_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_B3_PORT, LTDC_B3_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_B4_PORT, LTDC_B4_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_B5_PORT, LTDC_B5_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_B6_PORT, LTDC_B6_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_B7_PORT, LTDC_B7_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);

    STD_GPIO_Init(LTDC_CLK_PORT,  LTDC_CLK_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_DE_PORT,   LTDC_DE_PIN,   GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_HSYNC_PORT,LTDC_HSYNC_PIN,GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);
    STD_GPIO_Init(LTDC_VSYNC_PORT,LTDC_VSYNC_PIN,GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_LTDC);

    uint16_t LCD_PLLSAIN = 0;   /* 用于倍频的PLLSAIN参数，可取范围为50~432 */
    uint8_t  LCD_PLLSAIR = 3;   /* 用于分频的PLLSAIR参数，可取范围为2~7 */
    uint8_t  LCD_CLKDIV  = 8;   /* LCD时钟分频参数，默认设置为8分频，数值上等于RCC_PLLSAIDivR_Div8 */

    LTDC_InitTypeDef  ltdc_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE); 	

    LCD_PLLSAIN = LCD_CLK * LCD_PLLSAIR * LCD_CLKDIV;       /* 根据需要使用的LCD时钟计算PLLSAIN参数，可取范围为50~432 */
    RCC_PLLSAIConfig(LCD_PLLSAIN, 7, LCD_PLLSAIR);        
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);              /* LCD时钟分频设置，要和LCD_CLKDIV对应 */
    RCC_PLLSAICmd(ENABLE);                                  /* 使能PLLSAI时钟 */ 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);	/* 等待时钟配置完成 */

    uint16_t active_width = panels->active_width;
    uint16_t active_height = panels->active_height;

    uint32_t hsync_width = panels->hsync_width;
    uint32_t hback_porch = panels->hback_porch;
    uint32_t hfront_porch = panels->hfront_porch;
    uint32_t vback_porch = panels->vback_porch;
    uint32_t vsync_width = panels->vsync_width;
    uint32_t vfront_porch = panels->vfront_porch;


    ltdc_init.LTDC_HSPolarity = LTDC_HSPolarity_AL; /* 低电平有效 */
    ltdc_init.LTDC_VSPolarity = LTDC_VSPolarity_AL; /* 低电平有效 */
    ltdc_init.LTDC_DEPolarity = LTDC_DEPolarity_AL; /* 低电平有效，要注意的是，很多面板都是高电平有效，但是429需要设置成低电平才能正常显示 */
    ltdc_init.LTDC_PCPolarity = LTDC_PCPolarity_IPC; /* 正常时钟信号 */

	ltdc_init.LTDC_BackgroundRedValue	= 0;  // 初始背景色，R	     	     
	ltdc_init.LTDC_BackgroundGreenValue = 0;  // 初始背景色，G	        
	ltdc_init.LTDC_BackgroundBlueValue 	= 0;  // 初始背景色，B
    
    ltdc_init.LTDC_HorizontalSync       = hsync_width - 1;
    ltdc_init.LTDC_VerticalSync         = vsync_width - 1;
    ltdc_init.LTDC_AccumulatedHBP       = hback_porch + hsync_width - 1;
    ltdc_init.LTDC_AccumulatedVBP       = vback_porch + vsync_width - 1;
    ltdc_init.LTDC_AccumulatedActiveW   = active_width + hsync_width + hback_porch - 1;
    ltdc_init.LTDC_AccumulatedActiveH   = active_height + vsync_width + vback_porch - 1;
    ltdc_init.LTDC_TotalWidth           = active_width + hsync_width + hback_porch + hfront_porch - 1;
    ltdc_init.LTDC_TotalHeigh           = active_height + vsync_width + vback_porch + vfront_porch - 1;

    LTDC_Init(&ltdc_init);
    LTDC_Cmd(ENABLE);
}

void STD_LTDC_LayerInit(void)
{
    LTDC_Layer_InitTypeDef ltdc_layer; 

    uint16_t active_width = panels->active_width;
    uint16_t active_height = panels->active_height;

    // uint32_t hsync_width = panels->hsync_width;
    uint32_t hback_porch = panels->hback_porch;
    // uint32_t hfront_porch = panels->hfront_porch;
    uint32_t vback_porch = panels->vback_porch;
    // uint32_t vsync_width = panels->vsync_width;
    // uint32_t vfront_porch = panels->vfront_porch;

    ltdc_layer.LTDC_HorizontalStart = hback_porch + 1;
    ltdc_layer.LTDC_HorizontalStop  = active_width + hback_porch;
    ltdc_layer.LTDC_VerticalStart   = vback_porch + 1;
    ltdc_layer.LTDC_VerticalStop    = active_height + vback_porch;
    ltdc_layer.LTDC_CFBLineNumber   = active_height;

    /* 像素格式设置 */
    ltdc_layer.LTDC_PixelFormat     = LCD_LAYER1_COLOR_MODE;

    /* 配置 layer1 的恒定透明度，最终写入 LTDC_LxCACR 寄存器  */
    /* 需要注意的是，这个参数是直接配置整个 layer1 的透明度，这里设置为255即不透明 */
    ltdc_layer.LTDC_ConstantAlpha    = 255;

    /* 设置 layer1 的层混合系数，最终写入 LTDC_LxBFCR 寄存器 */
    /* 该参数用于设置 layer1 和 底层背景 之间的颜色混合系数，计算公式为 ：
       混合后的颜色 =  BF1 * layer1的颜色 + BF2 * 底层背景的颜色 */
    /* 如果 layer1 使用了透明色，则必须配置成 
    LTDC_BLENDING_FACTOR1_PAxCA 和 LTDC_BLENDING_FACTOR2_PAxCA，
    否则ARGB中的A通道不起作用 */
    ltdc_layer.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
    ltdc_layer.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

    /* layer1 的显存地址，本例程使用外部的SDRAM作为显存，起始地址0xD0000000，SDRAM大小为32M */
    /* layer1 显存大小等于 = LCD_Width * LCD_Width * BytesPerPixel_1（每个像素所占字节大小）*/
    /* 因为 SDRAM 大小为32M，用户设置的区域一定不能超过这个值 */
    ltdc_layer.LTDC_CFBStartAdress = LCD_LAYER1_ADDRESS;

    /* 配置 layer1 的初始默认颜色，包括A,R,G,B 的值 ，最终写入 LTDC_LxDCCR 寄存器 */
    ltdc_layer.LTDC_DefaultColorAlpha   = 0;
    ltdc_layer.LTDC_DefaultColorRed     = 0;
    ltdc_layer.LTDC_DefaultColorGreen   = 0;
    ltdc_layer.LTDC_DefaultColorBlue    = 0;

#if (   LCD_LAYER1_COLOR_MODE == LCD_FORMAT_RGB565      ||\
        LCD_LAYER1_COLOR_MODE == LCD_FORMAT_ARGB1555    ||\
        LCD_LAYER1_COLOR_MODE == LCD_FORMAT_ARGB4444    )

        /* 每行的像素占的总字节数 */
        ltdc_layer.LTDC_CFBLineLength = active_width * 2 + 3;

        /* 行间距，某像素的起始处到下一行的起始处的增量 */
        ltdc_layer.LTDC_CFBPitch = active_width * 2;

#elif ( LCD_LAYER1_COLOR_MODE == LCD_FORMAT_RGB888      ||\
        LCD_LAYER1_COLOR_MODE == LCD_FORMAT_ARGB8888    )

        ltdc_layer.LTDC_CFBLineLength = active_width * 4 + 3;
        ltdc_layer.LTDC_CFBPitch = active_width * 2;
        LTDC_DitherCmd(ENABLE);
#else
        ltdc_layer.LTDC_CFBLineLength = active_width * 2 + 3;
        ltdc_layer.LTDC_CFBPitch = active_width * 2;
#endif

        LTDC_LayerInit(LTDC_Layer1, &ltdc_layer);       //初始化层1
        LTDC_LayerCmd(LTDC_Layer1, ENABLE);             //使能层1
        LTDC_ReloadConfig(LTDC_IMReload);               //重新载入参数

        layers[0].width = active_width;
        layers[0].height = active_height;
        layers[0].pixel_format = LCD_LAYER1_COLOR_MODE;
        layers[0].pixel_size = LCD_LAYER1_PIXEL_BYTE;
        layers[0].start_address = LCD_LAYER1_ADDRESS;
        layers[0].rotated = LCD_ROTATED;    

#if LCD_LAYER2_ENABLE

        /* 设置 layer2 的层混合系数，最终写入 LTDC_LxBFCR 寄存器 */
        /* 该参数用于设置 layer2 和 (layer0+背景）之间的颜色混合系数，计算公式为 ：
           混合后的颜色 =  BF1 * layer2的颜色 + BF2 * (layer0+背景混合后的颜色） */
        /* 如果 layer2 使用了透明色，则必须配置成 
        LTDC_BLENDING_FACTOR1_PAxCA 和 LTDC_BLENDING_FACTOR2_PAxCA，
        否则ARGB中的A通道不起作用 */
        ltdc_layer.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
        ltdc_layer.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

        /* layer2 的显存地址，本例程使用外部的SDRAM作为显存，起始地址0xD0000000，SDRAM大小为32M */
        /* 由于 layer1 会占用一部分显存，因此设置 layer2 显存时，需要进行一定偏移 */  
        ltdc_layer.LTDC_CFBStartAdress = LCD_LAYER2_ADDRESS;
        ltdc_layer.LTDC_PixelFormat = LCD_LAYER2_COLOR_MODE;

        #if (   LCD_LAYER2_COLOR_MODE == LCD_FORMAT_RGB565      ||\
                LCD_LAYER2_COLOR_MODE == LCD_FORMAT_ARGB1555    ||\
                LCD_LAYER2_COLOR_MODE == LCD_FORMAT_ARGB4444    )

                ltdc_layer.LTDC_CFBLineLength = LCD_WIDTH * 2 + 3;
                ltdc_layer.LTDC_CFBPitch = LCD_WIDTH  * 2;

        #elif ( LCD_LAYER2_COLOR_MODE == LCD_FORMAT_RGB888      ||\
                LCD_LAYER2_COLOR_MODE == LCD_FORMAT_ARGB8888    )

                ltdc_layer.LTDC_CFBLineLength = LCD_WIDTH * 4 + 3;
                ltdc_layer.LTDC_CFBPitch = LCD_WIDTH * 2;
                LTDC_DitherCmd(ENABLE);
        #else
                ltdc_layer.LTDC_CFBLineLength = LCD_WIDTH * 2 + 3;
                ltdc_layer.LTDC_CFBPitch = LCD_WIDTH  * 2;
        #endif

        LTDC_LayerInit(LTDC_Layer2, &ltdc_layer);   //初始化层2
        LTDC_LayerCmd(LTDC_Layer2, ENABLE);         //使能层2
        LTDC_ReloadConfig(LTDC_IMReload);           //重新载入参数

        layer_info[1].direction = LCD_DIR_HOR;
        layer_info[1].address = LCD_LAYER2_ADDRESS;
        layer_info[1].color_mode = LCD_LAYER2_COLOR_MODE;
        layer_info[1].pixel_byte = LCD_LAYER2_PIXEL_BYTE;

        layers[1].width = layers[0].width;
        layers[1].height = layers[0].height;
        layers[1].pixel_format = LCD_LAYER2_COLOR_MODE;
        layers[1].pixel_size = LCD_LAYER2_PIXEL_BYTE;
        layers[1].start_address = LCD_LAYER2_ADDRESS;
        layers[1].rotated = layers[0].rotated;

#endif

}





/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

