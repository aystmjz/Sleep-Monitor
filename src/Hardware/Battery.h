#ifndef __BATTERY_H
#define __BATTERY_H
#include "stm32f10x.h"                  // Device header

extern uint8_t Battery_Flag;

uint8_t Battery_calculate(uint16_t Battery);
void Update_BatteryLevel(uint16_t Battery_ADC);
uint8_t Get_BatteryLevel();

#endif