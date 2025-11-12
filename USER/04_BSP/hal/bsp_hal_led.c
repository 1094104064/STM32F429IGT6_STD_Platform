/**
  ******************************************************************************
  *
  * @file    bsp_hal_led.c
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
#include "bsp_hal_led.h"
#include "gpio.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define BLUE_LED_PORT   GPIOD
#define BLUE_LED_PIN    GPIO_Pin_4

#define YELLOW_LED_PORT GPIOI
#define YELLOW_LED_PIN  GPIO_Pin_3
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
void bsp_hal_led1_init(void)
{
    STD_GPIO_Init(BLUE_LED_PORT, BLUE_LED_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
}

void bsp_hal_led1_on(void)
{
    GPIO_ResetBits(BLUE_LED_PORT, BLUE_LED_PIN);
}

void bsp_hal_led1_off(void)
{
    GPIO_SetBits(BLUE_LED_PORT, BLUE_LED_PIN);
}

void bsp_hal_led2_init(void)
{
    STD_GPIO_Init(YELLOW_LED_PORT, YELLOW_LED_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
}

void bsp_hal_led2_on(void)
{
    GPIO_ResetBits(YELLOW_LED_PORT, YELLOW_LED_PIN);
}

void bsp_hal_led2_off(void)
{
    GPIO_SetBits(YELLOW_LED_PORT, YELLOW_LED_PIN);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/



/******************************* (END OF FILE) *********************************/



 







