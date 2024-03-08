#include "Key.h"

uint8_t Key_KeyNumber;

void Timer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_InternalClockConfig(TIM1);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=9500-1;//100ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM1,ENABLE);
}


void Key_Init(void)
{
	Timer_Init();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_Read(void)
{
	return !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
}

uint8_t Key_Get(void)
{
	uint8_t Temp=0;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;

	return Temp;
}

uint8_t Key_GetState()
{
	uint8_t KeyNumber=0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)==0){KeyNumber=1;}

	return KeyNumber;
}

void Key_Entry()
{
	static uint8_t NowState,LastState;
	static uint16_t HoldTimer;
	LastState=NowState;
	NowState=Key_GetState();
	if(LastState==0&&NowState==1){Key_KeyNumber=1;HoldTimer=0;}
	else if(LastState==1&&NowState==1){HoldTimer++;if(HoldTimer>20)Key_KeyNumber=2;}
}