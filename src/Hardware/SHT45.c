#include "SHT45.h"

SHT45_DataTypeDef SHT;

void SHT45_WriteCommand(uint8_t Command)
{
    SHT45_I2C_Start();
    SHT45_I2C_SendByte(SHT45_ADDRESS);
    SHT45_I2C_ReceiveAck();
    SHT45_I2C_SendByte(Command);
    SHT45_I2C_ReceiveAck();
    SHT45_I2C_Stop();
}

void SHT45_MultiReadByte(uint8_t *Data_p, uint8_t Length)
{
    SHT45_I2C_Start();
    SHT45_I2C_SendByte(SHT45_ADDRESS | 0x01);
    SHT45_I2C_ReceiveAck();
    Length--;
    while (Length--) {
        *Data_p++ = SHT45_I2C_ReceiveByte();
        SHT45_I2C_SendAck(0);
    }
    *Data_p = SHT45_I2C_ReceiveByte();
    SHT45_I2C_SendAck(1);
    SHT45_I2C_Stop();
}

void SHT45_Init(void)
{
    SHT45_I2C_Init();
}

uint8_t SHT45_GetData(void)
{
    uint16_t t_ticks, rh_ticks;
    SHT45_WriteCommand(SHT45_HIGH_CMD);
    Delay_ms(10);
    SHT45_MultiReadByte(SHT.raw_data, 6);
    t_ticks  = ((uint16_t)SHT.raw_data[0] << 8) | SHT.raw_data[1];
    rh_ticks = ((uint16_t)SHT.raw_data[3] << 8) | SHT.raw_data[4];
    SHT.Temp = -45 + 175 * (float)t_ticks / 65535;
    SHT.Hum  = -6 + 125 * (float)rh_ticks / 65535;
    if (SHT.Hum > 100)
        SHT.Hum = 100;
    if (SHT.Hum < 0)
        SHT.Hum = 0;
    if (SHT.raw_data[0] || SHT.raw_data[3])
        return 1;
    else
        return 0;
}
