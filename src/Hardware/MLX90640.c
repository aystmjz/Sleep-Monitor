#include "MLX90640.h"
uint8_t RX_BUF[1550] = {0};
uint8_t CMD_SAVE[4]  = {0xA5, 0x56, 0x02, 0xFD};
uint8_t CMD_AUTO[4]  = {0xA5, 0x35, 0x02, 0xDC};

TempDataTypeDef TempData;

// 伪彩 1
void Draw_TempPseColor(float Temp)
{
    uint8_t Value, colorR, colorG, colorB;
    if (Temp > Temp_MAX) Temp = Temp_MAX;
    if (Temp < Temp_MIN) Temp = Temp_MIN;
    Value  = (uint8_t)((255.0 * (Temp - Temp_MIN)) / (Temp_MAX - Temp_MIN));
    colorR = abs(0 - Value);
    colorG = abs(127 - Value);
    colorB = abs(255 - Value);
    LCD_WR_RGB(colorR, colorG, colorB);
}

extern u8 ref;
void Show_TempRaw(uint8_t Location_x, uint8_t Location_y) // 显示原始热力图
{
    Address_set(Location_x, Location_y, Location_x + Raw_L - 1, Location_y + Raw_H - 1); // 坐标设置
    for (uint8_t i = 0; i < Raw_H; i++) {
        for (uint8_t j = 0; j < Raw_L; j++) {
            Draw_TempPseColor(TempData.Raw[j][i]);
        }
    }
    ref = 0;
}

void Show_TempBilinearInter(uint8_t Location_x, uint8_t Location_y, TempDataTypeDef *Data)
{
    // Address_set(Location_x, Location_y, Location_x + (Raw_L-1)*ZOOM, Location_y + (Raw_H-1)*ZOOM);
    for (uint8_t j = 0; j < Raw_H - 1; j++) {
        for (uint8_t i = 0; i < Raw_L - 1; i++) {
            Address_set(Location_x+ZOOM * i, Location_y+ZOOM * j, Location_x + ZOOM * (i+1), Location_y +ZOOM * (j+1));
            for (uint8_t n = ZOOM * j; n <= ZOOM * (j + 1); n++) {
                for (uint8_t m = ZOOM * i; m <= ZOOM * (i + 1); m++) {
                    float k1, k2, k3, k4, Data_Zoom;
                    // if (m == ZOOM * i && n == ZOOM * j) continue;
                    // if (m == ZOOM * (i + 1) && n == ZOOM * j) continue;
                    // if (m == ZOOM * i && n == ZOOM * (j + 1)) continue;
                    // if (m == ZOOM * (i + 1) && n == ZOOM * (j + 1)) continue;
                    // if (m == ZOOM * (i + 1) && i != Raw_L - 2) continue;
                    // if (m == ZOOM * (j + 1) && j != Raw_H - 2) continue;
                    // x=m,y=n,x1=ZOOM*i,x2=ZOOM*(i+1),y1=ZOOM*j,y2=ZOOM*(j+1)
                    k1 = (ZOOM * (i + 1) - m) * (ZOOM * (j + 1) - n);
                    k2 = (m - ZOOM * i) * (ZOOM * (j + 1) - n);
                    k3 = (ZOOM * (i + 1) - m) * (n - ZOOM * j);
                    k4 = (m - ZOOM * i) * (n - ZOOM * j);
                    Data_Zoom = k1 * Data->Raw[i][j];
                    Data_Zoom += k2 * Data->Raw[i + 1][j];
                    Data_Zoom += k3 * Data->Raw[i][j + 1];
                    Data_Zoom += k4 * Data->Raw[i + 1][j + 1];
                    Data_Zoom /= (float)(ZOOM * ZOOM);
            //         char str[10];
            //          sprintf(str, "Max=%.2f \r\n", Data_Zoom);
            //  Debug_printf(str);
                    Draw_TempPseColor(Data_Zoom);
                    //Data->Zoom[m][n]
                }
            }
        }
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

void MLX90640_SendInitCMD()
{
    USART1_Send_bytes(CMD_AUTO, 4); // 发送自动输出指令
    Delay_ms(50);
    USART1_Send_bytes(CMD_SAVE, 4); // 保存
    Delay_ms(50);
}

void MLX90640_SendCMD(uint8_t *CMD)
{
    USART1_Send_bytes(CMD, 4);
    Delay_ms(50);
}

void MLX90640_Init()
{
    MLX90640_SendInitCMD();
    uart1_init(11520);
}
