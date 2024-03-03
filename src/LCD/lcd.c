#include "lcd.h"
#include "lcdfont.h"

static u16 LCD_SendCounter = 0;

u16 BACK_COLOR, POINT_COLOR, REFRESH_MODE;
u8 LCD_SendBuff[LCD_SEND_BUFF_LEN];

void LCD_DMA_ClearCounter(void)
{
    LCD_SendCounter = 0;
}

void LCD_DMA_AddDate(u8 Date)
{
    LCD_SendBuff[LCD_SendCounter++] = Date;
    LCD_SendCounter %= LCD_SEND_BUFF_LEN;
}

void LCD_DMA_Transfer(void)
{
    DC = 1;
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA1_Transfer(DMA1_Channel3, LCD_SendCounter);
}

void LCD_DMA_TransferLen(u16 DataLen)
{
    DC = 1;
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA1_Transfer(DMA1_Channel3, DataLen);
}

void LCD_DMA_Waite(void)
{
    DMA_ClearFlag(DMA1_FLAG_TC3);
    while (DMA_GetFlagStatus(DMA1_FLAG_TC3) != SET) {}
    DMA_ClearFlag(DMA1_FLAG_TC3);
}

void LCD_DMA_Init(void)
{
    DMA1_Init(DMA1_Channel3, (u32)&SPI1->DR, (u32)LCD_SendBuff, LCD_SEND_BUFF_LEN);
}

void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能A端口时钟
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void LCD_WriteByte(u8 Byte)
{
    DC = 1;
    LCD_SPI_ReadWriteByte(Byte);
}
void LCD_WriteDATA(u16 Data)
{
    DC = 1;
    LCD_SPI_ReadWriteByte(Data >> 8);
    LCD_SPI_ReadWriteByte(Data);
}
void LCD_WriteREG(u8 Data)
{
    DC = 0;
    LCD_SPI_ReadWriteByte(Data);
}
void LCD_Write_REG_DATA(u8 reg, u16 Data)
{
    LCD_WriteREG(reg);
    LCD_WriteDATA(Data);
}

void LCD_SetAddress(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_WriteREG(0x2a);
    LCD_WriteByte(x1 >> 8);
    LCD_WriteByte(x1);
    LCD_WriteByte(x2 >> 8);
    LCD_WriteByte(x2);

    LCD_WriteREG(0x2b);
    LCD_WriteByte(y1 >> 8);
    LCD_WriteByte(y1);
    LCD_WriteByte(y2 >> 8);
    LCD_WriteByte(y2);

    LCD_WriteREG(0x2C);
}

