#include "I2C.h"

void DS3231_I2C_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS3231_I2C_delay()
{
	Delay_us(50);
}

void DS3231_I2C_Start(void)
{
	DS3231_I2C_SDA_SET=1;
	DS3231_I2C_SCL_SET=1;
	DS3231_I2C_delay();
	DS3231_I2C_SDA_SET=0;
	DS3231_I2C_delay();
	DS3231_I2C_SCL_SET=0;
}

void DS3231_I2C_Stop(void)
{
	DS3231_I2C_SDA_SET=0;
	DS3231_I2C_SCL_SET=1;
	DS3231_I2C_delay();
	DS3231_I2C_SDA_SET=1;
}

void DS3231_I2C_SendByte(uint8_t Byte)
{
	for(uint8_t i=0;i<8;i++)
	{
		DS3231_I2C_SDA_SET=!(!(Byte&(0x80>>i)));
		DS3231_I2C_delay();
		DS3231_I2C_SCL_SET=1;
		DS3231_I2C_delay();
		DS3231_I2C_SCL_SET=0;
		DS3231_I2C_delay();
	}
}

uint8_t DS3231_I2C_ReceiveByte()
{
	uint8_t Byte=0x00;
	DS3231_I2C_SDA_SET=1;
	DS3231_I2C_delay();
	for(uint8_t i=0;i<8;i++)
	{
		DS3231_I2C_SCL_SET=1;
		DS3231_I2C_delay();
		if(DS3231_I2C_SDA_READ){Byte|=(0x80>>i);}
		DS3231_I2C_SCL_SET=0;
		DS3231_I2C_delay();
	}
	return Byte;
}

/// @brief 发送应答
/// @param AckBit NACK:1 ACK:0
void DS3231_I2C_SendAck(uint8_t AckBit)
{
	DS3231_I2C_SDA_SET=AckBit;
	DS3231_I2C_delay();
	DS3231_I2C_SCL_SET=1;
	DS3231_I2C_delay();
	DS3231_I2C_SCL_SET=0;
	DS3231_I2C_delay();
}

uint8_t DS3231_I2C_ReceiveAck()
{
	uint8_t AckBit;
	DS3231_I2C_SDA_SET=1;
	DS3231_I2C_delay();
	DS3231_I2C_SCL_SET=1;
	AckBit=DS3231_I2C_SDA_READ;
	DS3231_I2C_delay();
	DS3231_I2C_SCL_SET=0;
	DS3231_I2C_delay();
	return AckBit;
}


//-------------------------------------------------------

void CCS811_I2C_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void CCS811_I2C_delay()
{
	Delay_us(20);
}

void CCS811_I2C_Start(void)
{
	CCS811_I2C_SDA_SET=1;
	CCS811_I2C_SCL_SET=1;
	CCS811_I2C_delay();
	CCS811_I2C_SDA_SET=0;
	CCS811_I2C_delay();
	CCS811_I2C_SCL_SET=0;
}

void CCS811_I2C_Stop(void)
{
	CCS811_I2C_SDA_SET=0;
	CCS811_I2C_SCL_SET=1;
	CCS811_I2C_delay();
	CCS811_I2C_SDA_SET=1;
}

void CCS811_I2C_SendByte(uint8_t Byte)
{
	for(uint8_t i=0;i<8;i++)
	{
		CCS811_I2C_SDA_SET=!(!(Byte&(0x80>>i)));
		CCS811_I2C_delay();
		CCS811_I2C_SCL_SET=1;
		CCS811_I2C_delay();
		CCS811_I2C_SCL_SET=0;
		CCS811_I2C_delay();
	}
}

uint8_t CCS811_I2C_ReceiveByte()
{
	uint8_t Byte=0x00;
	CCS811_I2C_SDA_SET=1;
	CCS811_I2C_delay();
	for(uint8_t i=0;i<8;i++)
	{
		CCS811_I2C_SCL_SET=1;
		CCS811_I2C_delay();
		if(CCS811_I2C_SDA_READ){Byte|=(0x80>>i);}
		CCS811_I2C_SCL_SET=0;
		CCS811_I2C_delay();
	}
	return Byte;
}

/// @brief 发送应答
/// @param AckBit NACK:1 ACK:0
void CCS811_I2C_SendAck(uint8_t AckBit)
{
	CCS811_I2C_SDA_SET=AckBit;
	CCS811_I2C_delay();
	CCS811_I2C_SCL_SET=1;
	CCS811_I2C_delay();
	CCS811_I2C_SCL_SET=0;
	CCS811_I2C_delay();
}

uint8_t CCS811_I2C_ReceiveAck()
{
	uint8_t AckBit;
	CCS811_I2C_SDA_SET=1;
	CCS811_I2C_delay();
	CCS811_I2C_SCL_SET=1;
	AckBit=CCS811_I2C_SDA_READ;
	CCS811_I2C_delay();
	CCS811_I2C_SCL_SET=0;
	CCS811_I2C_delay();
	return AckBit;
}


//-------------------------------------------------------

