#ifndef __BATTERY_H
#define __BATTERY_H
#include "stm32f10x.h"                  // Device header

#define Battery_CHARGE 4000

extern uint8_t Battery_Flag;

uint8_t Battery_calculate(uint16_t Battery);
uint8_t Battery_UpdateLevel(uint16_t Battery_ADC);
uint8_t Battery_GetLevel();

#endif