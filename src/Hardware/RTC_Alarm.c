
#include "RTC_Alarm.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "led.h"
//RTC中断配置
 void RTC_NVIC_Config(void)
{    
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;        //RTC全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    //先占优先级1位,从优先级3位
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //使能该通道中断
    NVIC_Init(&NVIC_InitStructure);        //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//RTC闹钟初始化:启动时钟、配置LSI做RTC时钟、设置预分频40000得到1Hz
//设置运行时间WORK_TIMES
void RTC_Alarm_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);	
	/* Reset Backup Domain */
	BKP_DeInit();

    /* RTC clock source configuration ----------------------------------------*/
	/* Enable the LSI OSC */
  	RCC_LSICmd(ENABLE);
    /* Wait till LSI is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
    }
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask(); 
	/* 使能RTC闹钟中断*/
	RTC_ITConfig(RTC_IT_ALR, ENABLE);	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(40000);	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
		
	//中断配置
	RTC_NVIC_Config();

	//设置运行WORK_TIMES
	RTC_SetAlarm(RTC_GetCounter() + WORK_TIMES);
	RTC_WaitForLastTask();
}

//设置闹钟时长并进入待机
//s为中断秒数
void RTC_Enter_StandbyMode(u32 s)
{

    RTC_SetAlarm(RTC_GetCounter() + s);
    RTC_WaitForLastTask();
    // 进入待机模式, 此时所有1.8V域的时钟都关闭,HIS和HSE的振荡器关闭, 电压调节器关闭.
    // 只有WKUP引脚上升沿,RTC警告事件,NRST引脚的外部复位,IWDG复位.
	/* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
   // PWR_EnterSTANDBYMode();
}

//中断服务函数
void RTC_IRQHandler(void)
{
 if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
 {
  	RTC_ClearITPendingBit(RTC_IT_ALR);  //清闹钟中断
	RTC_Enter_StandbyMode(STANDBY_TIMES);//进入待机
 }                 
}


