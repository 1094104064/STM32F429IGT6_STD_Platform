#include "bsp.h"

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

#define FMC_A0_PORT        GPIOF
#define FMC_A0_PIN         GPIO_Pin_0
#define FMC_A0_PINSOURCE   GPIO_PinSource0
#define FMC_A0_AF          GPIO_AF_FMC

#define FMC_A1_PORT        GPIOF
#define FMC_A1_PIN         GPIO_Pin_1
#define FMC_A1_PINSOURCE   GPIO_PinSource1
#define FMC_A1_AF          GPIO_AF_FMC

#define FMC_A2_PORT        GPIOF
#define FMC_A2_PIN         GPIO_Pin_2
#define FMC_A2_PINSOURCE   GPIO_PinSource2
#define FMC_A2_AF          GPIO_AF_FMC

#define FMC_A3_PORT        GPIOF
#define FMC_A3_PIN         GPIO_Pin_3
#define FMC_A3_PINSOURCE   GPIO_PinSource3
#define FMC_A3_AF          GPIO_AF_FMC

#define FMC_A4_PORT        GPIOF
#define FMC_A4_PIN         GPIO_Pin_4
#define FMC_A4_PINSOURCE   GPIO_PinSource4
#define FMC_A4_AF          GPIO_AF_FMC

#define FMC_A5_PORT        GPIOF
#define FMC_A5_PIN         GPIO_Pin_5
#define FMC_A5_PINSOURCE   GPIO_PinSource5
#define FMC_A5_AF          GPIO_AF_FMC

#define FMC_A6_PORT        GPIOF
#define FMC_A6_PIN         GPIO_Pin_12
#define FMC_A6_PINSOURCE   GPIO_PinSource12
#define FMC_A6_AF          GPIO_AF_FMC

#define FMC_A7_PORT        GPIOF
#define FMC_A7_PIN         GPIO_Pin_13
#define FMC_A7_PINSOURCE   GPIO_PinSource13
#define FMC_A7_AF          GPIO_AF_FMC

#define FMC_A8_PORT        GPIOF
#define FMC_A8_PIN         GPIO_Pin_14
#define FMC_A8_PINSOURCE   GPIO_PinSource14
#define FMC_A8_AF          GPIO_AF_FMC

#define FMC_A9_PORT        GPIOF
#define FMC_A9_PIN         GPIO_Pin_15
#define FMC_A9_PINSOURCE   GPIO_PinSource15
#define FMC_A9_AF          GPIO_AF_FMC


#define FMC_A10_PORT        GPIOG
#define FMC_A10_PIN         GPIO_Pin_0
#define FMC_A10_PINSOURCE   GPIO_PinSource0
#define FMC_A10_AF          GPIO_AF_FMC


#define FMC_A11_PORT        GPIOG
#define FMC_A11_PIN         GPIO_Pin_1
#define FMC_A11_PINSOURCE   GPIO_PinSource1
#define FMC_A11_AF          GPIO_AF_FMC

#define FMC_A12_PORT        GPIOG
#define FMC_A12_PIN         GPIO_Pin_2
#define FMC_A12_PINSOURCE   GPIO_PinSource2
#define FMC_A12_AF          GPIO_AF_FMC

/*数据信号线*/
#define FMC_D0_PORT        GPIOD
#define FMC_D0_PIN         GPIO_Pin_14
#define FMC_D0_PINSOURCE   GPIO_PinSource14
#define FMC_D0_AF          GPIO_AF_FMC

#define FMC_D1_PORT        GPIOD
#define FMC_D1_PIN         GPIO_Pin_15
#define FMC_D1_PINSOURCE   GPIO_PinSource15
#define FMC_D1_AF          GPIO_AF_FMC

#define FMC_D2_PORT        GPIOD
#define FMC_D2_PIN         GPIO_Pin_0
#define FMC_D2_PINSOURCE   GPIO_PinSource0
#define FMC_D2_AF          GPIO_AF_FMC

#define FMC_D3_PORT        GPIOD
#define FMC_D3_PIN         GPIO_Pin_1
#define FMC_D3_PINSOURCE   GPIO_PinSource1
#define FMC_D3_AF          GPIO_AF_FMC

#define FMC_D4_PORT        GPIOE
#define FMC_D4_PIN         GPIO_Pin_7
#define FMC_D4_PINSOURCE   GPIO_PinSource7
#define FMC_D4_AF          GPIO_AF_FMC

