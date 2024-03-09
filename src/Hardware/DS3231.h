#ifndef __DS3231_H__
#define __DS3231_H__
#include "I2C.h"
#include <time.h>

#define DS3231_ADDRESS  0xD0
#define DS3231_CONTROL  0x0E
#define DS3231_STATUS   0x0F
#define Alarm_Enable    0

#define Time_Year       (Time_Date.tm_year + 1900)
#define Time_Mon        (Time_Date.tm_mon + 1)
#define Time_Day        (Time_Date.tm_mday)
#define Time_Week       (Time_Date.tm_wday)
#define Time_Hour       (Time_Date.tm_hour)
#define Time_Min        (Time_Date.tm_min)
#define Time_Sec        (Time_Date.tm_sec)

#define TIME_HOUR_BEGIN 22
#define TIME_HOUR_END   9
//时间段总长度不能超过11小时

extern uint8_t Alarm_Date[2];     // 闹钟时间
extern uint8_t Alarm_Status;      // 闹钟状态
extern uint8_t Alarm_Enable_Flag; // 闹钟使能标志
extern struct tm Time_Date;

void DS3231_WriteByte(uint8_t WordAddress, uint8_t Data);
uint8_t DS3231_ReadByte(uint8_t WordAddress);
void DS3231_ReadTime();
time_t DS3231_GetTimeStamp(void);
void DS3231_WriteTime(struct tm time);
void DS3231_WriteAlarm();
void DS3231_ResetAlarm();
void DS3231_Init();
uint8_t IsSameDay(time_t time_cnt1, time_t time_cnt2);
uint8_t IsTime(void);
uint8_t Check_date(uint8_t w_year, uint8_t w_month, uint8_t w_date);
uint16_t TimeDate_ToAddress(uint8_t hour, uint8_t min, uint8_t sec);
uint16_t TimeStamp_ToAddress(time_t time_cnt);

#endif