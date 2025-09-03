/**
  ******************************************************************************
  *
  * @file    bsp_port_led.h
  * @author  Jamin
  * @brief   Header file of bsp_port_led module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_PORT_LED_H
#define _BSP_PORT_LED_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "gpio.h"
/*********************
 *      DEFINES
 *********************/
#define BLUE_LED_PORT   GPIOD
#define BLUE_LED_PIN    GPIO_Pin_4

#define YELLOW_LED_PORT GPIOI
#define YELLOW_LED_PIN  GPIO_Pin_3
/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
static inline void bsp_port_led_init(void)
{

}

static inline void bsp_port_blue_led_on(void)
{
    GPIO_ResetBits(BLUE_LED_PORT, BLUE_LED_PIN);
}

static inline void bsp_port_blue_led_off(void)
{
    GPIO_SetBits(BLUE_LED_PORT, BLUE_LED_PIN);
}

static inline void bsp_port_yellow_led_on(void)
{
    GPIO_ResetBits(YELLOW_LED_PORT, YELLOW_LED_PIN);
}

static inline void bsp_port_yellow_led_off(void)
{
    GPIO_SetBits(YELLOW_LED_PORT, YELLOW_LED_PIN);
}

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_PORT_LED_H*/


/******************************* (END OF FILE) *********************************/

