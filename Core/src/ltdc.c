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
#include "bsp_driver_fb.h"
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
static bool ltdc_initialized = false;

static uint16_t active_width;
static uint16_t active_height;

static uint32_t hsync_width;
static uint32_t hback_porch;
static uint32_t hfront_porch;
static uint32_t vback_porch;
static uint32_t vsync_width;
static uint32_t vfront_porch;

static uint8_t pixel_format;
static uint8_t pixel_size;
static uint8_t rotated;
static uint32_t start_address;

static LTDC_Layer_TypeDef * layer = LTDC_Layer1;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void STD_LTDC_TimingInit(uint32_t hsw, uint32_t hbp, uint32_t hfp, uint32_t vsw, uint32_t vbp, uint32_t vfp)
{
    hsync_width = hsw;
    hback_porch = hbp;
    hfront_porch = hfp;
    vsync_width = vsw;
    vback_porch = vbp;
    vfront_porch = vfp;
}

void STD_LTDC_ResolutionInit(uint16_t width, uint16_t height)
{
    active_width = width;
    active_height = height;
}

void STD_LTDC_PixelInfoExport(uint8_t * p_format, uint8_t * p_size)
{
    if(p_format) *p_format = pixel_format;
    if(p_size)   *p_size = pixel_size;
}


void STD_LTDC_SetARGB8888(void)
{
    pixel_format = LTDC_Pixelformat_ARGB8888;
    pixel_size = 4;
}

void STD_LTDC_SetRGB888(void)
{
    pixel_format = LTDC_Pixelformat_RGB888;
    pixel_size = 3;
}

void STD_LTDC_SetRGB565(void)
{
    pixel_format = LTDC_Pixelformat_RGB565;
    pixel_size = 2;
}

void STD_LTDC_SetARGB1555(void)
{
    pixel_format = LTDC_Pixelformat_ARGB1555;
    pixel_size = 2;
}

void STD_LTDC_SetARGB4444(void)
{
    pixel_format = LTDC_Pixelformat_ARGB4444;
    pixel_size = 2;
}

void STD_LTDC_SetL8(void)
{
    pixel_format = LTDC_Pixelformat_L8;
    pixel_size = 1;
}

void STD_LTDC_SetAL44(void)
{
    pixel_format = LTDC_Pixelformat_AL44;
    pixel_size = 2;
}

void STD_LTDC_SetAL88(void)
{
    pixel_format = LTDC_Pixelformat_AL88;
    pixel_size = 2;
}

void STD_LTDC_SetLayer0(uint32_t address)
{
    layer = LTDC_Layer1;
    start_address = address;
}

void STD_LTDC_SetLayer1(uint32_t address)
{
    layer = LTDC_Layer2;
    start_address = address;
}

void STD_LTDC_Init(void)
{
    if(ltdc_initialized) {
        return;
    }

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

    LTDC_InitTypeDef  ltdc_init = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE); 	

    LCD_PLLSAIN = LCD_CLK * LCD_PLLSAIR * LCD_CLKDIV;       /* 根据需要使用的LCD时钟计算PLLSAIN参数，可取范围为50~432 */
    RCC_PLLSAIConfig(LCD_PLLSAIN, 7, LCD_PLLSAIR);        
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);              /* LCD时钟分频设置，要和LCD_CLKDIV对应 */
    RCC_PLLSAICmd(ENABLE);                                  /* 使能PLLSAI时钟 */ 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);	/* 等待时钟配置完成 */


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