void LCD_Init(void)
{
    LCD_GPIOInit();
    LCD_SPI_Init();
    LCD_DMA_Init();

    // LCD_CS_Clr();  //打开片选使能
    // LCD_RST_Clr();
    // Delay_ms(20);
    // LCD_RST_Set();
    // Delay_ms(20);
    // LCD_BLK_Set();

    // Start Initial Sequence //
    LCD_WriteREG(0x36);
    LCD_WriteByte(0x00);

    LCD_WriteREG(0x3A);
    LCD_WriteByte(0x05);

    LCD_WriteREG(0xB2);
    LCD_WriteByte(0x0C);
    LCD_WriteByte(0x0C);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x33);
    LCD_WriteByte(0x33);

    LCD_WriteREG(0xB7);
    LCD_WriteByte(0x35);

    LCD_WriteREG(0xBB);
    LCD_WriteByte(0x19);

    LCD_WriteREG(0xC0);
    LCD_WriteByte(0x2C);

    LCD_WriteREG(0xC2);
    LCD_WriteByte(0x01);

    LCD_WriteREG(0xC3);
    LCD_WriteByte(0x12);

    LCD_WriteREG(0xC4);
    LCD_WriteByte(0x20);

    LCD_WriteREG(0xC6);
    LCD_WriteByte(0x0F);

    LCD_WriteREG(0xD0);
    LCD_WriteByte(0xA4);
    LCD_WriteByte(0xA1);

    LCD_WriteREG(0xE0);
    LCD_WriteByte(0xD0);
    LCD_WriteByte(0x04);
    LCD_WriteByte(0x0D);
    LCD_WriteByte(0x11);
    LCD_WriteByte(0x13);
    LCD_WriteByte(0x2B);
    LCD_WriteByte(0x3F);
    LCD_WriteByte(0x54);
    LCD_WriteByte(0x4C);
    LCD_WriteByte(0x18);
    LCD_WriteByte(0x0D);
    LCD_WriteByte(0x0B);
    LCD_WriteByte(0x1F);
    LCD_WriteByte(0x23);

    LCD_WriteREG(0xE1);
    LCD_WriteByte(0xD0);
    LCD_WriteByte(0x04);
    LCD_WriteByte(0x0C);
    LCD_WriteByte(0x11);
    LCD_WriteByte(0x13);
    LCD_WriteByte(0x2C);
    LCD_WriteByte(0x3F);
    LCD_WriteByte(0x44);
    LCD_WriteByte(0x51);
    LCD_WriteByte(0x2F);
    LCD_WriteByte(0x1F);
    LCD_WriteByte(0x1F);
    LCD_WriteByte(0x20);
    LCD_WriteByte(0x23);

    LCD_WriteREG(0x21);

    LCD_WriteREG(0x11);
    // Delay (120);

    LCD_WriteREG(0x29);
}

/// @brief 清屏函数
/// @param color 要清屏的填充色
void LCD_Clear(u16 color)
{
    u16 i, j;
    LCD_SetAddress(0, 0, LCD_W - 1, LCD_H - 1);
    for (i = 0; i < LCD_W; i++) {
        for (j = 0; j < LCD_H; j++) {
            LCD_WriteDATA(color);
        }
    }
}

// 在指定位置显示一个汉字(32*33大小)
// dcolor为内容颜色，gbcolor为背静颜色
void showhanzi(unsigned int x, unsigned int y, unsigned char index)
{
    unsigned char i, j;
    unsigned char *temp = hanzi;
    LCD_SetAddress(x, y, x + 16, y + 16); // 设置区域
    temp += index * 128;
    for (j = 0; j < 128; j++) {
        for (i = 0; i < 8; i++) {
            if ((*temp & (1 << i)) != 0) {
                LCD_WriteDATA(POINT_COLOR);
            } else {
                LCD_WriteDATA(BACK_COLOR);
            }
        }
        temp++;
    }
}

void LCD_ShowSymbol(u16 x, u16 y, u8 index)
{
    LCD_SetAddress(x, y, x + 15, y + 15);
    for (u8 n = 0; n < 32; n++) {
        for (u8 i = 0; i < 8; i++) {
            if ((Symbol[index][n] & (0x01 << i))) {
                LCD_WriteDATA(POINT_COLOR);
            } else {
                LCD_WriteDATA(BACK_COLOR);
            }
        }
    }
}

void LCD_DrawCross(u16 x, u16 y, u8 size, u16 color)
{
    LCD_SetAddress(x - size, y, x + size, y);
    for (int i = 0; i < (2 * size + 1); i++) {
        LCD_WriteDATA(color);
    }
    LCD_SetAddress(x, y - size, x, y + size);
    for (int i = 0; i < (2 * size + 1); i++) {
        LCD_WriteDATA(color);
    }
}

/// @brief 画点
/// @param x 点x坐标
/// @param y 点y坐标
void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_SetAddress(x, y, x, y);
    LCD_WriteDATA(POINT_COLOR);
}

