#include "MLX90640.h"

const uint8_t CMD_SAVE[4] = {0xA5, 0x56, 0x02, 0xFD};
const uint8_t CMD_AUTO[4] = {0xA5, 0x35, 0x02, 0xDC};
uint8_t RX_BUF[1550]      = {0};
int8_t Emissivity         = 95;
extern uint8_t Select_State;

TempDataTypeDef TempData;
ColorTypeDef PseColor[256];

uint16_t LCD_RGBToDATA(uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t Color;
    Color = (uint16_t)((colorR & 0xF8) << 8) | ((colorG & 0xFE) << 3) | ((colorB & 0xF8) >> 3);
    return Color;
}

void TempPseColor_Init(ConverMethod Method)
{
    uint8_t colorR, colorG, colorB;
    uint16_t Color;
    float Value;
    for (uint16_t n = 0; n < 256; n++) {
        Value = n;
        switch (Method) {
            case GCM_Pseudo1:
                colorR = abs(0 - Value);
                colorG = abs(127 - Value);
                colorB = abs(255 - Value);
                break;

            case GCM_Pseudo2:
                if ((Value >= 0) && (Value <= 63)) {
                    colorR = 0;
                    colorG = 0;
                    colorB = round(Value / 64.0 * 255.0);
                } else if ((Value >= 64) && (Value <= 127)) {
                    colorR = 0;
                    colorG = round((Value - 64.0) / 64.0 * 255.0) + 1;
                    colorB = round((127.0 - Value) / 64.0 * 255.0) - 1;
                } else if ((Value >= 128) && (Value <= 191)) {
                    colorR = round((Value - 128.0) / 64.0 * 255.0);
                    colorG = 255;
                    colorB = 0;
                } else if ((Value >= 192) && (Value <= 255)) {
                    colorR = 255;
                    colorG = round((255.0 - Value) / 64.0 * 255.0);
                    colorB = 0;
                }
                break;

            case GCM_Metal1:
                if ((Value >= 0) && (Value <= 63)) {
                    colorR = 0;
                    colorG = 0;
                    colorB = round(Value / 64 * 255);
                } else if ((Value >= 64) && (Value <= 95)) {
                    colorR = round((Value - 63) / 32 * 127);
                    colorG = round((Value - 63) / 32 * 127);
                    colorB = 255;
                } else if ((Value >= 96) && (Value <= 127)) {
                    colorR = round((Value - 95) / 32 * 127) + 128;
                    colorG = round((Value - 95) / 32 * 127) + 128;
                    colorB = round((127 - Value) / 32 * 255);
                } else if ((Value >= 128) && (Value <= 191)) {
                    colorR = 255;
                    colorG = 255;
                    colorB = 0;
                } else if ((Value >= 192) && (Value <= 255)) {
                    colorR = 255;
                    colorG = 255;
                    colorB = round((Value - 192) / 64 * 255);
                }
                break;

            case GCM_Metal2:
                colorR = 0;
                colorG = 0;
                colorB = 0;
                if ((Value >= 0) && (Value <= 16)) {
                    colorR = 0;
                } else if ((Value >= 17) && (Value <= 140)) {
                    colorR = round((Value - 16) / (140 - 16) * 255);
                } else if ((Value >= 141) && (Value <= 255)) {
                    colorR = 255;
                }

                if ((Value >= 0) && (Value <= 101)) {
                    colorG = 0;
                } else if ((Value >= 102) && (Value <= 218)) {
                    colorG = round((Value - 101) / (218 - 101) * 255);
                } else if ((Value >= 219) && (Value <= 255)) {
                    colorG = 255;
                }

                if ((Value >= 0) && (Value <= 91)) {
                    colorB = 28 + round((Value - 0) / (91 - 0) * 100);
                } else if ((Value >= 92) && (Value <= 120)) {
                    colorB = round((120 - Value) / (120 - 91) * 128);
                } else if ((Value >= 129) && (Value <= 214)) {
                    colorB = 0;
                } else if ((Value >= 215) && (Value <= 255)) {
                    colorB = round((Value - 214) / (255 - 214) * 255);
                }
                break;

            case GCM_Rainbow1:
                if ((Value >= 0) && (Value <= 31)) {
                    colorR = 0;
                    colorG = 0;
                    colorB = round(Value / 32 * 255);
                } else if ((Value >= 32) && (Value <= 63)) {
                    colorR = 0;
                    colorG = round((Value - 32) / 32 * 255);
                    colorB = 255;
                } else if ((Value >= 64) && (Value <= 95)) {
                    colorR = 0;
                    colorG = 255;
                    colorB = round((95 - Value) / 32 * 255);
                } else if ((Value >= 96) && (Value <= 127)) {
                    colorR = round((Value - 96) / 32 * 255);
                    colorG = 255;
                    colorB = 0;
                } else if ((Value >= 128) && (Value <= 191)) {
                    colorR = 255;
                    colorG = round((191 - Value) / 64 * 255);
                    colorB = 0;
                } else if ((Value >= 192) && (Value <= 255)) {
                    colorR = 255;
                    colorG = round((Value - 192) / 64 * 255); // 0
                    colorB = round((Value - 192) / 64 * 255);
                }
                break;

            case GCM_Rainbow2:
                if ((Value >= 0) && (Value <= 63)) {
                    colorR = 0;
                    colorG = round((Value - 0) / 64 * 255);
                    colorB = 255;
                } else if ((Value >= 64) && (Value <= 95)) {
                    colorR = 0;
                    colorG = 255;
                    colorB = round((95 - Value) / 32 * 255);
                } else if ((Value >= 96) && (Value <= 127)) {
                    colorR = round((Value - 96) / 32 * 255);
                    colorG = 255;
                    colorB = 0;
                } else if ((Value >= 128) && (Value <= 191)) {
                    colorR = 255;
                    colorG = round((191 - Value) / 64 * 255);
                    colorB = 0;
                } else if ((Value >= 192) && (Value <= 255)) {
                    colorR = 255;
                    colorG = round((Value - 192) / 64 * 255);
                    colorB = round((Value - 192) / 64 * 255);
                }
                break;

            case GCM_Rainbow3:
                if ((Value >= 0) && (Value <= 51)) {
                    colorR = 0;
                    colorG = Value * 5;
                    colorB = 255;
                } else if ((Value >= 52) && (Value <= 102)) {
                    colorR = 0;
                    colorG = 255;
                    colorB = 255 - (Value - 51) * 5;
                } else if ((Value >= 103) && (Value <= 153)) {
                    colorR = (Value - 102) * 5;
                    colorG = 255;
                    colorB = 0;
                } else if ((Value >= 154) && (Value <= 204)) {
                    colorR = 255;
                    colorG = round(255 - 128 * (Value - 153) / 51);
                    colorB = 0;
                } else if ((Value >= 205) && (Value <= 255)) {
                    colorR = 255;
                    colorG = round(127 - 127 * (Value - 204) / 51);
                    colorB = 0;
                }
                break;

            case GCM_Gray:
                colorR = Value;
                colorG = Value;
                colorB = Value;
                break;

            default:
                break;
        }
        Color              = LCD_RGBToDATA(colorR, colorG, colorB);
        PseColor[n].RGB_H  = Color >> 8;
        PseColor[n].RGB_L  = Color;
        PseColor[n].RGB    = Color;
        PseColor[n].colorR = colorR;
        PseColor[n].colorG = colorG;
        PseColor[n].colorB = colorB;
    }
    // sprintf(strc, "colorR=%.2d colorG=%.2d colorB=%.2d \r\n", colorR, colorG, colorB);
    // Debug_printf(strc);
    // Delay_ms(10);
}

