#include <string.h>
#include <math.h>
#include "OLED.h"

/********OLED补丁*********/
/**
 * 函    数：OLED显示图像(**Adam修改, Y轴可以是负数**)
 * 参    数：X 指定图像左上角的横坐标，范围：0~127
 * 参    数：Y 指定图像左上角的纵坐标，范围：int_8
 * 参    数：Width 指定图像的宽度，范围：0~128
 * 参    数：Height 指定图像的高度，范围：0~64
 * 参    数：Image 指定要显示的图像
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_ShowImage(uint8_t X, uint8_t Y0, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	int8_t Y = (int8_t)Y0; // XY值为负数时,只要图片还有部分在屏幕内显示,就还是有意义的;

	uint8_t i, j;
	/*参数检查，保证指定图像不会超出屏幕范围*/
	if (X > 127)
	{
		return;
	} // 如果X大于127则打印无意义,故无需打印

	uint8_t k = 0, l = 0, temp_Height; //(创建Y为负数相关变量)
	if (Y < 0)						   // 如果Y小于0, 则需打印图片位于屏幕内的部分;
	{
		temp_Height = Height; // 记录图片高度
		Height += Y;		  // 图片的高 减去Y轴绝对值
		k = (-Y - 1) / 8 + 1; // 图片的Byte行的下标偏移量;
		l = (-Y - 1) % 8 + 1; // Byte位移基	//(8 + Y % 8)%8;//		OLED_ShowNum(0, 56, l, 3, 6);
		Y = 0;				  // 图片仍然从屏幕Y0开始打印
		if ((int8_t)Height < 1)
		{
			return;
		} // 需打印的图片高小于1时,说明不需要打印;
	}
	else if (Y > 63)
	{
		return;
	} // 如果Y大于63则打印无意义,故无需打印

	/*将图像所在区域清空*/
	OLED_ClearArea(X, Y, Width, Height);

	/*遍历指定图像涉及的相关页*/
	/*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
	uint8_t Height_ceil = (Height - 1) / 8 + 1;

	for (j = 0; j < Height_ceil; j++) // 以OLED_DisplayBuf行下标遍历,(图像涉及的相关页)
	{
		/*遍历指定图像涉及的相关列*/
		for (i = 0; i < Width; i++)
		{
			/*超出边界，则跳过显示*/
			if (X + i > 127)
			{
				break;
			}
			if (Y / 8 + j > 7)
			{
				return;
			}

			if (k) // 以一行OLED_DisplayBuf行下标为基础, 取两行图像Byte, 根据位移基放置;
			{
				/*显示上一Byte图像在当前页的内容*/
				OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[(j + k - 1) * Width + i] >> ((l));

				/*超出边界，则跳过显示*/
				/*使用continue的目的是，当前Byte超出图片时，上一Byte的后续内容还需要继续显示*/
				if ((j + k) * 8 >= temp_Height)
				{
					continue;
				} // 如果当前行Byte已经不属于图像,则不放置;

				/*显示当前Byte图像在当前页的内容*/
				OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[(j + k) * Width + i] << (8 - (l));
			}

			else // 以一行图像Byte为基础, 放于两行OLED_DisplayBuf行之间;
			{
				/*显示图像在当前页的内容*/
				OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[(j)*Width + i] << ((Y) % 8);

				/*超出边界，则跳过显示*/
				/*使用continue的目的是，下一页超出边界时，上一页的后续内容还需要继续显示*/
				if (Y / 8 + j + 1 > 7)
				{
					continue;
				}

				/*显示图像在下一页的内容*/
				OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[(j)*Width + i] >> (8 - (Y) % 8);
			}
		}
	}
}

