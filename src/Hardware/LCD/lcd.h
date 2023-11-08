#ifndef LCD_H_
#define LCD_H_
#include "stm32f10x.h" // Device header
#include "DMA.h"
#include "sys.h"
#include "SPI.h"
#include "MLX90640.h"

#include "Delay.h"
#define LCD_W              240
#define LCD_H              240

#define TempSymbol         0
#define PercentSymbol      1
#define BatterySymbol1     2
#define BatterySymbol2     3
#define BatterySymbol3     14
#define BatterySymbol4     15
#define BatterySymbol5     16
#define EmissivitySymbol1  4
#define EmissivitySymbol2  5
#define EmissivitySymbol3  6



#define u8                 uint8_t
#define u16                uint16_t
#define u32                uint32_t

#define DC                 PAout(4) // DC

#define LCD_SEND_BUFF_LEN 6000

// LCD模式设置
// 0:4线串行模式
// 1:并行8080模式
#define LCD_MODE  0
#define SIZE       16
#define XLevelL    0x00
#define XLevelH    0x10
#define Max_Column 128
#define Max_Row    64
#define Brightness 0xFF
#define X_WIDTH    128
#define Y_WIDTH    64

#define LCD_CMD        0 // 写命令
#define LCD_DATA       1 // 写数据

extern u16 BACK_COLOR, POINT_COLOR,REFRESH_MODE; // 背景色，画笔色
extern u8 LCD_SendBuff[LCD_SEND_BUFF_LEN];

void LCD_DMA_ClearCounter(void);
void LCD_DMA_AddDate(u8 Date);
void LCD_DMA_Transfer(void);
void LCD_DMA_TransferLen(u16 DataLen);
void LCD_DMA_Waite(void);
void LCD_DMA_Init(void);

void LCD_GPIOInit(void);
void LCD_WriteByte(u8 Byte);
void LCD_WriteDATA(u16 Data);
void LCD_WriteREG(u8 Data);
void LCD_Write_REG_DATA(u8 reg, u16 Data);
void LCD_SetAddress(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Init(void);
void LCD_Clear(u16 color);
void showhanzi(unsigned int x, unsigned int y, unsigned char index);
void LCD_ShowSymbol(u16 x, u16 y, u8 index);
void LCD_DrawCross(u16 x, u16 y, u8 size, u16 color);
void LCD_DrawPoint(u16 x, u16 y);
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0, u16 y0, u8 r);
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 mode);
u32 mypow(u8 m, u8 n);

void LCD_MDA_ShowSymbol(u16 x, u8 index, u8 site);
void LCD_MDA_ShowChar(u16 x, u8 num, u8 site);
void LCD_MDA_ShowNum(u16 x, u32 num, u8 len, u8 site);
void LCD_MDA_ShowString(u16 x, const u8 *p, u8 site);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len);
void LCD_ShowString(u16 x, u16 y, const u8 *p);

u16 RGBToColor(u8 colorR, u8 colorG, u8 colorB);


// 画笔颜色
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40 // 棕色
#define BRRED   0XFC07 // 棕红色
#define GRAY    0X8430 // 灰色
// GUI颜色

#define DARKBLUE  0X01CF // 深蓝色
#define LIGHTBLUE 0X7D7C // 浅蓝色
#define GRAYBLUE  0X5458 // 灰蓝色
// 以上三色为PANEL的颜色

#define LIGHTGREEN 0X841F // 浅绿色
#define LGRAY      0XC618 // 浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE  0XA651 // 浅灰蓝色(中间层颜色)
#define LBBLUE     0X2B12 // 浅棕蓝色(选择条目的反色)

#endif