/// @brief 在指定区域内填充指定颜色
/// @param xsta x开始坐标
/// @param ysta y开始坐标
/// @param xend x结束坐标
/// @param yend y结束坐标
/// @param color 颜色
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
    u16 i, j;
    LCD_SetAddress(xsta, ysta, xend, yend); // 设置光标位置
    for (i = ysta; i <= yend; i++) {
        for (j = xsta; j <= xend; j++) LCD_WriteDATA(color); // 设置光标位置
    }
}

/// @brief 画线
/// @param x1,y1 起点坐标
/// @param x2,y2 终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow    = x1;
    uCol    = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else {
        incx    = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else {
        incy    = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++) // 画线输出
    {
        LCD_DrawPoint(uRow, uCol); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}
// 画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    if(x2<x1)x2=x1;
    if(y2<y1)y2=y1;
    if(x1>=SCREEN)x1=SCREEN-2;//矩形左右限制
    if(x2>=SCREEN)x2=SCREEN-2;
    LCD_SetAddress(x1, y1, x2, y1);
    for (uint8_t i = 0; i < (x2-x1); i++) {
        LCD_WriteDATA(POINT_COLOR);
    }
    LCD_SetAddress(x1, y1, x1, y2);
    for (uint8_t i = 0; i < (y2-y1); i++) {
        LCD_WriteDATA(POINT_COLOR);
    }
    LCD_SetAddress(x1, y2, x2, y2);
    for (uint8_t i = 0; i < (x2-x1); i++) {
        LCD_WriteDATA(POINT_COLOR);
    }
    LCD_SetAddress(x2, y1, x2, y2);
    for (uint8_t i = 0; i < (y2-y1); i++) {
        LCD_WriteDATA(POINT_COLOR);
    }

}

/// @brief 在指定位置画一个指定大小的圆
/// @param x,y 中心点
/// @param r 半径
void Draw_Circle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a  = 0;
    b  = r;
    di = 3 - (r << 1); // 判断下个点位置的标志
    while (a <= b) {
        LCD_DrawPoint(x0 - b, y0 - a); // 3
        LCD_DrawPoint(x0 + b, y0 - a); // 0
        LCD_DrawPoint(x0 - a, y0 + b); // 1
        LCD_DrawPoint(x0 - b, y0 - a); // 7
        LCD_DrawPoint(x0 - a, y0 - b); // 2
        LCD_DrawPoint(x0 + b, y0 + a); // 4
        LCD_DrawPoint(x0 + a, y0 - b); // 5
        LCD_DrawPoint(x0 + a, y0 + b); // 6
        LCD_DrawPoint(x0 - b, y0 + a);
        a++;
        // 使用Bresenham算法画圆
        if (di < 0)
            di += 4 * a + 6;
        else {
            di += 10 + 4 * (a - b);
            b--;
        }
        LCD_DrawPoint(x0 + a, y0 + b);
    }
}
// 在指定位置显示一个字符

// num:要显示的字符:" "--->"~"
// mode:叠加方式(1)还是非叠加方式(0)
// 在指定位置显示一个字符

// num:要显示的字符:" "--->"~"

// mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 mode)
{
    u8 temp;
    u8 pos, t;
    u16 x0        = x;
    u16 colortemp = POINT_COLOR;
    if (x > LCD_W - 16 || y > LCD_H - 16) return;
    // 设置窗口
    num = num - ' ';                             // 得到偏移后的值
    LCD_SetAddress(x, y, x + 8 - 1, y + 16 - 1); // 设置光标位置
    if (!mode)                                   // 非叠加方式
    {
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[(u16)num * 16 + pos]; // 调用1608字体
            for (t = 0; t < 8; t++) {
                if (temp & 0x01)
                    POINT_COLOR = colortemp;
                else
                    POINT_COLOR = BACK_COLOR;
                LCD_WriteDATA(POINT_COLOR);
                temp >>= 1;
                x++;
            }
            x = x0;
            y++;
        }
    } else // 叠加方式
    {
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[(u16)num * 16 + pos]; // 调用1608字体
            for (t = 0; t < 8; t++) {
                if (temp & 0x01) LCD_DrawPoint(x + t, y + pos); // 画一个点
                temp >>= 1;
            }
        }
    }
    POINT_COLOR = colortemp;
}

