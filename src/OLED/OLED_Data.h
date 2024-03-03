#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			3		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct 
{
	uint8_t Data[32];						//字模数据
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
} ChineseCell_t;

typedef struct 
{
	uint8_t Data[24];						//字模数据
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
} ChineseCell_t12;

typedef struct 
{
	uint8_t Data[20];						//字模数据
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
} ChineseCell_t10;

typedef struct 
{
	uint8_t Data[8];						//字模数据
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
} ChineseCell_t8;


/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];
extern const ChineseCell_t12 OLED_CF12x12[];
extern const ChineseCell_t10 OLED_CF10x10[];
extern const ChineseCell_t8 OLED_CF8x8[];

/*图像数据声明*/
extern const uint8_t Diode[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
extern const uint8_t Cursor[];
extern const uint8_t goutou[];
extern const uint8_t Wallpaper[];
extern const uint8_t XpWallpaper[];
extern const uint8_t Win11Wallpaper[];

//...
	
#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
