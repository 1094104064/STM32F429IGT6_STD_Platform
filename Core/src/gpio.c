/**
  ******************************************************************************
  *
  * @file    gpio.c
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
#include "gpio.h"
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
static void get_periph_clock(GPIO_TypeDef * port, uint32_t * periph_clk);
static void get_pin_source(uint32_t pin, uint16_t * pin_src);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void STD_GPIO_Init( GPIO_TypeDef * port, 
                    uint32_t pin, 
                    GPIOMode_TypeDef mode, 
                    GPIOSpeed_TypeDef speed, 
                    GPIOOType_TypeDef otype, 
                    GPIOPuPd_TypeDef pupd, 
                    uint8_t af  )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    uint32_t periph_clk = 0;

    get_periph_clock(port, &periph_clk);

    RCC_AHB1PeriphClockCmd(periph_clk, ENABLE);

    /*!< GPIO_InitStructure members configuration*/
    GPIO_InitStructure.GPIO_Pin   = pin;               /**< Setup by the user*/
    GPIO_InitStructure.GPIO_Mode  = mode;              /**< Setup by the user*/
    GPIO_InitStructure.GPIO_Speed = speed;             /**< Setup by the user*/
    GPIO_InitStructure.GPIO_OType = otype;
    GPIO_InitStructure.GPIO_PuPd  = pupd;              /**< Setup by the user*/
    
    /*!< GPIO Initialization */
    GPIO_Init(port, &GPIO_InitStructure);   
 
    if(mode == GPIO_Mode_AF) {
        uint16_t pin_source = 0;

        get_pin_source(pin, &pin_source);

        GPIO_PinAFConfig(port, pin_source, af);
    }
}

void STD_GPIO_OutputMode(GPIO_TypeDef * port, uint32_t pin)
{
    STD_GPIO_Init(port, pin, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
}

void STD_GPIO_InputMode(GPIO_TypeDef * port, uint32_t pin)
{
    STD_GPIO_Init(port, pin, GPIO_Mode_IN, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
}

void STD_GPIO_BspInit(void)
{
    /* Configure the GPIO pins for the Bsp */


    
    /* Software I2C */
    STD_GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);

    /* Test Pin */
    STD_GPIO_Init(TEST_POINT_PORT, TEST_POINT_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);


    /* touchpad */
    STD_GPIO_Init(GT911_SDA_PORT, GT911_SDA_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GT911_SCL_PORT, GT911_SCL_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GT911_INT_PORT, GT911_INT_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GT911_RST_PORT, GT911_RST_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);


    /* ST7735 Ctrl */
    STD_GPIO_Init(ST7735_DC_PORT , ST7735_DC_PIN , GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(ST7735_RST_PORT, ST7735_RST_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(ST7735_BL_PORT , ST7735_BL_PIN , GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void get_periph_clock(GPIO_TypeDef * port, uint32_t * periph_clk)
{
    if(port == GPIOA) {
        *periph_clk = RCC_AHB1Periph_GPIOA;
    }
    else if(port == GPIOB) {
        *periph_clk = RCC_AHB1Periph_GPIOB;
    }
    else if(port == GPIOC) {
        *periph_clk = RCC_AHB1Periph_GPIOC;
    }
    else if(port == GPIOD) {
        *periph_clk = RCC_AHB1Periph_GPIOD;
    }
    else if(port == GPIOE) {
        *periph_clk = RCC_AHB1Periph_GPIOE;
    }
    else if(port == GPIOF) {
        *periph_clk = RCC_AHB1Periph_GPIOF;
    }
    else if(port == GPIOG) {
        *periph_clk = RCC_AHB1Periph_GPIOG;
    }
    else if(port == GPIOH) {
        *periph_clk = RCC_AHB1Periph_GPIOH;
    }
    else if(port == GPIOI) {
        *periph_clk = RCC_AHB1Periph_GPIOI;
    }
    else if(port == GPIOJ) {
        *periph_clk = RCC_AHB1Periph_GPIOJ;
    }
    else if(port == GPIOK) {
        *periph_clk = RCC_AHB1Periph_GPIOK;
    }
}

static void get_pin_source(uint32_t pin, uint16_t * pin_src)
{
    if(pin == GPIO_Pin_0){
        *pin_src = GPIO_PinSource0;
    }
    else if(pin == GPIO_Pin_1){
        *pin_src = GPIO_PinSource1;
    }
    else if(pin == GPIO_Pin_2){
        *pin_src = GPIO_PinSource2;
    }
    else if(pin == GPIO_Pin_3){
        *pin_src = GPIO_PinSource3;
    }
    else if(pin == GPIO_Pin_4){
        *pin_src = GPIO_PinSource4;
    }
    else if(pin == GPIO_Pin_5){
        *pin_src = GPIO_PinSource5;
    }
    else if(pin == GPIO_Pin_6){
        *pin_src = GPIO_PinSource6;
    }
    else if(pin == GPIO_Pin_7){
        *pin_src = GPIO_PinSource7;
    }
    else if(pin == GPIO_Pin_8){
        *pin_src = GPIO_PinSource8;
    }
    else if(pin == GPIO_Pin_9){
        *pin_src = GPIO_PinSource9;
    }
    else if(pin == GPIO_Pin_10){
        *pin_src = GPIO_PinSource10;
    }
    else if(pin == GPIO_Pin_11){
        *pin_src = GPIO_PinSource11;
    }
    else if(pin == GPIO_Pin_12){
        *pin_src = GPIO_PinSource12;
    }
    else if(pin == GPIO_Pin_13){
        *pin_src = GPIO_PinSource13;
    }
    else if(pin == GPIO_Pin_14){
        *pin_src = GPIO_PinSource14;
    }
    else if(pin == GPIO_Pin_15){
        *pin_src = GPIO_PinSource15;
    }
}

/******************************* (END OF FILE) *********************************/

