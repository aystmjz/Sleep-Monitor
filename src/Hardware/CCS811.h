#ifndef __CCS811_H__
#define __CCS811_H__
#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "Delay.h"
#include "I2C.h"

#define CCS811_Wake()  do{GPIO_ResetBits(GPIOA,GPIO_Pin_8);Delay_ms(100);}while(0)
#define CCS811_Sleep()  do{GPIO_SetBits(GPIOA,GPIO_Pin_8);Delay_ms(100);}while(0)

#define CCS811_Add  0x5A//<<1
#define CCS811_ADDRESS 0x5A<<1
#define STATUS_REG 0x00
#define MEAS_MODE_REG 0x01
#define ALG_RESULT_DATA 0x02
#define ENV_DATA 0x05
#define NTC_REG 0x06
#define THRESHOLDS 0x10
#define BASELINE 0x11
#define HW_ID_REG 0x20 //0x81 for this family of CCS81x devices
#define ERROR_ID_REG 0xE0
#define APP_START_REG 0xF4
#define SW_RESET 0xFF
#define CCS_811_ADDRESS 0x5A
#define GPIO_WAKE 0x5
#define DRIVE_MODE_IDLE 0x0
#define DRIVE_MODE_1SEC 0x10
#define DRIVE_MODE_10SEC 0x20
#define DRIVE_MODE_60SEC 0x30
#define INTERRUPT_DRIVEN 0x8
#define THRESHOLDS_ENABLED 0x4

void CCS811_WriteByte(uint8_t WordAddress, uint8_t Data);
uint8_t CCS811_ReadByte(uint8_t WordAddress);
void CCS811_MultiReadByte(uint8_t WordAddress, uint8_t *Data_p, uint8_t Length);
void CCS811_Init();


#endif