// m^n函数
u32 mypow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--) result *= m;
    return result;
}

void LCD_MDA_ShowSymbol(u16 x, u8 index, u8 site)
{
    u8 temp, flag = 0;
    for (u8 y = 0; y < 16; y++) {
        temp = Symbol[index][y + site * 16];
        for (u8 n = 0; n < 8; n++) {
            if (temp & 0x01) {
                LCD_SendBuff[(y / 2 * SCREEN + flag * 8 + x + n) * 2]     = POINT_COLOR >> 8;
                LCD_SendBuff[(y / 2 * SCREEN + flag * 8 + x + n) * 2 + 1] = (u8)POINT_COLOR;
            }
            temp >>= 1;
        }
        flag++;
        flag %= 2;
    }
}

void LCD_MDA_ShowChar(u16 x, u8 num, u8 site)
{
    u8 temp;
    num -= ' ';
    for (u8 y = 0; y < 8; y++) {
        temp = asc2_1608[(u16)num * 16 + y + site * 8]; // 调用1608字体
        for (u8 n = 0; n < 8; n++) {
            if (temp & 0x01) {
                LCD_SendBuff[(y * SCREEN + x + n) * 2]     = POINT_COLOR >> 8;
                LCD_SendBuff[(y * SCREEN + x + n) * 2 + 1] = (u8)POINT_COLOR;
            }
            temp >>= 1;
        }
    }
}

void LCD_MDA_ShowNum(u16 x, u32 num, u8 len, u8 site)
{
    u8 t, temp;
    u8 enshow = 0;
    num       = (u16)num;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_MDA_ShowChar(x + 8 * t, ' ', site);
                continue;
            } else
                enshow = 1;
        }
        LCD_MDA_ShowChar(x + 8 * t, temp + 48, site);
    }
}

void LCD_MDA_ShowString(u16 x, const u8 *p, u8 site)
{
    while (*p != '\0') {
        LCD_MDA_ShowChar(x, *p, site);
        x += 8;
        p++;
    }
}

/// @brief 显示数字
/// @param x,y 起点坐标
/// @param num 数值(0~4294967295);
/// @param len 数字的位数
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len)
{
    u8 t, temp;
    u8 enshow = 0;
    num       = (u16)num;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + 8 * t, y, ' ', REFRESH_MODE);
                continue;
            } else
                enshow = 1;
        }
        LCD_ShowChar(x + 8 * t, y, temp + 48, REFRESH_MODE);
    }
}

/// @brief 显示字符串(用16字体)
/// @param x,y 起点坐标
/// @param *p 字符串起始地址
void LCD_ShowString(u16 x, u16 y, const u8 *p)
{
    while (*p != '\0') {
        if (x > LCD_W - 16) {
            x = 0;
            y += 16;
        }
        if (y > LCD_H - 16) {
            y = x = 0;
            LCD_Clear(RED);
        }
        LCD_ShowChar(x, y, *p, REFRESH_MODE);
        x += 8;
        p++;
    }
}

#define Image_X 132
#define Image_Y 150
#define Image_L 50

const unsigned char gImage_qmx[1];

void showimage() // 显示40*40图片
{

    LCD_SetAddress(Image_L, Image_L, Image_L + Image_X - 1, Image_L + Image_Y - 1); // 坐标设置
    for (int i = 0; i < 39600 / 2; i++) {
        LCD_WriteByte(gImage_qmx[i * 2 + 1]);
        LCD_WriteByte(gImage_qmx[i * 2]);
    }
}

u16 RGBToColor(u8 colorR, u8 colorG, u8 colorB)
{
    return ((colorR & 0xF8) << 8) | ((colorG & 0xFE) << 3) | ((colorB & 0xF8) >> 3);
}
