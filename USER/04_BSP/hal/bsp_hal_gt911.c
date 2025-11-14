/**
  ******************************************************************************
  *
  * @file    bsp_hal_gt911.c
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
#include "bsp_hal_gt911.h"
#include "gpio.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define GT911_SDA_PORT          GPIOH
#define GT911_SDA_PIN           GPIO_Pin_5

#define GT911_SCL_PORT          GPIOH
#define GT911_SCL_PIN           GPIO_Pin_4

#define GT911_INT_PORT          GPIOI
#define GT911_INT_PIN           GPIO_Pin_11

#define GT911_RST_PORT          GPIOC
#define GT911_RST_PIN           GPIO_Pin_13
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
void bsp_hal_gt911_si2c_init(void)
{
    /* touchpad */
    STD_GPIO_Init(GT911_SDA_PORT, GT911_SDA_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GT911_SCL_PORT, GT911_SCL_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_OD, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GT911_INT_PORT, GT911_INT_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
    STD_GPIO_Init(GT911_RST_PORT, GT911_RST_PIN, GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP, 0);
}

void bsp_hal_gt911_si2c_start(void)
{

}

void bsp_hal_gt911_si2c_stop(void)
{

}

uint8_t bsp_hal_gt911_si2c_wait_ack(void)
{

}

void bsp_hal_gt911_si2c_generate_ack(void)
{

}

void bsp_hal_gt911_si2c_generate_nack(void)
{

}

uint8_t bsp_hal_gt911_si2c_read_byte(void)
{

}

void bsp_hal_gt911_si2c_write_byte(uint8_t data)
{
    
}

void bsp_hal_gt911_write_int_pin(uint8_t new_state)
{

}

void bsp_hal_gt911_write_rst_pin(uint8_t new_state)
{

}

void bsp_hal_gt911_set_int_pin(uint8_t mode)
{

}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







