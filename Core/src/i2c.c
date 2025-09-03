/**
  ******************************************************************************
  *
  * @file    i2c.c
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
#include "i2c.h"
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

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void STD_I2C1_Init(void)
{
    GPIO_InitTypeDef gpio_bmp180[] = {
        {.GPIO_Pin = BMP180_SDA_PIN, .GPIO_Mode = GPIO_Mode_AF, .GPIO_Speed = GPIO_Speed_50MHz, .GPIO_OType = GPIO_OType_OD, .GPIO_PuPd = GPIO_PuPd_NOPULL},
        {.GPIO_Pin = BMP180_SCL_PIN, .GPIO_Mode = GPIO_Mode_AF, .GPIO_Speed = GPIO_Speed_50MHz, .GPIO_OType = GPIO_OType_OD, .GPIO_PuPd = GPIO_PuPd_NOPULL}
    };

    for(uint32_t i = 0; i < sizeof(gpio_bmp180) / sizeof(gpio_bmp180[0]); i++)
        STD_GPIO_Init(  GPIOB, gpio_bmp180[i].GPIO_Pin, gpio_bmp180[i].GPIO_Mode, 
                        gpio_bmp180[i].GPIO_Speed, gpio_bmp180[i].GPIO_OType, 
                        gpio_bmp180[i].GPIO_PuPd, GPIO_AF_I2C1 );

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_InitTypeDef I2c_InitStructure = {0};

    I2c_InitStructure.I2C_Mode                  = I2C_Mode_I2C;
    I2c_InitStructure.I2C_ClockSpeed            = 100000;
    I2c_InitStructure.I2C_DutyCycle             = I2C_DutyCycle_16_9;
    I2c_InitStructure.I2C_Ack                   = I2C_Ack_Enable;
    I2c_InitStructure.I2C_AcknowledgedAddress   = I2C_AcknowledgedAddress_7bit;
    I2c_InitStructure.I2C_OwnAddress1           = 0x00;
    I2C_Init(I2C1, &I2c_InitStructure);

    I2C_Cmd(I2C1, ENABLE);
}


void STD_I2C1_Start(void)
{
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

void STD_I2C1_Stop(void)
{
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void STD_I2C1_SendDevAddr(uint8_t dev_addr, uint8_t rw)
{
    if(rw == 1) {
        dev_addr |= 0x01;
    }
    else {
        dev_addr &= 0xfe;
    }

    I2C_Send7bitAddress(I2C1, dev_addr, rw);
    if(rw == 1) {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
    else {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
}

void STD_I2C1_WriteByte(uint8_t byte)
{
    I2C_SendData(I2C1, byte);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t STD_I2C1_ReadByte(void)
{
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    return I2C_ReceiveData(I2C1);
}

void STD_I2C1_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t * src, uint16_t len)
{
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, dev_addr, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, reg_addr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for(uint32_t i = 0; i < len; i++) {

        I2C_SendData(I2C1, *src);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

        src++;
    }

    I2C_GenerateSTOP(I2C1, ENABLE);
}

void STD_I2C1_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t * dst, uint16_t len)
{
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, dev_addr, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, reg_addr);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, dev_addr + 1, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    for(uint32_t i = 0; i < len; i++) {

        if(i == len - 1) {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }

        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *dst = I2C_ReceiveData(I2C1);
        dst++;
    }

    I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
