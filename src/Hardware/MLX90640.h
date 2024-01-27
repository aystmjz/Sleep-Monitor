#ifndef __MLX90640_H__
#define __MLX90640_H__
#include <math.h>
#include "stm32f10x.h" // Device header
#include "usart.h"
#include "Delay.h"
#include "lcd.h"

extern const char *MQTT_Topic;
extern uint8_t FPS_MLX90640;

#define abs(x)    ((x) > 0 ? (x) : -(x))

#define CROSS     1

#define Raw_H     24
#define Raw_L     32
#define SCREEN    240
#define BAR       16
#define Temp_MAX  4000
#define Temp_MIN  2000
#define ZOOM      8

#define MethodNUM 8

typedef enum {
    GCM_Pseudo1,
    GCM_Pseudo2,
    GCM_Metal1,
    GCM_Metal2,
    GCM_Rainbow1,
    GCM_Rainbow2,
    GCM_Rainbow3,
    GCM_Gray,
} ConverMethod;

typedef struct {
    uint16_t Raw[Raw_L][Raw_H];
    uint8_t PseColor[Raw_L][Raw_H];
    uint16_t Max, Min;
    uint8_t Max_x, Max_y, Min_x, Min_y;
    uint16_t Average;
    uint16_t Target;
} TempDataTypeDef;

typedef struct {
    uint8_t colorR;
    uint8_t colorG;
    uint8_t colorB;
    uint8_t RGB_L;
    uint8_t RGB_H;
    uint16_t RGB;
} ColorTypeDef;

extern TempDataTypeDef TempData;
extern uint8_t Emissivity;

uint16_t LCD_RGBToDATA(uint8_t colorR, uint8_t colorG, uint8_t colorB);
void TempPseColor_Init(ConverMethod Method);
void Show_TempRaw(uint8_t x, uint8_t y);
void Show_MinAndMax();
uint8_t MLX90640_CheckData(uint8_t *data);
void MLX90640_SetEmissivity(uint8_t value);
void MLX90640_SendInitCMD();
void MLX90640_SendCMD(const uint8_t *CMD);
void MLX90640_Init();
void Show_PseColorBar(uint8_t x, uint8_t y);
void Show_TempBilinearInter(uint8_t x, uint8_t y, TempDataTypeDef *Data);
uint8_t MLX90640_RefreshData(void);

#endif
