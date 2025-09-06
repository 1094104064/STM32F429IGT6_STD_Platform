/***
	***************************************************************************
	*	@file  	stm32f4xx_it.c
	*	@brief  中断服务函数
   ***************************************************************************
   *  @description
	*
	*  此文件存放了STM32所有的异常操作和中断服务函数
	* 	
	***************************************************************************
***/
#include "stm32f4xx_it.h"
#include "main.h"




/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/



/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    printf("%s\r\n", __FUNCTION__);
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    printf("%s\r\n", __FUNCTION__);
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    printf("%s\r\n", __FUNCTION__);
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    printf("%s\r\n", __FUNCTION__);
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

#if 0
/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}
#endif

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

#if 0
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick exception.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    STD_SYSTICK_Inc(1);

#if OS_ENABLE
extern void xPortSysTickHandler(void);
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) 
        xPortSysTickHandler();
#endif

#if LVGL_ENABLE
    lv_tick_inc(1);
#endif
}

/**
  * @brief  Usart1 receive interrupt processing.
  * @param  None
  * @retval NULL
  */
void USART1_IRQHandler(void)
{
    /*!< Interrupt Response */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {	

    }
}


void EXTI9_5_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line8);
    // LOG_INFO("Key press");
}


void TIM7_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    
//    core_tim7_irq_handler();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
