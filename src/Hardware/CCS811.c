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

void CCS811_Write(uint8_t WordAddress)
{
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(WordAddress);
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

uint8_t CCS811_GetHardWareID(void)
{
    return CCS811_ReadByte(HW_ID_REG);
}

uint8_t CCS811_GetErrorID(void)
{
    return CCS811_ReadByte(ERROR_ID_REG);
}

uint8_t CCS811_GetStatus(void)
{
    return CCS811_ReadByte(STATUS_REG);
}

void CCS811_RESET(void)
{
    CCS811_I2C_Start();
    CCS811_I2C_SendByte(CCS811_ADDRESS);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(SW_RESET);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(0x11);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(0xE5);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(0x72);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_SendByte(0x8A);
    CCS811_I2C_ReceiveAck();
    CCS811_I2C_Stop();
}

void CCS811_SetMeasurementMode(uint8_t mode)
{
    CCS.mode = mode;
    CCS811_Wake();
    Delay_ms(10);
    CCS811_Write(APP_START_REG);
    CCS811_WriteByte(MEAS_MODE_REG, mode);
    Delay_ms(100);
    CCS811_Sleep();
}

uint8_t CCS811_GetData(void)
{
    CCS811_Wake();
    if (CCS811_GetHardWareID() != CCS811_HardWareID) {
        Debug_printf("CCS811 HardWareError\r\n");
        CCS811_Sleep();
        return 0;
    } else if (CCS811_GetErrorID()) {
        CCS811_RESET();
        Delay_ms(100);
        CCS811_SetMeasurementMode(CCS.mode);
        Debug_printf("CCS811 RESET\r\n");
        CCS811_Sleep();
        return 0;
    }
    CCS.status = CCS811_GetStatus();
    if (CCS.status | 0x08) {
        CCS811_MultiReadByte(ALG_RESULT_DATA, CCS.raw_data, 8);
        CCS.eco2 = (uint16_t)CCS.raw_data[0] * 256 + CCS.raw_data[1];
        CCS.tvoc = (uint16_t)CCS.raw_data[2] * 256 + CCS.raw_data[3];
        CCS811_Sleep();
        if (CCS.eco2 != 400 && CCS.eco2 != 0 && CCS.tvoc != 0)
            return 1;
        else
            return 0;
    } else {
        CCS811_Sleep();
        return 0;
    }
}

/*uint8_t CCS811_BUF[12];
uint8_t CCS811_Information[10];
uint8_t CCS811_temp = 0x5a;
uint8_t CCS811_MeasureMode, CCS811_Status, CCS811_Error_ID;

CCS811_SetMeasurementMode(DRIVE_MODE_1SEC);

while (1) {

    CCS811_Wake(); // nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
                   // CCS811_WriteByte(MEAS_MODE_REG,DRIVE_MODE_1SEC); // Write Measure Mode Register,sensor measurement every second,no interrupt
    Delay_ms(300);
    CCS811_Information[0] = CCS811_ReadByte(HW_ID_REG); // Read CCS's information  ,ID
    sprintf(st, "HW_ID_REG:%x\r\n", CCS811_Information[0]);
    Debug_printf(st);
    CCS811_Status = CCS811_ReadByte(STATUS_REG);
    sprintf(st, "CCS811_Status:%x\r\n", CCS811_Status);
    Debug_printf(st);
    // if (CCS811_Status & 0x10) CCS811_WriteByte(APP_START_REG, 0x00);
    CCS811_Error_ID = CCS811_ReadByte(ERROR_ID_REG);
    sprintf(st, "CCS811_Error_ID:%x\r\n", CCS811_Error_ID);
    Debug_printf(st);
    // CCS811_WriteByte(MEAS_MODE_REG, DRIVE_MODE_1SEC);
    CCS811_MeasureMode = CCS811_ReadByte(MEAS_MODE_REG);
    sprintf(st, "CCS811_MeasureMode:%x\r\n", CCS811_MeasureMode);
    Debug_printf(st);
    CCS811_MultiReadByte(ALG_RESULT_DATA, CCS811_BUF, 8);
    // CCS811_Information[0] = CCS811_ReadByte(HW_ID_REG); // Read CCS's information  ,ID

    CCS.eco2 = (uint16_t)CCS811_BUF[0] * 256 + CCS811_BUF[1];
    CCS.tvoc = (uint16_t)CCS811_BUF[2] * 256 + CCS811_BUF[3];
    sprintf(st, "eco2=%d  tvoc=%d\r\n", CCS.eco2, CCS.tvoc);
    Debug_printf(st);
    LCD_ShowNum(0, 0, CCS.eco2, 4);
    LCD_ShowNum(60, 0, CCS.tvoc, 4);
    LCD_ShowNum(0, 40, CCS811_Information[0], 4);
    LCD_ShowNum(60, 40, CCS811_Information[1], 4);
    LCD_ShowNum(120, 40, CCS811_Information[3], 4);
    LCD_ShowNum(0, 20, CCS811_Status, 4);
    LCD_ShowNum(60, 20, CCS811_Error_ID, 4);
    LCD_ShowNum(120, 20, CCS811_MeasureMode, 4);
    Delay_ms(2000);
    // CCS811_Information[0] = 0;
    if (CCS811_GetErrorID()) {
        if (CCS811_GetHardWareID() != CCS811_HardWareID) {
            Debug_printf("CCS811 HardWareError\r\n");
        } else {
            CCS811_RESET();
            Delay_ms(100);
            CCS811_SetMeasurementMode(DRIVE_MODE_1SEC);
            Debug_printf("CCS811 RESET\r\n");
        }
    }
    CCS811_Sleep();
}
*/
