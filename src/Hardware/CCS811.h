#ifndef __CCS811_H__
#define __CCS811_H__
#include "stm32f10x.h" // Device header
#include "sys.h"
#include "Delay.h"
#include "I2C.h"
#include "EC800.h"

typedef struct {
    uint16_t eco2;
    uint16_t tvoc;
    uint8_t status;
    uint8_t mode;
    uint8_t raw_data[8];
} CCS811_DataTypeDef;

extern CCS811_DataTypeDef CCS;

#define CCS811_Wake()  do{GPIO_ResetBits(GPIOA,GPIO_Pin_8);Delay_ms(100);}while(0)
#define CCS811_Sleep()  do{GPIO_SetBits(GPIOA,GPIO_Pin_8);Delay_ms(100);}while(0)

#define CCS811_ADDRESS     0x5A << 1
#define CCS811_HardWareID  0x81
#define STATUS_REG         0x00
#define MEAS_MODE_REG      0x01
#define ALG_RESULT_DATA    0x02
#define ENV_DATA           0x05
#define NTC_REG            0x06
#define THRESHOLDS         0x10
#define BASELINE           0x11
#define HW_ID_REG          0x20
#define ERROR_ID_REG       0xE0
#define APP_START_REG      0xF4
#define SW_RESET           0xFF
#define CCS_811_ADDRESS    0x5A
#define GPIO_WAKE          0x5
#define DRIVE_MODE_IDLE    0x0
#define DRIVE_MODE_1SEC    0x10
#define DRIVE_MODE_10SEC   0x20
#define DRIVE_MODE_60SEC   0x30
#define INTERRUPT_DRIVEN   0x8
#define THRESHOLDS_ENABLED 0x4

void CCS811_WriteByte(uint8_t WordAddress, uint8_t Data);
void CCS811_Write(uint8_t WordAddress);
uint8_t CCS811_ReadByte(uint8_t WordAddress);
void CCS811_MultiReadByte(uint8_t WordAddress, uint8_t *Data_p, uint8_t Length);
void CCS811_Init();
uint8_t CCS811_GetHardWareID(void);
uint8_t CCS811_GetErrorID(void);
void CCS811_RESET(void);
void CCS811_SetMeasurementMode(uint8_t mode);
uint8_t CCS811_GetData(void);

#endif