#define FMC_D5_PORT        GPIOE
#define FMC_D5_PIN         GPIO_Pin_8
#define FMC_D5_PINSOURCE   GPIO_PinSource8
#define FMC_D5_AF          GPIO_AF_FMC

#define FMC_D6_PORT        GPIOE
#define FMC_D6_PIN         GPIO_Pin_9
#define FMC_D6_PINSOURCE   GPIO_PinSource9
#define FMC_D6_AF          GPIO_AF_FMC

#define FMC_D7_PORT        GPIOE
#define FMC_D7_PIN         GPIO_Pin_10
#define FMC_D7_PINSOURCE   GPIO_PinSource10
#define FMC_D7_AF          GPIO_AF_FMC

#define FMC_D8_PORT        GPIOE
#define FMC_D8_PIN         GPIO_Pin_11
#define FMC_D8_PINSOURCE   GPIO_PinSource11
#define FMC_D8_AF          GPIO_AF_FMC

#define FMC_D9_PORT        GPIOE
#define FMC_D9_PIN         GPIO_Pin_12
#define FMC_D9_PINSOURCE   GPIO_PinSource12
#define FMC_D9_AF          GPIO_AF_FMC

#define FMC_D10_PORT        GPIOE
#define FMC_D10_PIN         GPIO_Pin_13
#define FMC_D10_PINSOURCE   GPIO_PinSource13
#define FMC_D10_AF          GPIO_AF_FMC

#define FMC_D11_PORT        GPIOE
#define FMC_D11_PIN         GPIO_Pin_14
#define FMC_D11_PINSOURCE   GPIO_PinSource14
#define FMC_D11_AF          GPIO_AF_FMC

#define FMC_D12_PORT        GPIOE
#define FMC_D12_PIN         GPIO_Pin_15
#define FMC_D12_PINSOURCE   GPIO_PinSource15
#define FMC_D12_AF          GPIO_AF_FMC

#define FMC_D13_PORT        GPIOD
#define FMC_D13_PIN         GPIO_Pin_8
#define FMC_D13_PINSOURCE   GPIO_PinSource8
#define FMC_D13_AF          GPIO_AF_FMC

#define FMC_D14_PORT        GPIOD
#define FMC_D14_PIN         GPIO_Pin_9
#define FMC_D14_PINSOURCE   GPIO_PinSource9
#define FMC_D14_AF          GPIO_AF_FMC

#define FMC_D15_PORT        GPIOD
#define FMC_D15_PIN         GPIO_Pin_10
#define FMC_D15_PINSOURCE   GPIO_PinSource10
#define FMC_D15_AF          GPIO_AF_FMC


/*控制信号线*/  
#define FMC_CS_PORT        GPIOH
#define FMC_CS_PIN         GPIO_Pin_6
#define FMC_CS_PINSOURCE   GPIO_PinSource6
#define FMC_CS_AF          GPIO_AF_FMC

#define FMC_BA0_PORT        GPIOG
#define FMC_BA0_PIN         GPIO_Pin_4
#define FMC_BA0_PINSOURCE   GPIO_PinSource4
#define FMC_BA0_AF          GPIO_AF_FMC

#define FMC_BA1_PORT        GPIOG
#define FMC_BA1_PIN         GPIO_Pin_5
#define FMC_BA1_PINSOURCE   GPIO_PinSource5
#define FMC_BA1_AF          GPIO_AF_FMC

#define FMC_WE_PORT         GPIOC
#define FMC_WE_PIN          GPIO_Pin_0
#define FMC_WE_PINSOURCE    GPIO_PinSource0
#define FMC_WE_AF           GPIO_AF_FMC

#define FMC_RAS_PORT        GPIOF
#define FMC_RAS_PIN         GPIO_Pin_11
#define FMC_RAS_PINSOURCE   GPIO_PinSource11
#define FMC_RAS_AF          GPIO_AF_FMC

#define FMC_CAS_PORT        GPIOG
#define FMC_CAS_PIN         GPIO_Pin_15
#define FMC_CAS_PINSOURCE   GPIO_PinSource15
#define FMC_CAS_AF          GPIO_AF_FMC

#define FMC_CLK_PORT        GPIOG
#define FMC_CLK_PIN         GPIO_Pin_8
#define FMC_CLK_PINSOURCE   GPIO_PinSource8
#define FMC_CLK_AF          GPIO_AF_FMC

