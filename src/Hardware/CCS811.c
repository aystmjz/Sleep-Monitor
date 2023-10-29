
/******************** (C) COPYRIGHT  长江智动科技 ********************
 * 文件名  ：main.c
 * 描述    ：CJMCU测试代码
 * 库版本  ：3.5
 * 编译软件：KEIL4
 * 作者    ：D.K.Ann
 * 店铺地址：https://cjmcu.taobao.com/
 * 修改时间 ：2016-11-4
 **********************************************************************************/
/********************************************************************
硬件接法：
 STM32
SCL---PB6
SDA---PB7
aWAKE---PA4
STM32---FT232
TX---RX
RX---TX
波特率115200
********************************************************************/
#include "CCS811.h"

void CCS811_WriteByte(uint8_t WordAddress, uint8_t Data)
{
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(WordAddress);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(Data);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_Stop();
}

uint8_t CCS811_ReadByte(uint8_t WordAddress)
{
    uint8_t Data;
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(WordAddress);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_Stop();
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS | 0x01);
    CCS811_I2C_ReceiveAck();
    Data = CCS811_I2C_ReceiveByte();
    CCS811_I2C_SendAck(1);
    CCS811_I2C_Stop();
    return Data;
}

void CCS811_MultiReadByte(uint8_t WordAddress, uint8_t *Data_p, uint8_t Length)
{
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(WordAddress);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_Stop();
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS | 0x01);
    CCS811_I2C_ReceiveAck();
    Length--;
    while (Length--) {
        *Data_p++ = CCS811_I2C_ReceiveByte();
        CCS811_I2C_SendAck(0);
    }
    *Data_p = CCS811_I2C_ReceiveByte();
    CCS811_I2C_SendAck(1);
    CCS811_I2C_Stop();
}


void CCS811_Init()
{
    CCS811_I2C_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    CCS811_Sleep();
}

