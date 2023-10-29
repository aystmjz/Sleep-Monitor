#ifndef __MLX90640_H__
#define __MLX90640_H__
#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "Delay.h"
typedef struct TempDataTypeDef
{
    float Raw[32][24];
    float Zoom[768];
    float Max;
    float Min;
    float Average;
    float Target;
} TempDataTypeDef;

extern TempDataTypeDef TempData;

uint8_t MLX90640_CheckData(uint8_t *data);
void MLX90640_SendInitCMD();
void MLX90640_SendCMD(uint8_t *CMD);
void MLX90640_Init();

#endif



