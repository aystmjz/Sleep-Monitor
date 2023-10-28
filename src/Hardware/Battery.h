#ifndef __BATTERY_H
#define __BATTERY_H
#include "stm32f10x.h"                  // Device header

uint8_t Battery_calculate(uint16_t Battery);
uint8_t Get_BatteryLevel();

#endif