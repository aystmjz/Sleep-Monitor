#include "DS3231.h"

static const uint8_t RTC_CLOCK_ADDR[7]  = {0x00, 0x01, 0x02, 0x04, 0x05, 0x03, 0x06};
static const uint8_t RTC_ALARM1_ADDR[4] = {0x07, 0x08, 0x09, 0x0A};
static const uint8_t RTC_ALARM2_ADDR[3] = {0x0B, 0x0C, 0x0D};
uint8_t Alarm_Date[2]                   = {8, 0}; // 闹钟时间
uint8_t Alarm_Status;                             // 闹钟状态
uint8_t Alarm_Enable_Flag;                        // 闹钟使能标志

struct tm Time_Date; // 全局时间

void DS3231_TurnOnAlarm(void)
{
}

void DS3231_TurnOffAlarm(void)
{
}

uint8_t DS3231_GetAlarmEnableFlag(void)
{
    return ((DS3231_ReadByte(DS3231_CONTROL) & 0x01));
}

uint8_t DS3231_GetAlarmStatus(void)
{
    return (DS3231_ReadByte(DS3231_STATUS) & 0x01);
}

void DS3231_WriteByte(uint8_t WordAddress, uint8_t Data)
{
    DS3231_I2C_Start();
    DS3231_I2C_SendByte(DS3231_ADDRESS);
    DS3231_I2C_ReceiveAck();
    DS3231_I2C_SendByte(WordAddress);
    DS3231_I2C_ReceiveAck();
    DS3231_I2C_SendByte(Data);
    DS3231_I2C_ReceiveAck();
    DS3231_I2C_Stop();
}

uint8_t DS3231_ReadByte(uint8_t WordAddress)
{
    uint8_t Data;
    DS3231_I2C_Start();
    DS3231_I2C_SendByte(DS3231_ADDRESS);
    DS3231_I2C_ReceiveAck();
    DS3231_I2C_SendByte(WordAddress);
    DS3231_I2C_ReceiveAck();
    DS3231_I2C_Stop();
    DS3231_I2C_Start();
    DS3231_I2C_SendByte(DS3231_ADDRESS | 0x01);
    DS3231_I2C_ReceiveAck();
    Data = DS3231_I2C_ReceiveByte();
    DS3231_I2C_SendAck(1);
    DS3231_I2C_Stop();
    return Data;
}

/// @brief 读取时钟(闹钟)数据
void DS3231_ReadTime()
{
    uint8_t i, temp, time[7];
    for (i = 0; i < 7; i++) {
        temp    = DS3231_ReadByte(RTC_CLOCK_ADDR[i]);
        time[i] = (temp / 16) * 10 + (temp % 16);
    }
    Alarm_Status = (DS3231_ReadByte(DS3231_STATUS) & 0x01);

    Time_Date.tm_year = time[6] + 2000 - 1900;
    Time_Date.tm_mon  = time[4] - 1;
    Time_Date.tm_mday = time[3];
    Time_Date.tm_hour = time[2];
    Time_Date.tm_min  = time[1];
    Time_Date.tm_sec  = time[0];
    mktime(&Time_Date); // 计算星期
}

time_t DS3231_GetTimeStamp(void)
{
    return (mktime(&Time_Date) - 8 * 60 * 60);
}

/// @brief 写入时钟数据
void DS3231_WriteTime(struct tm time_data)
{
    uint8_t time[7];
    mktime(&time_data); // 计算星期
    time[6] = time_data.tm_year + 1900 - 2000;
    time[4] = time_data.tm_mon + 1;
    time[5] = time_data.tm_wday;
    time[3] = time_data.tm_mday;
    time[2] = time_data.tm_hour;
    time[1] = time_data.tm_min;
    time[0] = time_data.tm_sec;
    for (uint8_t i = 0; i < 7; i++) {
        DS3231_WriteByte(RTC_CLOCK_ADDR[i], (time[i] / 10) * 16 + time[i] % 10);
    }
}

/// @brief 写入闹钟数据
void DS3231_WriteAlarm()
{
    DS3231_WriteByte(RTC_ALARM1_ADDR[0], 0);
    DS3231_WriteByte(RTC_ALARM1_ADDR[3], 0x80);
    for (uint8_t i = 1; i < 3; i++) {
        DS3231_WriteByte(RTC_ALARM1_ADDR[i], ((Alarm_Date[2 - i] / 10) << 4) + (Alarm_Date[2 - i] % 10));
    }
    DS3231_WriteByte(DS3231_CONTROL, 0x04 + Alarm_Enable_Flag);
}

/// @brief 复位闹钟
void DS3231_ResetAlarm()
{
    DS3231_WriteByte(DS3231_STATUS, 0x00);
}

void DS3231_Init()
{
    DS3231_I2C_Init();
    uint8_t i, temp;
    DS3231_WriteByte(DS3231_STATUS, 0x00);
    if (!Alarm_Enable) DS3231_WriteByte(DS3231_CONTROL, 0x04);
    DS3231_ReadTime();

    temp              = DS3231_ReadByte(DS3231_CONTROL);
    Alarm_Enable_Flag = !(!(temp & 0x01));

    Alarm_Status = (DS3231_ReadByte(DS3231_STATUS) & 0x01);

    for (i = 1; i <= 2; i++) {
        temp              = DS3231_ReadByte(RTC_ALARM1_ADDR[i]);
        Alarm_Date[2 - i] = (temp >> 4) * 10 + (temp & 0x0F);
    }
}

uint8_t IsSameDay(time_t time_cnt1, time_t time_cnt2)
{
    struct tm time_date1, time_date2;
    time_date1 = *localtime(&time_cnt1);
    time_date2 = *localtime(&time_cnt2);
    if (time_date1.tm_yday == time_date2.tm_yday)
        return 1;
    else
        return 0;
}

uint8_t IsTime(void)
{
    static uint8_t last_sec=0;
    if (Time_Sec / 10 == last_sec)
        return 0;
    else if ((Time_Hour >= 0 && Time_Hour <= TIME_HOUR_END) || (Time_Hour <= 23 && Time_Hour >= TIME_HOUR_BEGIN)) {
        last_sec = Time_Sec / 10;
        return 1;
    } else
        return 0;
}

uint16_t TimeDate_ToAddress(uint8_t hour, uint8_t min, uint8_t sec)
{
    if (hour <= TIME_HOUR_END) hour += 24;
    hour -= TIME_HOUR_BEGIN;
    sec /= 10;
    return (hour * 60 * 6 + min * 6 + sec);
}

uint16_t TimeStamp_ToAddress(time_t time_cnt)
{
    struct tm time_date;
    time_cnt += 8 * 60 * 60;
    time_date = *localtime(&time_cnt);
    return TimeDate_ToAddress(time_date.tm_hour, time_date.tm_min, time_date.tm_sec);
}