/**
  ******************************************************************************
  *
  * @file    fmc.c
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
#include "fmc.h"
#include "gpio.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)   
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
void STD_FMC_SDRAM_Init(void)
{
    /* address lines */
    STD_GPIO_Init(FMC_A0_PORT,  FMC_A0_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A0_AF);
    STD_GPIO_Init(FMC_A1_PORT,  FMC_A1_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A1_AF);
    STD_GPIO_Init(FMC_A2_PORT,  FMC_A2_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A2_AF);
    STD_GPIO_Init(FMC_A3_PORT,  FMC_A3_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A3_AF);
    STD_GPIO_Init(FMC_A4_PORT,  FMC_A4_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A4_AF);
    STD_GPIO_Init(FMC_A5_PORT,  FMC_A5_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A5_AF);
    STD_GPIO_Init(FMC_A6_PORT,  FMC_A6_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A6_AF);
    STD_GPIO_Init(FMC_A7_PORT,  FMC_A7_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A7_AF);
    STD_GPIO_Init(FMC_A8_PORT,  FMC_A8_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A8_AF);
    STD_GPIO_Init(FMC_A9_PORT,  FMC_A9_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A9_AF);
    STD_GPIO_Init(FMC_A10_PORT, FMC_A10_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A10_AF);
    STD_GPIO_Init(FMC_A11_PORT, FMC_A11_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A11_AF);
    STD_GPIO_Init(FMC_A12_PORT, FMC_A12_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_A12_AF);

    /* data lines */
    STD_GPIO_Init(FMC_D0_PORT,  FMC_D0_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D0_AF);
    STD_GPIO_Init(FMC_D1_PORT,  FMC_D1_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D1_AF);
    STD_GPIO_Init(FMC_D2_PORT,  FMC_D2_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D2_AF);
    STD_GPIO_Init(FMC_D3_PORT,  FMC_D3_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D3_AF);
    STD_GPIO_Init(FMC_D4_PORT,  FMC_D4_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D4_AF);
    STD_GPIO_Init(FMC_D5_PORT,  FMC_D5_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D5_AF);
    STD_GPIO_Init(FMC_D6_PORT,  FMC_D6_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D6_AF);
    STD_GPIO_Init(FMC_D7_PORT,  FMC_D7_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D7_AF);
    STD_GPIO_Init(FMC_D8_PORT,  FMC_D8_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D8_AF);
    STD_GPIO_Init(FMC_D9_PORT,  FMC_D9_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D9_AF);
    STD_GPIO_Init(FMC_D10_PORT, FMC_D10_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D10_AF);
    STD_GPIO_Init(FMC_D11_PORT, FMC_D11_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D11_AF);
    STD_GPIO_Init(FMC_D12_PORT, FMC_D12_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D12_AF);
    STD_GPIO_Init(FMC_D13_PORT, FMC_D13_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D13_AF);
    STD_GPIO_Init(FMC_D14_PORT, FMC_D14_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D14_AF);
    STD_GPIO_Init(FMC_D15_PORT, FMC_D15_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_D15_AF);

    /* control signals */
    STD_GPIO_Init(FMC_CS_PORT,   FMC_CS_PIN,   GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_CS_AF);
    STD_GPIO_Init(FMC_BA0_PORT,  FMC_BA0_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_BA0_AF);
    STD_GPIO_Init(FMC_BA1_PORT,  FMC_BA1_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_BA1_AF);
    STD_GPIO_Init(FMC_WE_PORT,   FMC_WE_PIN,   GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_WE_AF);
    STD_GPIO_Init(FMC_RAS_PORT,  FMC_RAS_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_RAS_AF);
    STD_GPIO_Init(FMC_CAS_PORT,  FMC_CAS_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_CAS_AF);
    STD_GPIO_Init(FMC_CLK_PORT,  FMC_CLK_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_CLK_AF);
    STD_GPIO_Init(FMC_CKE_PORT,  FMC_CKE_PIN,  GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_CKE_AF);
    STD_GPIO_Init(FMC_UDQM_PORT, FMC_UDQM_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_UDQM_AF);
    STD_GPIO_Init(FMC_LDQM_PORT, FMC_LDQM_PIN, GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL, FMC_LDQM_AF);


    FMC_SDRAMInitTypeDef  FMC_SDRAMInitStructure = {0};
    FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure = {0}; 

    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);	// 开启FMC时钟

    FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay    = 2;  // TMRD: min=12ns (2x11.11ns)  
    FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 7;  // TXSR: min=72ns (7x11.11ns)  
    FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime      = 4;  // TRAS: min=42ns (4x11.11ns)    
    FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay        = 7;  // TRC:  min=72ns (7x11.11ns)   
    FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime    = 2;  // TWR:  2 Tck 
    FMC_SDRAMTimingInitStructure.FMC_RPDelay              = 2;  // TRP:  18ns => 2x11.11ns             
    FMC_SDRAMTimingInitStructure.FMC_RCDDelay             = 2;  // TRCD: 12ns => 2x11.11ns 

    // 标准库配置FMC BAMK2 的 SDRAM时，没有配置 TRCD 参数，会导致SDRAM工作不正常
    // 因为ST已经不对标准库进行维护了，所以这个问题一直存在
    // 解决方法是，将TRCD参数和TWR参数进行合并，然后调用库函数对TWR进行配置的时候，会一并配置TRCD
    FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = (FMC_SDRAMTimingInitStructure.FMC_RCDDelay-1)<<8 | FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime ;


    FMC_SDRAMInitStructure.FMC_Bank                 = FMC_BANK_SDRAM; 					// 选择BANK区
    FMC_SDRAMInitStructure.FMC_ColumnBitsNumber     = FMC_ColumnBits_Number_9b; 		// 行地址宽度
    FMC_SDRAMInitStructure.FMC_RowBitsNumber        = FMC_RowBits_Number_13b;			// 列地址线宽度
    FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth    = FMC_SDMemory_Width_16b; 			// 数据宽度  
    FMC_SDRAMInitStructure.FMC_InternalBankNumber   = FMC_InternalBank_Number_4;		// bank数量
    FMC_SDRAMInitStructure.FMC_CASLatency           = FMC_CAS_Latency_3; 				//	CAS 
    FMC_SDRAMInitStructure.FMC_WriteProtection      = FMC_Write_Protection_Disable; 	// 禁止写保护
    FMC_SDRAMInitStructure.FMC_SDClockPeriod        = FMC_SDClock_Period_2; 			// 分频
    FMC_SDRAMInitStructure.FMC_ReadBurst            = FMC_Read_Burst_Enable;			// 突发模式  
    FMC_SDRAMInitStructure.FMC_ReadPipeDelay        = FMC_ReadPipe_Delay_1; 			// 读延迟
    FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct    = &FMC_SDRAMTimingInitStructure;

    FMC_SDRAMInit(&FMC_SDRAMInitStructure); 	// 初始化FMC接口


	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
	u32 Reg = 0;	// 定义寄存器变量

	FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_CLK_Enabled;	// 开启SDRAM时钟 
	FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_COMMAND_TARGET_BANK;			// 选择要控制的区域
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);  //等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);  						 // 配置SDRAM
	STD_SYSTICK_Delay(5); //延时5ms 
	 

	FMC_SDRAMCommandStructure.FMC_CommandMode               = FMC_Command_Mode_PALL;			// 预充电命令
	FMC_SDRAMCommandStructure.FMC_CommandTarget             = FMC_COMMAND_TARGET_BANK;		// 选择要控制的区域
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber         = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition    = 0;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);							// 配置SDRAM
	STD_SYSTICK_Delay(5); //延时5ms 
	
	//自动刷新配置 
	FMC_SDRAMCommandStructure.FMC_CommandMode               = FMC_Command_Mode_AutoRefresh; // 使用自动刷新
	FMC_SDRAMCommandStructure.FMC_CommandTarget             = FMC_COMMAND_TARGET_BANK; 		 // 选择要控制的区域
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber         = 4;							// 自动刷新次数
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition    = 0;    

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);							// 配置SDRAM

	//寄存器配置 
	Reg  = (u32)(SDRAM_MODEREG_BURST_LENGTH_2 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE);

	FMC_SDRAMCommandStructure.FMC_CommandMode               = FMC_Command_Mode_LoadMode;	// 加载模式寄存器命令
	FMC_SDRAMCommandStructure.FMC_CommandTarget             = FMC_COMMAND_TARGET_BANK;		// 选择要控制的区域
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber         = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition    = Reg;									// 寄存器值

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);							// 配置SDRAM

	FMC_SetRefreshCount(1386);	// 设置刷新计数器 
	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 

    memset((uint8_t *)SDRAM_BASE_ADDR, 0, SDRAM_SIZE);
}

