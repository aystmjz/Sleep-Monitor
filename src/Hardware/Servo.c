#include "stm32f10x.h"
#include "PWM.h"     // Device header

void Servo_Init_(void)
{
	PWM_Servo_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle/180*2000+500);
	
}
