/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  * Architectural Principle: This file serves solely as a hardware interrupt 
  * entry point and does not contain specific business logic.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private function prototypes -----------------------------------------------*/

__weak void HAL_SysTick_Callback(void) {}
__weak void HAL_USART1_RxCpltCallback(void) {}

/******************************************************************************/
/* Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/* @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    for(;;);
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    for(;;);
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    for(;;);
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    for(;;);
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    for(;;);
}

/**
  * @brief This function handles System service call via SWI instruction.
  * @note  If you are using FreeRTOS, 
  *        this function is typically taken over by the OS and should be commented out.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  * @note  If you are using FreeRTOS, 
  *        this function is typically taken over by the OS and should be commented out.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  * @note  
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        /*!< Clear the USART1 RXNE interrupt flag */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        
        HAL_USART1_RxCpltCallback();
    }
}