void STD_LTDC_LayerInit(LTDC_Layer_TypeDef * layerx, uint8_t pixel_format, uint32_t start_address)
{
    LTDC_Layer_InitTypeDef ltdc_layer = {0}; 

    ltdc_layer.LTDC_HorizontalStart = hback_porch + 1;
    ltdc_layer.LTDC_HorizontalStop  = active_width + hback_porch;
    ltdc_layer.LTDC_VerticalStart   = vback_porch + 1;
    ltdc_layer.LTDC_VerticalStop    = active_height + vback_porch;
    ltdc_layer.LTDC_CFBLineNumber   = active_height;

    /* 像素格式设置 */
    ltdc_layer.LTDC_PixelFormat     = pixel_format;

    /* 配置 layer1 的恒定透明度，最终写入 LTDC_LxCACR 寄存器  */
    /* 需要注意的是，这个参数是直接配置整个 layer1 的透明度，这里设置为255即不透明 */
    ltdc_layer.LTDC_ConstantAlpha    = 255;

    /* 设置 layer1 的层混合系数，最终写入 LTDC_LxBFCR 寄存器 */
    /* 该参数用于设置 layer1 和 底层背景 之间的颜色混合系数，计算公式为 ：
       混合后的颜色 =  BF1 * layer1的颜色 + BF2 * 底层背景的颜色 */
    /* 如果 layer1 使用了透明色，则必须配置成 
    LTDC_BLENDING_FACTOR1_PAxCA 和 LTDC_BLENDING_FACTOR2_PAxCA，
    否则ARGB中的A通道不起作用 */
    if( pixel_format == LTDC_Pixelformat_ARGB8888   || 
        pixel_format == LTDC_Pixelformat_ARGB1555   ||
        pixel_format == LTDC_Pixelformat_ARGB4444 )
    {
        ltdc_layer.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
        ltdc_layer.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
    }
    else
    {
        ltdc_layer.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
        ltdc_layer.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
    }


    /* layer1 的显存地址，本例程使用外部的SDRAM作为显存，起始地址0xD0000000，SDRAM大小为32M */
    /* layer1 显存大小等于 = LCD_Width * LCD_Width * BytesPerPixel_1（每个像素所占字节大小）*/
    /* 因为 SDRAM 大小为32M，用户设置的区域一定不能超过这个值 */
    ltdc_layer.LTDC_CFBStartAdress = start_address;

    /* 配置 layer1 的初始默认颜色，包括A,R,G,B 的值 ，最终写入 LTDC_LxDCCR 寄存器 */
    ltdc_layer.LTDC_DefaultColorAlpha   = 0;
    ltdc_layer.LTDC_DefaultColorRed     = 0;
    ltdc_layer.LTDC_DefaultColorGreen   = 0;
    ltdc_layer.LTDC_DefaultColorBlue    = 0;

    if( pixel_format == LTDC_Pixelformat_RGB565     || 
        pixel_format == LTDC_Pixelformat_ARGB1555   ||
        pixel_format == LTDC_Pixelformat_ARGB4444 )
    {
        /* 每行的像素占的总字节数 */
        ltdc_layer.LTDC_CFBLineLength = active_width * 2 + 3;

        /* 行间距，某像素的起始处到下一行的起始处的增量 */
        ltdc_layer.LTDC_CFBPitch = active_width * 2;
    }
    else if ( pixel_format == LTDC_Pixelformat_RGB888   || 
              pixel_format == LTDC_Pixelformat_ARGB8888 )
    {
        ltdc_layer.LTDC_CFBLineLength = active_width * 4 + 3;
        ltdc_layer.LTDC_CFBPitch = active_width * 4;
        LTDC_DitherCmd(ENABLE);
    }
    else if ( pixel_format == LTDC_Pixelformat_L8 )
    {
        ltdc_layer.LTDC_CFBLineLength = active_width + 3;
        ltdc_layer.LTDC_CFBPitch = active_width;
    }
    else if ( pixel_format == LTDC_Pixelformat_AL44 )
    {
        ltdc_layer.LTDC_CFBLineLength = active_width * 2 + 3;
        ltdc_layer.LTDC_CFBPitch = active_width * 2;
    }
    else if ( pixel_format == LTDC_Pixelformat_AL88 )
    {
        ltdc_layer.LTDC_CFBLineLength = active_width * 2 + 3;
        ltdc_layer.LTDC_CFBPitch = active_width * 2;
    }
    else
    {
        /* 默认按 RGB565 来设置 */
        ltdc_layer.LTDC_CFBLineLength = active_width * 2 + 3;
        ltdc_layer.LTDC_CFBPitch = active_width * 2;
    }

    LTDC_LayerInit(layer, &ltdc_layer);       
    LTDC_LayerCmd(layer, ENABLE);             
    LTDC_ReloadConfig(LTDC_IMReload);              

}





/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

