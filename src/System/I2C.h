#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "Delay.h"

#define DS3231_I2C_SDA_SET  PBout(8)
#define DS3231_I2C_SDA_READ PBin(8)
#define DS3231_I2C_SCL_SET  PBout(9)

void DS3231_I2C_Init();
void DS3231_I2C_delay();
void DS3231_I2C_Start(void);
void DS3231_I2C_Stop(void);
void DS3231_I2C_SendByte(uint8_t Byte);
uint8_t DS3231_I2C_ReceiveByte();
void DS3231_I2C_SendAck(uint8_t AckBit);
uint8_t DS3231_I2C_ReceiveAck();

#endif