/**
  ******************************************************************************
  *
  * @file    usart.c
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
#include "usart.h"
#include "gpio.h"
#include "user_conf.h"
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
void STD_USART1_Init(void)
{
    STD_GPIO_Init(GPIOA, GPIO_Pin_9, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_USART1);
    STD_GPIO_Init(GPIOA, GPIO_Pin_10, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_USART1);

    USART_InitTypeDef USART_InitStructure = {0};

    /*!< Enable the Usart1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
    
    /*!< USART_InitStructure members configuration*/
    USART_InitStructure.USART_BaudRate 	          = 230400;                         /**< Setup by the user*/
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            /**< Data length 8 bits*/
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               /**< 1 stop bit*/
    USART_InitStructure.USART_Parity              = USART_Parity_No ;               /**< No parity check*/
    USART_InitStructure.USART_Mode 	              = USART_Mode_Rx | USART_Mode_Tx;  /**< Full duplex communication*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /**< No hardware flow control*/
    USART_Init(USART1, &USART_InitStructure); 

    /*!< Enable Usart1 */
    USART_Cmd(USART1, ENABLE);

    printf("\r\n\r\n--------Build date: %s %s-----v%s--------\r\n", __DATE__, __TIME__, PLATFORM_VERSION);
    printf("Usart1 for Debug Initialized\r\nSystemCoreClock: %dMHz\r\n", SystemCoreClock / 1000000);
}

void STD_USART1_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    /*!< NVIC_InitStructure members configuration */
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;     /**< Enable Usart1 interrupt Channel*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               /**< PreemptionPriority 3*/
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;               /**< SubPriority 1*/
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;          /**< Enable interrupt*/
    NVIC_Init(&NVIC_InitStructure); 
    
    /*!< Enable the receive disrupt of Usart1 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
}

/**********************
 *   STATIC FUNCTIONS
 **********************/



 #if defined(__CC_ARM) && (__ARMCC_VERSION < 6000000)

    #pragma import(__use_no_semihosting)
    typedef struct __FILE FILE;

    struct __FILE
    {
        int handle;
    };

    FILE __stdout;

    void _sys_exit(int x)
    {
        x = x;
    }
 
#endif



#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)

    __asm (".global __use_no_semihosting\n\t");
    void _sys_exit(int x) 
    {
        x = x;
    }

    void _ttywrch(int ch)
    {
        ch = ch;
    }

    FILE __stdout;
 
#endif


/**
  * @brief  redirect fputc function to use printf.
  * @param  c: character
  * @param  fp: file pointer
  * @retval Returns a character if normal, or EOF if there is an error.
  */
int fputc(int c, FILE *fp)
{
    /*!< Send single byte */
    USART_SendData(USART1, (uint8_t)c);	
    
    /*!< Waiting for sending to complete */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	

    return (c); 
}



/******************************* (END OF FILE) *********************************/

