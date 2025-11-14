/**
  ******************************************************************************
  *
  * @file    gpio.h
  * @author  Jamin
  * @brief   Header file of gpio module.
  *
  ******************************************************************************
  **/
  
#ifndef _GPIO_H
#define _GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "stm32f4xx.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void STD_GPIO_Init( GPIO_TypeDef * port, 
                    uint32_t pin, 
                    GPIOMode_TypeDef mode, 
                    GPIOSpeed_TypeDef speed, 
                    GPIOOType_TypeDef otype, 
                    GPIOPuPd_TypeDef pupd, 
                    uint8_t af  );
void STD_GPIO_OutputMode(GPIO_TypeDef * port, uint32_t pin);
void STD_GPIO_InputMode(GPIO_TypeDef * port, uint32_t pin);
void STD_GPIO_BspInit(void);
/**********************
 *      MACROS
 **********************/

#define TEST_POINT_PORT         GPIOA
#define TEST_POINT_PIN          GPIO_Pin_12



#define SENSOR_SDA_PORT         GPIOB
#define SENSOR_SDA_PIN          GPIO_Pin_7
#define SENSOR_SCL_PORT         GPIOB
#define SENSOR_SCL_PIN          GPIO_Pin_6

#define ST7735_DC_PORT          GPIOD
#define ST7735_DC_PIN           GPIO_Pin_5

#define ST7735_RST_PORT         GPIOD
#define ST7735_RST_PIN          GPIO_Pin_6

#define ST7735_BL_PORT          GPIOD
#define ST7735_BL_PIN           GPIO_Pin_7




#ifdef __cplusplus
}
#endif

#endif /*_GPIO_H*/


/******************************* (END OF FILE) *********************************/

