#include "led.h"

/*
//初始化PA0
//LED IO初始化
//初始化PA15 PA1   
//LED IO初始化  这些端口根据你的板子的灯来定义，没有特别指定
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA,PB端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA0
    GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_8);					

	 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB3
    GPIO_SetBits(GPIOB,GPIO_Pin_0);			
	  LED1=0;
}
*/


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);
}
        
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
}


void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_2)==0)
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
	else
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);
}

