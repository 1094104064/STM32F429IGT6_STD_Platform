#include "bsp.h"


#define DEBUG_TX_PORT   GPIOA
#define DEBUG_TX_PIN    GPIO_Pin_9

#define DEBUG_RX_PORT   GPIOA
#define DEBUG_RX_PIN    GPIO_Pin_10

#define DEBUG_BAUDRATE  230400


void bsp_debug_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin       = DEBUG_TX_PIN | DEBUG_RX_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed     = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType     = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd      = GPIO_PuPd_UP;
    GPIO_Init(DEBUG_TX_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(DEBUG_TX_PORT, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(DEBUG_RX_PORT, GPIO_PinSource10, GPIO_AF_USART1);
 
    USART_InitTypeDef USART_InitStructure = {0};

    /*!< Enable the Usart1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
    
    /*!< USART_InitStructure members configuration*/
    USART_InitStructure.USART_BaudRate 	          = DEBUG_BAUDRATE;                 /**< Setup by the user*/
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            /**< Data length 8 bits*/
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               /**< 1 stop bit*/
    USART_InitStructure.USART_Parity              = USART_Parity_No ;               /**< No parity check*/
    USART_InitStructure.USART_Mode 	              = USART_Mode_Rx | USART_Mode_Tx;  /**< Full duplex communication*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /**< No hardware flow control*/
    USART_Init(USART1, &USART_InitStructure); 

    NVIC_InitTypeDef NVIC_InitStructure = {0};
    
    /*!< Enable Usart1 */
    USART_Cmd(USART1, ENABLE);
    
    /*!< NVIC_InitStructure members configuration */
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;     /**< Enable Usart1 interrupt Channel*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               /**< PreemptionPriority 3*/
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;               /**< SubPriority 1*/
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;          /**< Enable interrupt*/
    NVIC_Init(&NVIC_InitStructure); 
    
    /*!< Enable the receive disrupt of Usart1 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void bsp_debug_putc(char c)
{
    /*!< Send a char */
    USART_SendData(USART1, (uint8_t)c);
    
    /*!< Wait until the char has been sent */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}