/**
 * 函    数：OLED显示汉字单字
 * 参    数：X 指定字符串左上角的横坐标，范围：0~127
 * 参    数：Y 指定字符串左上角的纵坐标，范围：0~63
 * 参    数：Hanzi 指定要显示的字符，范围：字库字符
 * 参    数：FontSize 指定字体大小
 *           范围：OLED_8X16		宽8像素，高16像素
 *                 OLED_6X8		宽6像素，高8像素
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_MyShowChinese(int8_t X, int8_t Y, char *Hanzi, uint8_t FontSize) // 汉字单字打印;
{
	uint8_t pIndex;
	if(FontSize == OLED_8X16)
	{
		for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex++)
		{
			/*找到匹配的汉字*/
			if (strcmp(OLED_CF16x16[pIndex].Index, Hanzi) == 0)
			{
				break; // 跳出循环，此时pIndex的值为指定汉字的索引
			}
		}
		/*将汉字字模库OLED_CF16x16的指定数据以16*16的图像格式显示*/
		OLED_ShowImage(X, Y, 16, 16, OLED_CF16x16[pIndex].Data);
	}
	else if(FontSize == OLED_6X8)
	{
		for (pIndex = 0; strcmp(OLED_CF12x12[pIndex].Index, "") != 0; pIndex++)
		{
			/*找到匹配的汉字*/
			if (strcmp(OLED_CF12x12[pIndex].Index, Hanzi) == 0)
			{
				break; // 跳出循环，此时pIndex的值为指定汉字的索引
			}
		}
		/*将汉字字模库OLED_CF12x12的指定数据以12*12的图像格式显示*/
		OLED_ShowImage(X, Y-2, 12, 12, OLED_CF12x12[pIndex].Data);
	}
	else
	{
		for (pIndex = 0; strcmp(OLED_CF10x10[pIndex].Index, "") != 0; pIndex++)
		{
			/*找到匹配的汉字*/
			if (strcmp(OLED_CF10x10[pIndex].Index, Hanzi) == 0)
			{
				break; // 跳出循环，此时pIndex的值为指定汉字的索引
			}
		}
		/*将汉字字模库OLED_CF16x16的指定数据以16*16的图像格式显示*/
		OLED_ShowImage(X+1, Y-1, 10, 10, OLED_CF10x10[pIndex].Data);
	}
}

/**
 * 函    数：OLED显示字符串
 * 参    数：X 指定字符串左上角的横坐标，范围：0~127
 * 参    数：Y 指定字符串左上角的纵坐标，范围：0~63
 * 参    数：String 指定要显示的字符串，范围：字库字符组成的字符串
 * 参    数：FontSize 指定字体大小
 *           范围：OLED_8X16		宽8像素，高16像素
 *                 OLED_6X8		宽6像素，高8像素
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize) // 中英文打印;
{
	uint8_t i = 0, len = 0;
	while (String[i] != '\0') // 遍历字符串的每个字符
	{
		if (String[i] == '\n')
		{
			Y += (FontSize == 8) ? 16 : 8;
			len = 0;
			i++;
		} // 兼容换行符
		if (X + (len + 1) * FontSize > 128)
		{
			Y += (FontSize == 8) ? 16 : 8;
			len = 0;
		} // 超出屏幕自动换行
		if ((int8_t)Y > 64)
		{
			return;
		} //

		if (String[i] > '~') // 如果不属于英文字符
		{
			char SingleChinese[4] = {0};
			SingleChinese[0] = String[i];
			i++;
			SingleChinese[1] = String[i];
			i++;
			SingleChinese[2] = String[i];

			OLED_MyShowChinese(X + len * FontSize, Y, SingleChinese, FontSize);
		
			i++;
			len += 2;
		}
		else /*调用OLED_ShowChar函数，依次显示每个字符*/
		{
			OLED_ShowChar(X + len * FontSize, Y, String[i], FontSize);
			i++;
			len++;
		}
	}
}

