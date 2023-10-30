#ifndef __MLX90640_H__
#define __MLX90640_H__
#include "stm32f10x.h" // Device header
#include "usart.h"
#include "Delay.h"
#include "oled.h"

#define abs(x)   ((x) > 0 ? (x) : -(x))

#define Raw_H    24
#define Raw_L    32
#define Temp_MAX 40
#define Temp_MIN 20
#define ZOOM     8
typedef struct TempDataTypeDef {
    float Raw[Raw_L][Raw_H];
    // float Zoom[(Raw_L-1)*ZOOM+1][(Raw_H-1)*ZOOM+1];
    float Zoom[(Raw_L - 1) * ZOOM + 1][10];
    float Max;
    float Min;
    float Average;
    float Target;
} TempDataTypeDef;

extern TempDataTypeDef TempData;


void Draw_TempPseColor(float Temp);
void Show_TempRaw(uint8_t Location_x,uint8_t Location_y);
uint8_t MLX90640_CheckData(uint8_t *data);
void MLX90640_SendInitCMD();
void MLX90640_SendCMD(uint8_t *CMD);
void MLX90640_Init();
void Show_TempBilinearInter(uint8_t Location_x, uint8_t Location_y, TempDataTypeDef *Data);

#endif