#define FMC_CKE_PORT        GPIOH
#define FMC_CKE_PIN         GPIO_Pin_7
#define FMC_CKE_PINSOURCE   GPIO_PinSource7
#define FMC_CKE_AF          GPIO_AF_FMC

/*UDQM LDQM*/
#define FMC_UDQM_PORT        GPIOE
#define FMC_UDQM_PIN         GPIO_Pin_1
#define FMC_UDQM_PINSOURCE   GPIO_PinSource1
#define FMC_UDQM_AF          GPIO_AF_FMC

#define FMC_LDQM_PORT        GPIOE
#define FMC_LDQM_PIN         GPIO_Pin_0
#define FMC_LDQM_PINSOURCE   GPIO_PinSource0
#define FMC_LDQM_AF          GPIO_AF_FMC

#define FMC_BANK_SDRAM            FMC_Bank2_SDRAM 				 // SDRAM 的bank选择
#define FMC_COMMAND_TARGET_BANK   FMC_Command_Target_bank2


#define FMC_SDRAM_SIZE              ((uint32_t)0x02000000)
#define FMC_SDRAM_BASE_ADDR         ((uint32_t)0xD0000000)

struct gpio_config {
    GPIO_TypeDef *port;
    uint32_t pin;
    uint8_t pin_source;
    uint8_t af;
};

