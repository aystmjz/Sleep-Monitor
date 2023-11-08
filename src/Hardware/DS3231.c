#include "DS3231.h"

const uint8_t RTC_CLOCK_ADDR[7]={0x00,0x01,0x02,0x04,0x05,0x03,0x06};
const uint8_t RTC_ALARM1_ADDR[4]={0x07,0x08,0x09,0x0A};
const uint8_t RTC_ALARM2_ADDR[3]={0x0B,0x0C,0x0D};
uint8_t TIME[7] = {0, 0, 0x16, 0x1C, 0x06, 0x01, 0x17};//秒(0)分(1)时(2)日(3)月(4)周(5)年(6)
uint8_t Alarm_Date[2]={8,0};//闹钟时间
uint8_t Alarm_Status;//闹钟状态
uint8_t Alarm_Enable_Flag;//闹钟使能标志


void DS3231_WriteByte(uint8_t WordAddress,uint8_t Data)
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
	DS3231_I2C_SendByte(DS3231_ADDRESS|0x01);
	DS3231_I2C_ReceiveAck();
	Data=DS3231_I2C_ReceiveByte();
	DS3231_I2C_SendAck(1);
	DS3231_I2C_Stop();
	return Data;
}

/// @brief 读取时钟(闹钟)数据
void DS3231_ReadTime()
{
    uint8_t i,Temp;
    for(i=0;i<7;i++)
    {
		Temp = DS3231_ReadByte(RTC_CLOCK_ADDR[i]);
      	TIME[i]=(Temp/16)*10+(Temp%16);
    }
	Alarm_Status=(DS3231_ReadByte(DS3231_STATUS)&0x01);
}

/// @brief 写入时钟数据
void DS3231_WriteTime()
{
    for(uint8_t i=0;i<7;i++)
    {
        DS3231_WriteByte(RTC_CLOCK_ADDR[i],(TIME[i]/10)*16+TIME[i]%10);
    }
}

/// @brief 写入闹钟数据
void DS3231_WriteAlarm()
{
	DS3231_WriteByte(RTC_ALARM1_ADDR[0],0);
	DS3231_WriteByte(RTC_ALARM1_ADDR[3],0x80);
	for(uint8_t i=1;i<3;i++)
	{
		DS3231_WriteByte(RTC_ALARM1_ADDR[i],((Alarm_Date[2-i]/10)<<4)+(Alarm_Date[2-i]%10));
	}
	DS3231_WriteByte(DS3231_CONTROL,0x04+Alarm_Enable_Flag);
}

/// @brief 复位闹钟
void Alarm_Reset()
{
	DS3231_WriteByte(DS3231_STATUS,0x00);
}

void DS3231_Init()
{
	DS3231_I2C_Init();
	uint8_t i,Temp;
	DS3231_WriteByte(DS3231_STATUS,0x00);
	if(!Alarm_Enable)DS3231_WriteByte(DS3231_CONTROL,0x04);
	DS3231_ReadTime();

	Temp=DS3231_ReadByte(DS3231_CONTROL);
	Alarm_Enable_Flag=!(!(Temp&0x01));

	Alarm_Status=(DS3231_ReadByte(DS3231_STATUS)&0x01);

	for(i=1;i<=2;i++)
	{
		Temp=DS3231_ReadByte(RTC_ALARM1_ADDR[i]);
		Alarm_Date[2-i]=(Temp>>4)*10+(Temp&0x0F);
	}
}