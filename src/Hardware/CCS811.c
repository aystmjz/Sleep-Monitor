#include "CCS811.h"

CCS811_DataTypeDef CCS;

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

void f()
{
    uint8_t CCS811_BUF[12];
    uint8_t CCS811_Information[10];
    uint8_t CCS811_temp = 0x5a;
    uint8_t CCS811_MeasureMode, CCS811_Status, CCS811_Error_ID;

    CCS811_Wake();                                                   // nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
    CCS811_Information[0] = CCS811_ReadByte(HW_ID_REG);              // Read CCS's information  ,ID
    CCS811_MultiReadByte(0x23, &CCS811_Information[1], 2);           // FW_Boot_Version
    CCS811_MultiReadByte(0x24, &CCS811_Information[3], 2);           // FW_App_Version
    CCS811_Status = CCS811_ReadByte(STATUS_REG);                     // Firstly the status register is read and the APP_VALID flag is checked.
    if (CCS811_Status & 0x10) CCS811_WriteByte(APP_START_REG, 0x00); // Used to transition the CCS811 state from boot to application mode, a write with no data is required.
    CCS811_Status      = CCS811_ReadByte(STATUS_REG);
    CCS811_MeasureMode = CCS811_ReadByte(MEAS_MODE_REG);
    CCS811_WriteByte(MEAS_MODE_REG, 0x20); // Write Measure Mode Register,sensor measurement every second,no interrupt
    CCS811_Sleep();






            CCS811_Wake(); // nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
        CCS811_Status == CCS811_ReadByte(STATUS_REG);
        CCS811_Error_ID == CCS811_ReadByte(ERROR_ID_REG);
        // CCS811_WriteByte(MEAS_MODE_REG, DRIVE_MODE_1SEC);
        CCS811_MeasureMode = CCS811_ReadByte(MEAS_MODE_REG);
        CCS811_MultiReadByte(ALG_RESULT_DATA, CCS811_BUF, 8);
        CCS811_Information[0] = CCS811_ReadByte(HW_ID_REG); // Read CCS's information  ,ID
        CCS811_Sleep();
        //CCS.eco2 = (uint16_t)CCS811_BUF[0] * 256 + CCS811_BUF[1];
        //CCS.tvoc = (uint16_t)CCS811_BUF[2] * 256 + CCS811_BUF[3];
        //LCD_ShowNum(0, 0, CCS.eco2, 4);
        //LCD_ShowNum(60, 0, CCS.tvoc, 4);
        LCD_ShowNum(120, 0, CCS811_Information[0], 4);
        // CCS811_Information[0] = 0;
}