void SHT45_I2C_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SHT45_I2C_delay()
{
	Delay_us(20);
}

void SHT45_I2C_Start(void)
{
	SHT45_I2C_SDA_SET=1;
	SHT45_I2C_SCL_SET=1;
	SHT45_I2C_delay();
	SHT45_I2C_SDA_SET=0;
	SHT45_I2C_delay();
	SHT45_I2C_SCL_SET=0;
}

void SHT45_I2C_Stop(void)
{
	SHT45_I2C_SDA_SET=0;
	SHT45_I2C_SCL_SET=1;
	SHT45_I2C_delay();
	SHT45_I2C_SDA_SET=1;
}

void SHT45_I2C_SendByte(uint8_t Byte)
{
	for(uint8_t i=0;i<8;i++)
	{
		SHT45_I2C_SDA_SET=!(!(Byte&(0x80>>i)));
		SHT45_I2C_delay();
		SHT45_I2C_SCL_SET=1;
		SHT45_I2C_delay();
		SHT45_I2C_SCL_SET=0;
		SHT45_I2C_delay();
	}
}

uint8_t SHT45_I2C_ReceiveByte()
{
	uint8_t Byte=0x00;
	SHT45_I2C_SDA_SET=1;
	SHT45_I2C_delay();
	for(uint8_t i=0;i<8;i++)
	{
		SHT45_I2C_SCL_SET=1;
		SHT45_I2C_delay();
		if(SHT45_I2C_SDA_READ){Byte|=(0x80>>i);}
		SHT45_I2C_SCL_SET=0;
		SHT45_I2C_delay();
	}
	return Byte;
}

/// @brief 发送应答
/// @param AckBit NACK:1 ACK:0
void SHT45_I2C_SendAck(uint8_t AckBit)
{
	SHT45_I2C_SDA_SET=AckBit;
	SHT45_I2C_delay();
	SHT45_I2C_SCL_SET=1;
	SHT45_I2C_delay();
	SHT45_I2C_SCL_SET=0;
	SHT45_I2C_delay();
}

uint8_t SHT45_I2C_ReceiveAck()
{
	uint8_t AckBit;
	SHT45_I2C_SDA_SET=1;
	SHT45_I2C_delay();
	SHT45_I2C_SCL_SET=1;
	AckBit=SHT45_I2C_SDA_READ;
	SHT45_I2C_delay();
	SHT45_I2C_SCL_SET=0;
	SHT45_I2C_delay();
	return AckBit;
}


//-------------------------------------------------------

void MAX44009_I2C_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void MAX44009_I2C_delay()
{
	Delay_us(50);
}

void MAX44009_I2C_Start(void)
{
	MAX44009_I2C_SDA_SET=1;
	MAX44009_I2C_SCL_SET=1;
	MAX44009_I2C_delay();
	MAX44009_I2C_SDA_SET=0;
	MAX44009_I2C_delay();
	MAX44009_I2C_SCL_SET=0;
}

void MAX44009_I2C_Stop(void)
{
	MAX44009_I2C_SDA_SET=0;
	MAX44009_I2C_SCL_SET=1;
	MAX44009_I2C_delay();
	MAX44009_I2C_SDA_SET=1;
}

void MAX44009_I2C_SendByte(uint8_t Byte)
{
	for(uint8_t i=0;i<8;i++)
	{
		MAX44009_I2C_SDA_SET=!(!(Byte&(0x80>>i)));
		MAX44009_I2C_delay();
		MAX44009_I2C_SCL_SET=1;
		MAX44009_I2C_delay();
		MAX44009_I2C_SCL_SET=0;
		MAX44009_I2C_delay();
	}
}

uint8_t MAX44009_I2C_ReceiveByte()
{
	uint8_t Byte=0x00;
	MAX44009_I2C_SDA_SET=1;
	MAX44009_I2C_delay();
	for(uint8_t i=0;i<8;i++)
	{
		MAX44009_I2C_SCL_SET=1;
		MAX44009_I2C_delay();
		if(MAX44009_I2C_SDA_READ){Byte|=(0x80>>i);}
		MAX44009_I2C_SCL_SET=0;
		MAX44009_I2C_delay();
	}
	return Byte;
}

/// @brief 发送应答
/// @param AckBit NACK:1 ACK:0
void MAX44009_I2C_SendAck(uint8_t AckBit)
{
	MAX44009_I2C_SDA_SET=AckBit;
	MAX44009_I2C_delay();
	MAX44009_I2C_SCL_SET=1;
	MAX44009_I2C_delay();
	MAX44009_I2C_SCL_SET=0;
	MAX44009_I2C_delay();
}

uint8_t MAX44009_I2C_ReceiveAck()
{
	uint8_t AckBit;
	MAX44009_I2C_SDA_SET=1;
	MAX44009_I2C_delay();
	MAX44009_I2C_SCL_SET=1;
	AckBit=MAX44009_I2C_SDA_READ;
	MAX44009_I2C_delay();
	MAX44009_I2C_SCL_SET=0;
	MAX44009_I2C_delay();
	return AckBit;
}