void Show_PseColorBar(uint8_t Location_x, uint8_t Location_y)
{
    Address_set(Location_x, Location_y, Location_x + SCREEN - 1, Location_y + BAR - 1); // 坐标设置
    for (uint8_t n = 0; n < (BAR / ZOOM); n++) {
        OLED_DMA_ClearCounter();
        for (uint8_t i = 0; i < ZOOM; i++) {
            for (uint8_t Value = 10; Value < 250; Value++) {
                OLED_DMA_AddDate(PseColor[Value].RGB_H);
                OLED_DMA_AddDate(PseColor[Value].RGB_L);
            }
        }
        OLED_DMA_Transfer();
        OLED_DMA_Waite();
    }
}

void Show_TempRaw(uint8_t Location_x, uint8_t Location_y) // 显示原始热力图
{
    Address_set(Location_x, Location_y, Location_x + Raw_L - 1, Location_y + Raw_H - 1); // 坐标设置
    OLED_DMA_ClearCounter();
    for (uint8_t i = 0; i < Raw_H; i++) {
        for (uint8_t j = 0; j < Raw_L; j++) {
            OLED_DMA_AddDate(PseColor[TempData.PseColor[j][i]].RGB_H);
            OLED_DMA_AddDate(PseColor[TempData.PseColor[j][i]].RGB_L);
        }
    }
    OLED_DMA_Transfer();
    OLED_DMA_Waite();
}

void Show_MinAndMax(uint8_t n)
{
    POINT_COLOR = WHITE;
    if (n == (TempData.Max_y) || n == (TempData.Max_y + 1)) LCD_DrawCross(((TempData.Max_x * SCREEN) / (Raw_L - 1)), (((TempData.Max_y + 1) * Raw_H * ZOOM) / (Raw_H + 1)) + BAR, 5, RED);
    if (n == (TempData.Min_y) || n == (TempData.Min_y + 1)) LCD_DrawCross(((TempData.Min_x * SCREEN) / (Raw_L - 1)), (((TempData.Min_y + 1) * Raw_H * ZOOM) / (Raw_H + 1)) + BAR, 5, RGBToColor(60, 60, 255));
}

