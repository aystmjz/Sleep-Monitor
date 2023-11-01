#include "oled.h"
#include "oledfont.h"

u16 BACK_COLOR, POINT_COLOR;

uint8_t OLED_SendBuff[OLED_SEND_BUFF_LEN];
static uint16_t OLED_SendCounter=0;


void OLED_DMA_ClearCounter()
{
    OLED_SendCounter=0;
}

void OLED_DMA_AddDate(uint8_t Date)
{
   OLED_SendBuff[OLED_SendCounter++]=Date;
   OLED_SendCounter%=OLED_SEND_BUFF_LEN;
}


void OLED_DMA_Transfer()
{
    DC=1;
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    DMA1_Transfer(DMA1_Channel3, ZOOM*240*2);
    //OLED_SendCounter++;
}

void OLED_DMA_TransferLen(uint16_t DataLen)
{
    DC=1;
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    DMA1_Transfer(DMA1_Channel3, DataLen);
}

void OLED_DMA_Waite()
{
    while (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET) {}
    DMA_ClearFlag(DMA1_FLAG_TC3);
}

void OLED_DMA_Init(void)
{
	DMA1_Init(DMA1_Channel3, (uint32_t)&SPI1->DR, (uint32_t)OLED_SendBuff, OLED_SEND_BUFF_LEN);
}

void OLED_DCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能A端口时钟
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);            // 初始化GPIOD3,6
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}


void writeData(u8 data)
{
    DC = 1;
    OLED_SPI_ReadWriteByte(data);
}

void writeCommand(u8 cmd)
{
    DC = 0;
    OLED_SPI_ReadWriteByte(cmd);
}
/*
void fillScreen(u16 color)
{
    u16 i, j;
    // DC = 0;
    writeCommand(0x2A);
    writeData(0);
    writeData(0);
    writeData(0);
    writeData(240);
    writeCommand(0X2B);
    writeData(0);
    writeData(0);
    writeData(0X01);
    writeData(0X40);
    writeCommand(0X2C);
    DC = 1;
    for (j = 0; j < 480;) {
        SendBuff[j]     = color >> 8;
        SendBuff[j + 1] = color;
        j += 2;
    }

    for (i = 0; i < 320; i++) {

        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); //????1?DMA??
        MYDMA_Enable(DMA1_Channel3);

        while (1) {
            if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET) // µÈ´ýÍ¨µÀ4´«ÊäÍê³É
            {
                DMA_ClearFlag(DMA1_FLAG_TC3); // Çå³ýÍ¨µÀ4´«ÊäÍê³É±êÖ¾
                break;
            }
        }
    }
}
*/

void LCD_Writ_Bus(char dat)                                // 串行数据写入
{
    u8 i;

    for (i = 0; i < 8; i++) {
        OLED_SCLK_Clr();
        if (dat & 0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat <<= 1;
    }
}

// void writeData(u8 data)
// {
//     DC = 1;
//     OLED_SPI_ReadWriteByte(data);
// }

// void writeCommand(u8 cmd)
// {
//     DC = 0;
//     OLED_SPI_ReadWriteByte(cmd);
// }

void LCD_WR_DATA8(char da) // 发送数据-8位参数
{                          // OLED_CS_Clr();
    DC = 1;
    OLED_SPI_ReadWriteByte(da);
    // OLED_CS_Set();
}
void LCD_WR_DATA(int da)
{ //	OLED_CS_Clr();
	DC = 1;
    OLED_SPI_ReadWriteByte(da >> 8);
    OLED_SPI_ReadWriteByte(da);
    // OLED_CS_Set();
}
void LCD_WR_REG(char da)
{ //	OLED_CS_Clr();
    OLED_DC_Clr();
    OLED_SPI_ReadWriteByte(da);
    // OLED_CS_Set();
}
void LCD_WR_REG_DATA(int reg, int da)
{ // OLED_CS_Clr();
    LCD_WR_REG(reg);
    LCD_WR_DATA(da);
    // OLED_CS_Set();
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_WR_REG(0x2a);
    LCD_WR_DATA8(x1 >> 8);
    LCD_WR_DATA8(x1);
    LCD_WR_DATA8(x2 >> 8);
    LCD_WR_DATA8(x2);

    LCD_WR_REG(0x2b);
    LCD_WR_DATA8(y1 >> 8);
    LCD_WR_DATA8(y1);
    LCD_WR_DATA8(y2 >> 8);
    LCD_WR_DATA8(y2);

    LCD_WR_REG(0x2C);
}

void Lcd_Init(void)
{
    OLED_DCInit();
	OLED_SPI_Init();
	OLED_DMA_Init();

    // OLED_CS_Clr();  //打开片选使能
    // OLED_RST_Clr();
    // Delay_ms(20);
    // OLED_RST_Set();
    // Delay_ms(20);
    // OLED_BLK_Set();

    //Start Initial Sequence //
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0x35);

    LCD_WR_REG(0xBB);
    LCD_WR_DATA8(0x19);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x12);

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC6);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0xD0);
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0x21);

    LCD_WR_REG(0x11);
    // Delay (120);

    LCD_WR_REG(0x29);
}