const struct gpio_config gpio_cfg[] = {
{.port = FMC_A0_PORT, .pin = FMC_A0_PIN, .pin_source = FMC_A0_PINSOURCE, .af = FMC_A0_AF},
{.port = FMC_A1_PORT, .pin = FMC_A1_PIN, .pin_source = FMC_A1_PINSOURCE, .af = FMC_A1_AF },
{.port = FMC_A2_PORT, .pin = FMC_A2_PIN, .pin_source = FMC_A2_PINSOURCE, .af = FMC_A2_AF },
{.port = FMC_A3_PORT, .pin = FMC_A3_PIN, .pin_source = FMC_A3_PINSOURCE, .af = FMC_A3_AF },
{.port = FMC_A4_PORT, .pin = FMC_A4_PIN, .pin_source = FMC_A4_PINSOURCE, .af = FMC_A4_AF },
{.port = FMC_A5_PORT, .pin = FMC_A5_PIN, .pin_source = FMC_A5_PINSOURCE, .af = FMC_A5_AF },
{.port = FMC_A6_PORT, .pin = FMC_A6_PIN, .pin_source = FMC_A6_PINSOURCE, .af = FMC_A6_AF },
{.port = FMC_A7_PORT, .pin = FMC_A7_PIN, .pin_source = FMC_A7_PINSOURCE, .af = FMC_A7_AF },
{.port = FMC_A8_PORT, .pin = FMC_A8_PIN, .pin_source = FMC_A8_PINSOURCE, .af = FMC_A8_AF },
{.port = FMC_A9_PORT, .pin = FMC_A9_PIN, .pin_source = FMC_A9_PINSOURCE, .af = FMC_A9_AF },
{.port = FMC_A10_PORT, .pin = FMC_A10_PIN, .pin_source = FMC_A10_PINSOURCE, .af = FMC_A10_AF },
{.port = FMC_A11_PORT, .pin = FMC_A11_PIN, .pin_source = FMC_A11_PINSOURCE, .af = FMC_A11_AF },
{.port = FMC_A12_PORT, .pin = FMC_A12_PIN, .pin_source =FMC_A12_PINSOURCE, .af = FMC_A12_AF },
{.port = FMC_D0_PORT, .pin = FMC_D0_PIN, .pin_source = FMC_D0_PINSOURCE, .af = FMC_D0_AF },
{.port = FMC_D1_PORT, .pin = FMC_D1_PIN, .pin_source = FMC_D1_PINSOURCE, .af = FMC_D1_AF },
{.port = FMC_D2_PORT, .pin = FMC_D2_PIN, .pin_source = FMC_D2_PINSOURCE, .af = FMC_D2_AF },
{.port = FMC_D3_PORT, .pin = FMC_D3_PIN, .pin_source = FMC_D3_PINSOURCE, .af = FMC_D3_AF },
{.port = FMC_D4_PORT, .pin = FMC_D4_PIN, .pin_source = FMC_D4_PINSOURCE, .af = FMC_D4_AF },
{.port = FMC_D5_PORT, .pin = FMC_D5_PIN, .pin_source = FMC_D5_PINSOURCE, .af = FMC_D5_AF },
{.port = FMC_D6_PORT, .pin = FMC_D6_PIN, .pin_source = FMC_D6_PINSOURCE, .af = FMC_D6_AF },
{.port = FMC_D7_PORT, .pin = FMC_D7_PIN, .pin_source = FMC_D7_PINSOURCE, .af = FMC_D7_AF },
{.port = FMC_D8_PORT, .pin = FMC_D8_PIN, .pin_source = FMC_D8_PINSOURCE, .af = FMC_D8_AF },
{.port = FMC_D9_PORT, .pin = FMC_D9_PIN, .pin_source = FMC_D9_PINSOURCE, .af = FMC_D9_AF },
{.port = FMC_D10_PORT, .pin = FMC_D10_PIN, .pin_source = FMC_D10_PINSOURCE, .af = FMC_D10_AF },
{.port = FMC_D11_PORT, .pin = FMC_D11_PIN, .pin_source = FMC_D11_PINSOURCE, .af = FMC_D11_AF },
{.port = FMC_D12_PORT, .pin = FMC_D12_PIN, .pin_source = FMC_D12_PINSOURCE, .af = FMC_D12_AF },
{.port =FMC_D13_PORT, .pin = FMC_D13_PIN, .pin_source = FMC_D13_PINSOURCE, .af = FMC_D13_AF },
{.port = FMC_D14_PORT, .pin = FMC_D14_PIN, .pin_source = FMC_D14_PINSOURCE, .af = FMC_D14_AF },
{.port = FMC_D15_PORT, .pin = FMC_D15_PIN, .pin_source = FMC_D15_PINSOURCE, .af = FMC_D15_AF },
{.port = FMC_CS_PORT, .pin = FMC_CS_PIN, .pin_source = FMC_CS_PINSOURCE, .af = FMC_CS_AF },
{.port = FMC_BA0_PORT, .pin = FMC_BA0_PIN, .pin_source = FMC_BA0_PINSOURCE, .af = FMC_BA0_AF },
{.port = FMC_BA1_PORT, .pin = FMC_BA1_PIN, .pin_source = FMC_BA1_PINSOURCE, .af = FMC_BA1_AF },
{.port = FMC_WE_PORT, .pin = FMC_WE_PIN, .pin_source = FMC_WE_PINSOURCE, .af = FMC_WE_AF },
{.port = FMC_RAS_PORT, .pin = FMC_RAS_PIN, .pin_source = FMC_RAS_PINSOURCE, .af = FMC_RAS_AF },
{.port = FMC_CAS_PORT, .pin = FMC_CAS_PIN, .pin_source = FMC_CAS_PINSOURCE, .af = FMC_CAS_AF },
{.port = FMC_CLK_PORT, .pin = FMC_CLK_PIN, .pin_source = FMC_CLK_PINSOURCE, .af = FMC_CLK_AF },
{.port = FMC_CKE_PORT, .pin = FMC_CKE_PIN, .pin_source = FMC_CKE_PINSOURCE, .af = FMC_CKE_AF },
{.port = FMC_UDQM_PORT, .pin = FMC_UDQM_PIN, .pin_source = FMC_UDQM_PINSOURCE, .af = FMC_UDQM_AF },
{.port = FMC_LDQM_PORT, .pin = FMC_LDQM_PIN, .pin_source = FMC_LDQM_PINSOURCE, .af = FMC_LDQM_AF },
};

static void _delay_ms(uint32_t ms)
{

}

void bsp_sdram_init(void)
{
    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | \
                           RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | \
                           RCC_AHB1Periph_GPIOH, ENABLE);

    /*!< Configure FMC SDRAM pins */
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    for(uint32_t i = 0; i < sizeof(gpio_cfg)/sizeof(gpio_cfg[0]); i++) {
        GPIO_InitStructure.GPIO_Pin   = gpio_cfg[i].pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(gpio_cfg[i].port, &GPIO_InitStructure);

        /*!< Connect FMC pins to AF */
        GPIO_PinAFConfig(gpio_cfg[i].port, gpio_cfg[i].pin_source, gpio_cfg[i].af);
    }

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
	_delay_ms(5); //延时5ms 
	 

	FMC_SDRAMCommandStructure.FMC_CommandMode               = FMC_Command_Mode_PALL;			// 预充电命令
	FMC_SDRAMCommandStructure.FMC_CommandTarget             = FMC_COMMAND_TARGET_BANK;		// 选择要控制的区域
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber         = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition    = 0;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);							// 配置SDRAM
	_delay_ms(5); //延时5ms 
	
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

}
