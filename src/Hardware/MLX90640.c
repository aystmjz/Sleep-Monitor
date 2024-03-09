#include "MLX90640.h"

const uint8_t CMD_SAVE[4]   = {0xA5, 0x56, 0x02, 0xFD};
const uint8_t CMD_AUTO[4]   = {0xA5, 0x35, 0x02, 0xDC};
const uint8_t CMD_MANUAL[4] = {0xA5, 0x35, 0x01, 0xDB};
uint8_t RX_BUF[1550]        = {0};
uint8_t Emissivity          = 95;
extern uint8_t Select_State;

const char *MQTT_Topic = "aystmjz/topic/hxd";

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
        Color             = LCD_RGBToDATA(colorR, colorG, colorB);
        PseColor[n].RGB_H = Color >> 8;
        PseColor[n].RGB_L = Color;
        // PseColor[n].RGB    = Color;
        // PseColor[n].colorR = colorR;
        // PseColor[n].colorG = colorG;
        // PseColor[n].colorB = colorB;
    }
    // sprintf(strc, "colorR=%.2d colorG=%.2d colorB=%.2d \r\n", colorR, colorG, colorB);
    // Debug_printf(strc);
    // Delay_ms(10);
}

void Show_PseColorBar(uint8_t x, uint8_t y)
{
    LCD_SetAddress(x, y, x + SCREEN - 1, y + BAR - 1); // 坐标设置
    for (uint8_t n = 0; n < (BAR / ZOOM); n++) {
        LCD_DMA_ClearCounter();
        for (uint8_t i = 0; i < ZOOM; i++) {
            for (uint8_t Value = 10; Value < 250; Value++) {
                LCD_DMA_AddDate(PseColor[Value].RGB_H);
                LCD_DMA_AddDate(PseColor[Value].RGB_L);
            }
        }
        LCD_DMA_Transfer();
        LCD_DMA_Waite();
    }
}

void Show_TempRaw(uint8_t x, uint8_t y) // 显示原始热力图
{
    LCD_SetAddress(x, y, x + Raw_L - 1, y + Raw_H - 1); // 坐标设置
    LCD_DMA_ClearCounter();
    for (uint8_t i = 0; i < Raw_H; i++) {
        for (uint8_t j = 0; j < Raw_L; j++) {
            LCD_DMA_AddDate(PseColor[TempData.PseColor[j][i]].RGB_H);
            LCD_DMA_AddDate(PseColor[TempData.PseColor[j][i]].RGB_L);
        }
    }
    LCD_DMA_Transfer();
    LCD_DMA_Waite();
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

        LCD_MDA_ShowString(96, "FPS ", mode);
        LCD_MDA_ShowNum(128, FPS_MLX90640, 2, mode);
        // LCD_MDA_ShowNum(144, Remote_GetCommand(), 2, mode);

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
        LCD_MDA_ShowString(64, "AVG=", mode);
        LCD_MDA_ShowNum(96, TempData.Average / 100, 2, mode);
        LCD_MDA_ShowString(112, ".", mode);
        LCD_MDA_ShowNum(120, (TempData.Average % 100) / 10, 1, mode);
        LCD_MDA_ShowSymbol(128, TempSymbol, mode);
        LCD_MDA_ShowString(160, "TAG=", mode);
        LCD_MDA_ShowNum(192, TempData.Target / 100, 2, mode);
        LCD_MDA_ShowString(208, ".", mode);
        LCD_MDA_ShowNum(216, (TempData.Target % 100) / 10, 1, mode);
        LCD_MDA_ShowSymbol(224, TempSymbol, mode);
    }
}