/*
void LCD_Writ_Bus(char dat)                                // 串行数据写入
{
    u8 i;

    for (i = 0; i < 8; i++) {
        OLED_SCLK_Clr();
        if (dat & 0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat <<= 1;
    }
}

void LCD_WR_DATA8(char da) // 发送数据-8位参数
{                          // OLED_CS_Clr();
    OLED_DC_Set();
    LCD_Writ_Bus(da);
    // OLED_CS_Set();
}
void LCD_WR_DATA(int da)
{ //	OLED_CS_Clr();
    OLED_DC_Set();
    LCD_Writ_Bus(da >> 8);
    LCD_Writ_Bus(da);
    // OLED_CS_Set();
}
void LCD_WR_REG(char da)
{ //	OLED_CS_Clr();
    OLED_DC_Clr();
    LCD_Writ_Bus(da);
    // OLED_CS_Set();
}
void LCD_WR_REG_DATA(int reg, int da)
{ // OLED_CS_Clr();
    LCD_WR_REG(reg);
    LCD_WR_DATA(da);
    // OLED_CS_Set();
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_WR_REG(0x2a);
    LCD_WR_DATA8(x1 >> 8);
    LCD_WR_DATA8(x1);
    LCD_WR_DATA8(x2 >> 8);
    LCD_WR_DATA8(x2);

    LCD_WR_REG(0x2b);
    LCD_WR_DATA8(y1 >> 8);
    LCD_WR_DATA8(y1);
    LCD_WR_DATA8(y2 >> 8);
    LCD_WR_DATA8(y2);

    LCD_WR_REG(0x2C);
}

void Lcd_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能A端口时钟
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);            // 初始化GPIOD3,6
    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_4);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能A端口时钟
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOD3,6
    GPIO_SetBits(GPIOB, GPIO_Pin_0);

    // OLED_CS_Clr();  //打开片选使能
    OLED_RST_Clr();
    Delay_ms(20);
    OLED_RST_Set();
    Delay_ms(20);
    OLED_BLK_Set();

    //Start Initial Sequence //
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0x35);

    LCD_WR_REG(0xBB);
    LCD_WR_DATA8(0x19);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x12);

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC6);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0xD0);
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0x21);

    LCD_WR_REG(0x11);
    // Delay (120);

    LCD_WR_REG(0x29);
}
*/

// 清屏函数
// Color:要清屏的填充色
void LCD_Clear(u16 Color)
{
    u16 i, j;
    Address_set(0, 0, LCD_W - 1, LCD_H - 1);
    for (i = 0; i < LCD_W; i++) {
        for (j = 0; j < LCD_H; j++) {
            LCD_WR_DATA(Color);
        }
    }
}

// 在指定位置显示一个汉字(32*33大小)
// dcolor为内容颜色，gbcolor为背静颜色
void showhanzi(unsigned int x, unsigned int y, unsigned char index)
{
    unsigned char i, j;
    unsigned char *temp = hanzi;
    Address_set(x, y, x + 31, y + 31); // 设置区域
    temp += index * 128;
    for (j = 0; j < 128; j++) {
        for (i = 0; i < 8; i++) {
            if ((*temp & (1 << i)) != 0) {
                LCD_WR_DATA(POINT_COLOR);
            } else {
                LCD_WR_DATA(BACK_COLOR);
            }
        }
        temp++;
    }
}
// 画点
// POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x, u16 y)
{
    Address_set(x, y, x, y); // 设置光标位置
    LCD_WR_DATA(POINT_COLOR);
}
// 画一个大点
// POINT_COLOR:此点的颜色
void LCD_DrawPoint_big(u16 x, u16 y)
{
    LCD_Fill(x - 1, y - 1, x + 1, y + 1, POINT_COLOR);
}
// 在指定区域内填充指定颜色
// 区域大小:
//   (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
    u16 i, j;
    Address_set(xsta, ysta, xend, yend); // 设置光标位置
    for (i = ysta; i <= yend; i++) {
        for (j = xsta; j <= xend; j++) LCD_WR_DATA(color); // 设置光标位置
    }
}
// 画线
// x1,y1:起点坐标
// x2,y2:终点坐标
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
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}
// 在指定位置画一个指定大小的圆
//(x,y):中心点
// r    :半径
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
    num = num - ' ';                          // 得到偏移后的值
    Address_set(x, y, x + 8 - 1, y + 16 - 1); // 设置光标位置
    if (!mode)                                // 非叠加方式
    {
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[(u16)num * 16 + pos]; // 调用1608字体
            for (t = 0; t < 8; t++) {
                if (temp & 0x01)
                    POINT_COLOR = colortemp;
                else
                    POINT_COLOR = BACK_COLOR;
                LCD_WR_DATA(POINT_COLOR);
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
// 显示2个数字
// x,y :起点坐标
// len :数字的位数
// color:颜色
// num:数值(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len)
{
    u8 t, temp;
    u8 enshow = 0;
    num       = (u16)num;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + 8 * t, y, ' ', 0);
                continue;
            } else
                enshow = 1;
        }
        LCD_ShowChar(x + 8 * t, y, temp + 48, 0);
    }
}
// 显示2个数字
// x,y:起点坐标
// num:数值(0~99);
void LCD_Show2Num(u16 x, u16 y, u16 num, u8 len)
{
    u8 t, temp;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        LCD_ShowChar(x + 8 * t, y, temp + '0', 0);
    }
}
// 显示字符串
// x,y:起点坐标
//*p:字符串起始地址
// 用16字体
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
        LCD_ShowChar(x, y, *p, 0);
        x += 8;
        p++;
    }
}

void GrayToPseColor(uint8_t grayValue, uint16_t *Color)
{
    uint8_t colorR, colorG, colorB;
    *Color = 0;
    colorR = abs(0 - grayValue);
    colorG = abs(127 - grayValue);
    colorB = abs(255 - grayValue);
    *Color = (colorR << 15) | (colorG << 10) | colorB;
}


void LCD_WR_RGB(uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t Color;
    Color = ((colorR & 0xF8) << 8) | ((colorG & 0xFE) << 3) | ((colorB & 0xF8) >> 3);
    LCD_WR_DATA(Color);
}
