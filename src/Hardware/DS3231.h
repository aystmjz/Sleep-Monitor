#ifndef __DS3231_H__
#define __DS3231_H__
#include "I2C.h"

#define DS3231_ADDRESS 0xD0
#define DS3231_CONTROL 0x0E
#define DS3231_STATUS  0x0F
#define Alarm_Enable 0

void DS3231_WriteByte(uint8_t WordAddress,uint8_t Data);
uint8_t DS3231_ReadByte(uint8_t WordAddress);
void DS3231_ReadTime();
void DS3231_WriteTime();
void DS3231_WriteAlarm();
void Alarm_Reset();
void DS3231_Init();

#endif