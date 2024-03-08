#include "MAX44009.h"

MAX44009_DataTypeDef MAX;

void MAX44009_WriteByte(uint8_t WordAddress, uint8_t Data)
{
    MAX44009_I2C_Start();
    MAX44009_I2C_SendByte(MAX44009_ADDRESS);
    MAX44009_I2C_ReceiveAck();
    MAX44009_I2C_SendByte(WordAddress);
    MAX44009_I2C_ReceiveAck();
    MAX44009_I2C_SendByte(Data);
    MAX44009_I2C_ReceiveAck();
    MAX44009_I2C_Stop();
}

uint8_t MAX44009_ReadByte(uint8_t WordAddress)
{
    uint8_t Data;
    MAX44009_I2C_Start();
    MAX44009_I2C_SendByte(MAX44009_ADDRESS);
    MAX44009_I2C_ReceiveAck();
    MAX44009_I2C_SendByte(WordAddress);
    MAX44009_I2C_ReceiveAck();
    MAX44009_I2C_Start();
    MAX44009_I2C_SendByte(MAX44009_ADDRESS | 0x01);
    MAX44009_I2C_ReceiveAck();
    Data = MAX44009_I2C_ReceiveByte();
    MAX44009_I2C_SendAck(1);
    MAX44009_I2C_Stop();
    return Data;
}

void MAX44009_Init(void)
{
    MAX44009_I2C_Init();
    MAX44009_WriteByte(MAX44009_CONFIG_REG, MAX44009_AUTO_MODE);
}

uint8_t MAX44009_GetData(void)
{
    uint8_t exponent, mant;
    MAX.raw_data[0] = MAX44009_ReadByte(MAX44009_HIGH_REG);
    MAX.raw_data[1] = MAX44009_ReadByte(MAX44009_LOW_REG);
    exponent        = (MAX.raw_data[0] & 0xf0) >> 4;
    mant            = (MAX.raw_data[0] & 0x0f) << 4 | MAX.raw_data[1];
    MAX.Lux         = (float)(((0x00000001 << exponent) * (float)mant) * 0.045);
    if (MAX.raw_data[0] || MAX.raw_data[1])
        return 1;
    else
        return 0;
}
