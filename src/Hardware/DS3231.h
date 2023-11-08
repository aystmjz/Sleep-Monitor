#ifndef __DS3231_H__
#define __DS3231_H__
#include "I2C.h"

#define DS3231_ADDRESS 0xD0
#define DS3231_CONTROL 0x0E
#define DS3231_STATUS  0x0F
#define Alarm_Enable 0

extern uint8_t TIME[7]; //秒(0)分(1)时(2)日(3)月(4)周(5)年(6)
extern uint8_t Alarm_Date[2];//闹钟时间
extern uint8_t Alarm_Status;//闹钟状态
extern uint8_t Alarm_Enable_Flag;//闹钟使能标志

void DS3231_WriteByte(uint8_t WordAddress,uint8_t Data);
uint8_t DS3231_ReadByte(uint8_t WordAddress);
void DS3231_ReadTime();
void DS3231_WriteTime();
void DS3231_WriteAlarm();
void Alarm_Reset();
void DS3231_Init();

#endif