#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Timer_Init(void);
void Key_Init(void);
uint8_t Key_Read(void);
uint8_t Key_Get(void);
uint8_t Key_GetState();
void Key_Entry();

#endif
