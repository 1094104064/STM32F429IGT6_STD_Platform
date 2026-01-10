#include "bsp.h"



void bsp_tick_init(void)
{
    /* 1ms */
    SysTick_Config(SystemCoreClock / 1000);

    /* Enable the DWT cycle counter */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Reset the DWT cycle counter */
    DWT->CYCCNT = 0;

    /* Enable the cycle counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void bsp_tick_deinit(void)
{
    /* Disable SysTick */
    SysTick->CTRL = 0;

    /* Disable the DWT cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
}

void bsp_tick_delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t delay = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < delay);
}
