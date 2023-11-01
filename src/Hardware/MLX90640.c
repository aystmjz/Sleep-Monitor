#include "MLX90640.h"
uint8_t RX_BUF[1550] = {0};
uint8_t CMD_SAVE[4]  = {0xA5, 0x56, 0x02, 0xFD};
uint8_t CMD_AUTO[4]  = {0xA5, 0x35, 0x02, 0xDC};
uint16_t Conter_     = 0;
TempDataTypeDef TempData;
ColorTypeDef PseColor[256];
uint8_t BI_K1[ZOOM * ZOOM];
uint8_t BI_K2[ZOOM * ZOOM];
uint8_t BI_K3[ZOOM * ZOOM];
uint8_t BI_K4[ZOOM * ZOOM];

void BilinearInter_Init()
{
    for (uint8_t n = 0; n < ZOOM; n++) {
        for (uint8_t m = 0; m < ZOOM; m++) {
            uint16_t Data_Zoom;
            uint8_t k1, k2, k3, k4;
            k1 = (ZOOM - m) * (ZOOM - n);
            k2 = (m) * (ZOOM - n);
            k3 = (ZOOM - m) * (n);
            k4 = (m) * (n);
        }
    }
}

uint16_t LCD_RGBToDATA(uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t Color;
    Color = ((colorR & 0xF8) << 8) | ((colorG & 0xFE) << 3) | ((colorB & 0xF8) >> 3);
    return Color;
}

void TempPseColor_Init()
{
    uint8_t colorR, colorG, colorB;
    uint16_t Color;
    for (int16_t Value = 0; Value < 256; Value++) {
        colorR=abs(0-Value);
	    colorG=abs(127-Value);
		colorB=abs(255-Value);	
        /*if ((Value >= 0) && (Value <= 63)) {
            colorR = 0;
            colorG = 0;
            colorB = round(Value / 64 * 255);
        } else if ((Value >= 64) && (Value <= 127)) {
            colorR = 0;
            colorG = round((Value - 64) / 64 * 255);
            colorB = round((127 - Value) / 64 * 255);
        } else if ((Value >= 128) && (Value <= 191)) {
            colorR = round((Value - 128) / 64 * 255);
            colorG = 255;
            colorB = 0;
        } else if ((Value >= 192) && (Value <= 255)) {
            colorR = 255;
            colorG = round((255 - Value) / 64 * 255);
            colorB = 0;
        }*/
        Color                  = LCD_RGBToDATA(colorR, colorG, colorB);
        PseColor[Value].RGB_H  = (uint8_t)Color >> 8;
        PseColor[Value].RGB_L  = (uint8_t)Color;
        PseColor[Value].RGB    = Color;
        PseColor[Value].colorR = colorR;
        PseColor[Value].colorG = colorG;
        PseColor[Value].colorB = colorB;
    }
}
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
    Conter_ = 0;
    for (uint8_t i = 0; i < Raw_H; i++) {
        for (uint8_t j = 0; j < Raw_L; j++) {
            Draw_TempPseColor(TempData.Raw[j][i]);
        }
    }
    OLED_DMA_Transfer((--Conter_) * 2);
    while (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET) {}
    DMA_ClearFlag(DMA1_FLAG_TC3);
    ref = 0;
}
uint16_t abc = 0;
float t[20];
char strc[100];
void Show_TempBilinearInter(uint8_t Location_x, uint8_t Location_y, TempDataTypeDef *Data)
{
    // Address_set(Location_x, Location_y, Location_x + (Raw_L-1)*ZOOM, Location_y + (Raw_H-1)*ZOOM);
    abc++;
    abc %= 5000;
    for (uint8_t j = 0; j < Raw_H - 1; j++) {
        // Debug_printf("Address_set");
        if (j) {
            Address_set(Location_x, Location_y + ZOOM * (j - 1), Location_x + 239, Location_y + ZOOM * ((j - 1) + 1) - 1);
            OLED_DMA_Transfer();
            //OLED_DMA_Waite();
        }

        // OLED_DMA_ClearCounter();

        for (uint8_t i = 0; i < Raw_L - 1; i++) {
            for (uint8_t n = 0; n < ZOOM; n++) {
                for (uint8_t m = 0; m < ZOOM; m++) {
                    uint16_t Data_Zoom;
                    uint8_t k1, k2, k3, k4;
                    // x=m,y=n,x1=ZOOM*i,x2=ZOOM*(i+1),y1=ZOOM*j,y2=ZOOM*(j+1)

                    k1        = (ZOOM - m) * (ZOOM - n);
                    k2        = m * (ZOOM - n);
                    k3        = (ZOOM - m) * n;
                    k4        = m * n;
                    Data_Zoom = (k1 * Data->PseColor[i][j] + k2 * Data->PseColor[i + 1][j] + k3 * Data->PseColor[i][j + 1] + k4 * Data->PseColor[i + 1][j + 1]) / (ZOOM * ZOOM);
                    // sprintf(strc, "k1=%d k2=%d k3=%d k4=%d Data_Zoom=%d\r\n", k1, k2, k3, k4, Data->PseColor[i][j + 1]);
                    // Delay_ms(20);
                    // Debug_printf(strc);
                     OLED_SendBuff[(n * 240 + ZOOM * i + m) * 2]     = PseColor[Data_Zoom].RGB_H;
                     OLED_SendBuff[(n * 240 + ZOOM * i + m) * 2 + 1] = PseColor[Data_Zoom].RGB_L;
                    //  Data_Zoom += k2 * Data->PseColor[i + 1][j];
                    //  Data_Zoom += k3 * Data->PseColor[i][j + 1];
                    //  Data_Zoom += k4 * Data->PseColor[i + 1][j + 1];
                    //   sprintf(strc, "k1=%d k2=%d k3=%d k4=%d Data=%d Data_Zoom=%d \r\n", k1, k2, k3, k4,Data->PseColor[i][j],Data_Zoom);
                    //   Delay_ms(5);
                    //   Debug_printf(strc);
                    //  Data_Zoom /= ZOOM * ZOOM;
                    //   Data_Zoom=(Data->Raw[i][j]+Data->Raw[i + 1][j]+Data->Raw[i][j + 1]+Data->Raw[i + 1][j + 1])/4;

                    // //LCD_WR_DATA(Color);

                    // Draw_TempPseColor(&Data_Zoom);
                    //  Data->Zoom[m][n]
                }
            }
            // DMA_ClearFlag(DMA1_FLAG_TC3);
        }
        // Delay_ms(10);
        // if(j)
        // OLED_DMA_Waite();
    }
     Address_set(Location_x, Location_y + ZOOM *  (Raw_H - 3) , Location_x + 319, Location_y +ZOOM* ((Raw_H - 3) + 1)-1);
     OLED_DMA_Transfer();
     OLED_DMA_Waite();
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
