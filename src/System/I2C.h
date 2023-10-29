#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "Delay.h"

#define DS3231_I2C_SDA_SET  PBout(8)
#define DS3231_I2C_SDA_READ PBin(8)
#define DS3231_I2C_SCL_SET  PBout(9)

#define CCS811_I2C_SDA_SET  PAout(11)
#define CCS811_I2C_SDA_READ PAin(11)
#define CCS811_I2C_SCL_SET  PAout(12)

void DS3231_I2C_Init();
void DS3231_I2C_delay();
void DS3231_I2C_Start(void);
void DS3231_I2C_Stop(void);
void DS3231_I2C_SendByte(uint8_t Byte);
uint8_t DS3231_I2C_ReceiveByte();
void DS3231_I2C_SendAck(uint8_t AckBit);
uint8_t DS3231_I2C_ReceiveAck();

void CCS811_I2C_Init();
void CCS811_I2C_delay();
void CCS811_I2C_Start(void);
void CCS811_I2C_Stop(void);
void CCS811_I2C_SendByte(uint8_t Byte);
uint8_t CCS811_I2C_ReceiveByte();
void CCS811_I2C_SendAck(uint8_t AckBit);
uint8_t CCS811_I2C_ReceiveAck();

#endif