/**
 * 函    数：OLED区域显示字符串
 * 参    数：X 指定字符串左上角的横坐标，范围：0~127
 * 参    数：Y 指定字符串左上角的纵坐标，范围：0~63
 * 参    数：Width 指定区域宽度，范围：0~127
 * 参    数：Height 指定区域高度，范围：0~63
 * 参    数：String 指定要显示的字符串，范围：字库字符组成的字符串
 * 参    数：FontSize 指定字体大小
 *           范围：OLED_8X16		宽8像素，高16像素
 *                 OLED_6X8		宽6像素，高8像素
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
uint8_t OLED_ShowStringArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, char *String, uint8_t FontSize) // 区域打印打印;
{
	uint8_t i = 0, len = 0;
	while (String[i] != '\0') // 遍历字符串的每个字符
	{
		if (String[i] == '\n') // 兼容换行符
		{
			Y += (FontSize == 8) ? 16 : 8;
			Height -= (FontSize == 8) ? 16 : 8;
			if (Height < ((FontSize == 8) ? 16 : 8))
			{
				break;
			}

			len = 0;
			i++;
		}

		if ((len + 1) * FontSize > Width) // 超出屏幕自动换行
		{
			Y += (FontSize == 8) ? 16 : 8;
			Height -= (FontSize == 8) ? 16 : 8;
			if (Height < ((FontSize == 8) ? 16 : 8))
			{
				break;
			}

			len = 0;
		}

		if (String[i] > '~') // 如果不属于英文字符
		{
			char SingleChinese[4] = {0};
			SingleChinese[0] = String[i];
			i++;
			SingleChinese[1] = String[i];
			i++;
			SingleChinese[2] = String[i];

			OLED_MyShowChinese(X + len * FontSize, Y, SingleChinese, FontSize);
			
			i++;
			len += 2;
		}
		else /*调用OLED_ShowChar函数，依次显示每个字符*/
		{
			OLED_ShowChar(X + len * FontSize, Y, String[i], FontSize);
			i++;
			len++;
		}
	}
	return len;
}

/**
 * 函    数：旋转点
 * 参    数：CX 指定旋转原点的横坐标，范围：0~127
 * 参    数：CY 指定旋转原点的纵坐标，范围：0~63
 * 参    数：PX 指定旋转点的横坐标，范围：0~127
 * 参    数：PY 指定旋转点的纵坐标，范围：0~63
 * 参    数：Angle 指定旋转角度，范围：-360~360
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_Rotation_C_P(int8_t CX, int8_t CY, float *PX, float *PY, int16_t Angle) // 旋转点
{
	float Theta = (3.14 / 180) * Angle;
	float Xd = *PX - CX;
	float Yd = *PY - CY;

	*PX = (Xd)*cos(Theta) - (Yd)*sin(Theta) + CX; // + 0.5;
	*PY = (Xd)*sin(Theta) + (Yd)*cos(Theta) + CY; // + 0.5;
}
/**
 * 函    数：将OLED显存数组部分旋转
 * 参    数：X 指定旋转原点的横坐标，范围：0~127
 * 参    数：Y 指定旋转原点的纵坐标，范围：0~63
 * 参    数：Width 指定旋转区域半径，范围：0~63
 * 参    数：Angle 指定旋转角度，范围：-360~360
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_Rotation_Block(int8_t X, int8_t Y, int8_t Width, int16_t Angle) // 旋转区块
{
	uint8_t OLED_DpB1[8][128];
	memcpy(OLED_DpB1, OLED_DisplayBuf, 1024);

	// OLED_Clear();
	OLED_ClearArea(X - Width, Y - Width, Width * 2, Width * 2);
	float Theta = (3.14 / 180) * Angle;

	uint8_t x, y;
	for (y = Y - Width; y < Y + Width; y++) // 遍历指定页
	{
		for (x = X - Width; x < X + Width; x++) // 遍历指定列
		{
			//			if(x > 127) break;
			//			if(y > 63) break;
			//			if(x < 0) break;
			//			if(y < 0) break;
			x %= 128;
			y %= 64;

			if (OLED_DpB1[y / 8][x] & 0x01 << (y % 8)) // 效果同if(OLED_GetPoint(x, y))
			{
				OLED_DrawPoint(
					(float)(x - X) * cos(Theta) - (float)(y - Y) * sin(Theta) + X, //+ 0.5,
					(float)(x - X) * sin(Theta) + (float)(y - Y) * cos(Theta) + Y  // + 0.5
				);
			}
		}
	}
}

void ellipse_algorithm(int8_t x0, int8_t y0, int8_t a, int8_t b)
{
	float t = 0.01;
	float d = 6.28 / ((a + b) * 2);

	int8_t x = x0;
	int8_t y = y0;

	int8_t xtemp = x;
	int8_t ytemp = y;

	x = (a * cos(t));
	y = (b * sin(t));

	while (t <= 6.28)
	{

		xtemp = x;
		ytemp = y;

		x = (a * cos(t));
		y = (b * sin(t));
		OLED_DrawLine(x0 + xtemp, y0 + ytemp, x0 + x, y0 + y);
		t += d;
		// OLED_Update();
	}
}