int8_t STD_FMC_SDRAM_Benchmark(void)
{
    uint16_t read_data_16b = 0; 	    
    uint8_t  read_data_8b = 0;
    uint32_t i = 0;
    uint32_t size = SDRAM_SIZE;
    uint32_t addr = SDRAM_BASE_ADDR;

    for (i = 0; i < size / 2; i++) {
        *(volatile uint16_t *) (addr + 2 * i) = (uint16_t)i;		
    }

    for(i = 0; i < size / 2; i++) {
        read_data_16b = *(volatile uint16_t*)(addr + 2 * i );       
        if(read_data_16b != (uint16_t)i) {                       
            return -1;
        }
    }

    for (i = 0; i < 255; i++) {
        *(volatile uint8_t *)(addr + i) = (uint8_t)i;
    }	

    for (i = 0; i < 255; i++) {
        read_data_8b = *(volatile uint8_t*)(addr + i);
        if(read_data_8b != (uint8_t)i) {                      
            return -2;	                                
        }
    }

    memset((uint8_t *)SDRAM_BASE_ADDR, 0, SDRAM_SIZE);

    return 0;
}

void STD_FMC_SDRAM_WriteSpeedTest(void)
{
#if 0
    uint32_t start, end;
    uint32_t * pBuf;
    uint32_t i, j;

    pBuf = (uint32_t *)SDRAM_BASE_ADDR;

    /* 设置初始化值并记下开始时间 */
    start = STD_SYSTICK_Get();

    /* 以递增的方式写数据到SDRAM所有空间 */
    for (i = 1024 * 1024 / 8; i > 0 ; i--) {
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;	

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;	

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;	

        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;
        *pBuf++ = j++;	
    }

    end = STD_SYSTICK_Get();

    /* 读取写入的是否出错 */
    j = 0;
    pBuf = (uint32_t *)SDRAM_BASE_ADDR;
    for (i = 0; i < 1024 * 1024 * 4; i++) {
        if(*pBuf++ != j++) {
            break;
        }
    }

    pr_info("SDRAM Write Time: %d ms, Speed: %d MB/s\r\n", (end - start), (SDRAM_SIZE / 1024 / 1024 * 1000) / (end - start));
#endif    
}

void STD_FMC_SDRAM_ReadSpeedTest(void)
{
#if 0
    uint32_t start, end;
    uint32_t i;
    uint32_t * pBuf;
    static __IO  uint32_t ulTemp; /* 设置为__IO类型, 且设置为静态局部变量，防止被MDK优化 */

    pBuf = (uint32_t *)SDRAM_BASE_ADDR;

    /* 设置初始化值并记下开始时间 */
    start = STD_SYSTICK_Get();

    /* 读取SDRAM所有空间数据 */	
    for (i = 1024 * 1024 / 8; i >0 ; i--) {
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;

        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
        ulTemp = *pBuf++;
    }

    /* 记下结束时间 */
    end = STD_SYSTICK_Get();

    pr_info("SDRAM Read Time: %d ms, Speed: %d MB/s\r\n", (end - start), (SDRAM_SIZE / 1024 / 1024 * 1000) / (end - start));
#endif    
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/******************************* (END OF FILE) *********************************/

