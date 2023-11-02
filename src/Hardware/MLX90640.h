#ifndef __MLX90640_H__
#define __MLX90640_H__
#include <math.h>
#include "stm32f10x.h" // Device header
#include "usart.h"
#include "Delay.h"
#include "oled.h"


#define abs(x)   ((x) > 0 ? (x) : -(x))

#define Raw_H    24
#define Raw_L    32
#define SCREEN   240
#define BAR      16
#define Temp_MAX 4000
#define Temp_MIN 2000
#define ZOOM     8
typedef struct TempDataTypeDef {
    uint16_t Raw[Raw_L][Raw_H];
    // float Zoom[(Raw_L-1)*ZOOM+1][(Raw_H-1)*ZOOM+1];
    // float Zoom[(Raw_L - 1) * ZOOM + 1][10];
    uint8_t PseColor[Raw_L][Raw_H];
    uint16_t Max;
    uint16_t Min;
    uint16_t Average;
    uint16_t Target;
} TempDataTypeDef;

typedef struct ColorTypeDef {
    uint8_t colorR;
    uint8_t colorG;
    uint8_t colorB;
    uint8_t RGB_L;
    uint8_t RGB_H;
    uint16_t RGB;
} ColorTypeDef;

extern TempDataTypeDef TempData;
extern uint16_t Conter_;

uint16_t LCD_RGBToDATA(uint8_t colorR, uint8_t colorG, uint8_t colorB);
void TempPseColor_Init();
void Draw_TempPseColor(float Temp);
void Show_TempRaw(uint8_t Location_x, uint8_t Location_y);
uint8_t MLX90640_CheckData(uint8_t *data);
void MLX90640_SendInitCMD();
void MLX90640_SendCMD(uint8_t *CMD);
void MLX90640_Init();
void Show_PseColorBar(uint8_t Location_x, uint8_t Location_y);
void Show_TempBilinearInter(uint8_t Location_x, uint8_t Location_y, TempDataTypeDef *Data);

#endif
