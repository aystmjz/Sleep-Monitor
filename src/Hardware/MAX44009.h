#ifndef __MAX44009_H
#define __MAX44009_H
#include "stm32f10x.h" // Device header
#include "sys.h"
#include "I2C.h"

typedef struct {
    float Lux;
    uint8_t raw_data[2];
} MAX44009_DataTypeDef;

extern MAX44009_DataTypeDef MAX;

#define MAX44009_ADDRESS  0x94
#define MAX44009_CONFIG_REG 0x02
#define MAX44009_HIGH_REG 0x03
#define MAX44009_LOW_REG 0x04
#define MAX44009_AUTO_MODE 0x00


uint8_t MAX44009_ReadByte(uint8_t WordAddress);
void MAX44009_Init(void);
uint8_t MAX44009_GetData(void);

#endif