#ifndef __SHT45_H
#define __SHT45_H
#include "stm32f10x.h" // Device header
#include "sys.h"
#include "I2C.h"

typedef struct {
    float Temp;
    float Hum;
    uint8_t raw_data[6];
} SHT45_DataTypeDef;

extern SHT45_DataTypeDef SHT;

#define SHT45_ADDRESS  0x44<<1
#define SHT45_HIGH_CMD 0xFD

void SHT45_WriteCommand(uint8_t Command);
void SHT45_MultiReadByte(uint8_t *Data_p, uint8_t Length);
void SHT45_Init(void);
uint8_t SHT45_GetData(void);

#endif