void Show_TempBilinearInter(uint8_t x, uint8_t y, TempDataTypeDef *Data)
{
    for (uint8_t j = 0; j < Raw_H - 1; j++) {
        LCD_SetAddress(x, y + ZOOM * j, x + SCREEN - 1, y + ZOOM * j - 1);
        for (uint8_t i = 0; i < Raw_L - 1; i++) {
            for (uint8_t n = 0; n < ZOOM; n++) {
                for (uint8_t m = 0; m < ZOOM; m++) {
                    uint16_t Data_Zoom, Index;
                    uint8_t k1, k2, k3, k4;
                    if ((ZOOM * i + m) > (SCREEN - 1)) continue;
                    k1                      = (ZOOM - m) * (ZOOM - n);
                    k2                      = m * (ZOOM - n);
                    k3                      = (ZOOM - m) * n;
                    k4                      = m * n;
                    Data_Zoom               = (k1 * Data->PseColor[i][j] + k2 * Data->PseColor[i + 1][j] + k3 * Data->PseColor[i][j + 1] + k4 * Data->PseColor[i + 1][j + 1]) / (ZOOM * ZOOM);
                    Index                   = (n * SCREEN + ZOOM * i + m) * 2;
                    LCD_SendBuff[Index]     = PseColor[Data_Zoom].RGB_H;
                    LCD_SendBuff[Index + 1] = PseColor[Data_Zoom].RGB_L;
                }
            }
        }
        Show_Text(j);
        LCD_DMA_TransferLen(ZOOM * SCREEN * 2);
        LCD_DMA_Waite();
        if (j == (Raw_H / 2) && CROSS) LCD_DrawCross(SCREEN / 2, Raw_H * ZOOM / 2 + BAR, 5, WHITE);
        Show_MinAndMax(j);
    }
    // char temp[10];
    // sprintf(temp,"x1=%d  y1=%d  x2=%d  y2=%d  \r\n",TempData.Face_Left,TempData.Face_Up,TempData.Face_Right,TempData.Face_Down);
    // Debug_printf(temp);
    if (TempData.Face_Flag) {
        LCD_DrawRectangle(TempData.Face_Left * ZOOM, TempData.Face_Up * ZOOM + BAR, TempData.Face_Right * ZOOM, TempData.Face_Down * ZOOM + BAR);
        LCD_DrawCross(((TempData.Face_x * SCREEN) / (Raw_L - 1)), (((TempData.Max_y + 1) * Raw_H * ZOOM) / (Raw_H + 1)) + BAR, 5, WHITE);
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

void MLX90640_SendInitCMD(void)
{
    MLX90640_SendCMD(CMD_AUTO); // 发送自动输出指令
    MLX90640_SendCMD(CMD_SAVE); // 保存
}

void MLX90640_TurnOff(void)
{
    MLX90640_SendCMD(CMD_MANUAL); // 发送手动输出指令
    MLX90640_SendCMD(CMD_SAVE);   // 保存
}

void MLX90640_Init(void)
{
    MLX90640_SendInitCMD();
    uart1_init(11520);
}

uint8_t MLX90640_RefreshData(void)
{
    int8_t face_up = Raw_H - 1, face_down = 0, face_left = Raw_L - 1, face_right = 0, face_flag = 0;
    uint16_t face_num = 0, face_x_sum = 0, face_y_sum = 0;
    uint32_t temp_sum = 0, face_temp_sum = 0;

    int16_t temperature, max = -1000, min = 10000;
    uint8_t y_p = 0, data_buf[1550];
    if (MLX90640_CheckData(data_buf)) {
        for (int8_t y = 23; y >= 0; y--) {
            uint8_t x_p = 0;
            for (uint8_t x = 4; x < 64 + 4; x += 2) {
                temperature = ((int16_t)data_buf[x + 1 + 64 * y] << 8 | data_buf[x + 64 * y]);
                if (temperature < min) {
                    min            = temperature;
                    TempData.Min_x = x_p;
                    TempData.Min_y = y_p;
                }
                if (temperature > max) {
                    max            = temperature;
                    TempData.Max_x = x_p;
                    TempData.Max_y = y_p;
                }
                if (temperature > Temp_Face) {
                    if (x_p < face_left) face_left = x_p;
                    if (x_p > face_right) face_right = x_p;
                    if (y_p < face_up) face_up = y_p;
                    if (y_p > face_down) face_down = y_p;
                    face_temp_sum += temperature;
                    face_x_sum += x_p;
                    face_y_sum += y_p;
                    face_num++;
                    face_flag = 1;
                }
                temp_sum += temperature;
                TempData.Raw[x_p][y_p] = temperature;
                if (temperature > Temp_MAX) temperature = Temp_MAX;
                if (temperature < Temp_MIN) temperature = Temp_MIN;
                TempData.PseColor[x_p][y_p] = ((temperature - Temp_MIN) * 255) / ((Temp_MAX - Temp_MIN));
                x_p++;
            }
            y_p++;
        }
        TempData.Face_Average = face_temp_sum / face_num;
        TempData.Face_x       = face_x_sum / face_num;
        TempData.Face_y       = face_y_sum / face_num;
        TempData.Face_Up      = face_up;
        TempData.Face_Down    = face_down;
        TempData.Face_Left    = face_left;
        TempData.Face_Right   = face_right;
        TempData.Face_Flag    = face_flag;

        TempData.Max     = max;
        TempData.Min     = min;
        TempData.Average = temp_sum / (32 * 24);
        TempData.Target  = (TempData.Raw[16][12] + TempData.Raw[15][12] + TempData.Raw[16][11] + TempData.Raw[15][11]) / 4;
        return 1;
    }
    return 0;
}
