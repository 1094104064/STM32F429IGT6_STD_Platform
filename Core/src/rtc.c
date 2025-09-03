/**
  ******************************************************************************
  *
  * @file    rtc.c
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
#include "rtc.h"
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
static RTC_TimeTypeDef TimeSet_Struct;	 // 时间信息
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void STD_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);		

	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xAAAA)		//检测是否第一次配置RTC
	{	
		RCC_LSEConfig(RCC_LSE_ON);  //启动LSE时钟，即外接的32.768K晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); //等待时钟配置完毕
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	//配置RTC时钟为LSE时钟
		
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;	//异步分频系数，根据手册的推荐，设置为最大
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;	//同步分频系数
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;	//时间格式为24小时制
		RTC_Init(&RTC_InitStructure);		
		RCC_RTCCLKCmd(ENABLE);	 //使能RTC	

		TimeSet_Struct.RTC_Hours = 0;TimeSet_Struct.RTC_Minutes = 0;TimeSet_Struct.RTC_Seconds = 0;  // 初始时间设置为 00:00:00
		RTC_SetTime(RTC_Format_BIN,&TimeSet_Struct);
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xAAAA);
	}
}

void STD_RTC_AlarmInit(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RTC_AlarmTypeDef RTC_AlarmStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;               /* 不匹配日期 */
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;  /* 闹铃日期 */   
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;               /* 闹铃星期 */   
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0;                             /* 闹铃小时 */   
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;                           /* 闹铃分钟 */   
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 5;                           /* 闹铃秒 */    
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

    RTC_ITConfig(RTC_IT_ALRA, ENABLE);                                          /* 使能RTC闹铃中断 */
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);                                          /* 使能RTC闹铃 */
    RTC_ClearFlag(RTC_FLAG_ALRAF);                                              /* 清除RTC闹铃标志 */
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/