void Show_Text(uint8_t n)
{
    uint8_t mode;
    POINT_COLOR = WHITE;
    if (n == 0 || n == 1) {
        mode = n;
        LCD_MDA_ShowString(0, "MAX=", mode);
        LCD_MDA_ShowNum(32, TempData.Max / 100, 2, mode);
        LCD_MDA_ShowString(48, ".", mode);
        LCD_MDA_ShowNum(56, (TempData.Max % 100) / 10, 1, mode);
        LCD_MDA_ShowSymbol(64, TempSymbol, mode);
        LCD_MDA_ShowString(160, "MIN=", mode);
        LCD_MDA_ShowNum(192, TempData.Min / 100, 2, mode);
        LCD_MDA_ShowString(208, ".", mode);
        LCD_MDA_ShowNum(216, (TempData.Min % 100) / 10, 1, mode);
        LCD_MDA_ShowSymbol(224, TempSymbol, mode);
    }
    if (n == (Raw_H - 3) || n == (Raw_H - 2)) {
        mode = n - (Raw_H - 3);
        if (Select_State == 1) POINT_COLOR = RED;
        LCD_MDA_ShowString(0, "E=", mode);
        POINT_COLOR = WHITE;
        LCD_MDA_ShowNum(16, Emissivity / 100, 1, mode);
        LCD_MDA_ShowString(24, ".", mode);
        LCD_MDA_ShowNum(32, Emissivity / 10, 1, mode);
        LCD_MDA_ShowNum(40, Emissivity % 10, 1, mode);
        LCD_MDA_ShowString(160, "TAG=", mode);
        LCD_MDA_ShowNum(192, TempData.Target / 100, 2, mode);
        LCD_MDA_ShowString(208, ".", mode);
        LCD_MDA_ShowNum(216, (TempData.Target % 100) / 10, 1, mode);
        LCD_MDA_ShowSymbol(224, TempSymbol, mode);
    }
}

void Show_TempBilinearInter(uint8_t Location_x, uint8_t Location_y, TempDataTypeDef *Data)
{
    for (uint8_t j = 0; j < Raw_H - 1; j++) {
        Address_set(Location_x, Location_y + ZOOM * j, Location_x + SCREEN - 1, Location_y + ZOOM * j - 1);
        for (uint8_t i = 0; i < Raw_L - 1; i++) {
            for (uint8_t n = 0; n < ZOOM; n++) {
                for (uint8_t m = 0; m < ZOOM; m++) {
                    uint16_t Data_Zoom, Index;
                    uint8_t k1, k2, k3, k4;
                    if ((ZOOM * i + m) > (SCREEN - 1)) continue;
                    k1                       = (ZOOM - m) * (ZOOM - n);
                    k2                       = m * (ZOOM - n);
                    k3                       = (ZOOM - m) * n;
                    k4                       = m * n;
                    Data_Zoom                = (k1 * Data->PseColor[i][j] + k2 * Data->PseColor[i + 1][j] + k3 * Data->PseColor[i][j + 1] + k4 * Data->PseColor[i + 1][j + 1]) / (ZOOM * ZOOM);
                    Index                    = (n * SCREEN + ZOOM * i + m) * 2;
                    OLED_SendBuff[Index]     = PseColor[Data_Zoom].RGB_H;
                    OLED_SendBuff[Index + 1] = PseColor[Data_Zoom].RGB_L;
                }
            }
        }
        Show_Text(j);
        OLED_DMA_TransferLen(ZOOM * SCREEN * 2);
        OLED_DMA_Waite();
        if (j == (Raw_H / 2) && CROSS) LCD_DrawCross(SCREEN / 2, Raw_H * ZOOM / 2 + BAR, 5, WHITE);
        Show_MinAndMax(j);
    }
}

// 校验和检查
uint8_t MLX90640_CheckData(uint8_t *data)
{
    uint16_t sum = 0, length = 0, i = 0;
    uint16_t temp = 0;
    length        = ((uint16_t)RX_BUF[3] << 8) | RX_BUF[2] + 6;
    if (length > 1544) // 超过上传数据
        return 0;
    for (i = 0; i < length - 2; i = i + 2) {
        temp = ((uint16_t)RX_BUF[i + 1] << 8) | RX_BUF[i];
        sum += temp;
    }
    temp = ((uint16_t)RX_BUF[1543] << 8) | RX_BUF[1542];
    if (sum == temp) {
        memcpy(data, RX_BUF, length);
        return 1;
    } else
        return 0;
}

void MLX90640_SetEmissivity(uint8_t value)
{
    uint8_t sum = 0;
    USART1_Send_byte(0xA5);
    USART1_Send_byte(0x45);
    USART1_Send_byte(value);
    sum = 0xA5 + 0x45 + value;
    USART1_Send_byte(sum);
    Delay_ms(10);
}

void MLX90640_SendCMD(const uint8_t *CMD)
{
    USART1_Send_bytes((uint8_t *)CMD, 4);
    Delay_ms(10);
}

void MLX90640_SendInitCMD()
{
    MLX90640_SendCMD(CMD_AUTO); // 发送自动输出指令
    MLX90640_SendCMD(CMD_SAVE); // 保存
    MLX90640_SetEmissivity(95);
}

void MLX90640_Init()
{
    MLX90640_SendInitCMD();
    uart1_init(11520);
}
