#ifndef __MENU_H
#define __MENU_H
#include "stm32f10x.h" // Device header
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "OLED.h"
#include "Encoder.h"
#include "Key.h"

#include "Tools.h"

#include "Game_Snake.h"
#include "game_of_life.h"

enum CursorStyle
{
	reverse,
	mouse,
	frame,
};

enum MenuStyle
{
	multi,
	single,
};

//struct Object
//{
//	int8_t X;  // 位置X
//	int8_t Y;  // 位置Y
//	uint8_t W; // 宽度
//	uint8_t H; // 高度
//};

struct MenuProperty
{
	uint8_t isRun;//菜单是否在运行
	float Cursor_Actual_X;		// 当前光标位置X
	float Cursor_Actual_Y;		// 当前光标位置Y
	float Cursor_Actual_W;		// 当前光标尺寸宽
	float Cursor_Actual_H;		// 当前光标尺寸高
	enum CursorStyle Cur_Style; // 光标风格;
	float Cursor_ActSpeed;		// 光标动画速度系数;
	float Slide_ActSpeed;		// 滚动动画速度系数;
	int8_t Font_Width;			// 字体宽度
	int8_t Font_Height;			// 字体宽度
	int8_t Line_Height;			// 行高
	int8_t Layout_Margin;		// 页边距

	int8_t Window_X;  // 窗口位置X
	int8_t Window_Y;  // 窗口位置Y
	uint8_t Window_W; // 窗口宽度
	uint8_t Window_H; // 窗口高度

	struct Menu_Class *RunningMenu; // 运行中的菜单
};


/**********************************************************/
struct Option_Class
{
	char *String;		// 选项名字
	void (*func)(void); // 函数指针
	float *Variable;	// 数值

	// int8_t Coord_X; // 选项坐标X
	// int8_t Coord_Y; // 选项坐标Y
	// uint8_t Width;	// 选项宽度
	// uint8_t Height; // 选项高度

	uint8_t StrLen;	 // 由于中文占三个字节,用strlen计算名字宽度不再准确,故需额外储存名字宽度
};

struct Menu_Class
{
	enum MenuStyle Menu_Style;//菜单样式

	int8_t isInit;	  // 是否初始化菜单状态
	int8_t isShow;	  // 是否显示菜单
	int8_t isReadKey; // 是否读取按键
	int8_t isCaller;  // 是否调用下级程序

	int8_t Cat_i;				// 选中下标
	int8_t Cur_i;				// 光标下标
	int8_t Cur_i_Ceiling;		// 光标位置限制,自动计算;上限位
	enum CursorStyle Cur_Style; // 光标风格

	int8_t Show_i;			// 显示起始下标
	int8_t Show_i_previous; // 显示起始下标的前一个状态(用于比较)
	int8_t Show_offset;		// 显示Y轴的偏移相关

	uint8_t Option_MaxNum;	  // 选项列表长度
	uint8_t Option_MaxStrLen; // 选项列表长度
	struct Option_Class *Option_List;

	int8_t Font_Width;	  // 字宽
	int8_t Font_Height;	  // 字高
	int8_t Line_Height;	  // 行高
	int8_t Layout_Margin; // 页边距 布局

	int8_t Window_X;	  // 窗口坐标X
	int8_t Window_Y;	  // 窗口坐标Y
	uint8_t Window_W;	  // 窗口宽度
	uint8_t Window_H;	  // 窗口高度
	int8_t Window_Border; // 窗口边框
	int8_t Window_BotCol; // 窗口底色bottom colour

	int8_t Target_Window_X;	 // 窗口坐标X
	int8_t Target_Window_Y;	 // 窗口坐标Y
	uint8_t Target_Window_W; // 窗口宽度
	uint8_t Target_Window_H; // 窗口高度
};

extern struct MenuProperty Menu_Global;//全局属性
extern struct Menu_Class Menu_StartMenu;//开始菜单对象

extern int8_t GlobalState;

/**********************************************************/
int8_t Menu_Start(int8_t Shift);
void Menu_SelectOption(void);
void Menu_TurnOffMenu(void);
/**********************************************************/
int8_t Menu_RunWindow(struct Menu_Class *MU); // 运行窗口
void Menu_Run_Option_List(struct Option_Class *Option_List);//运行选项列表
/**********************************************************/
uint8_t Menu_GetOptionStrLen(char *String);//获取选项字符串长度
uint8_t Menu_PrintfOptionStr(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t FontSize, char *format, ...);//范围格式化字符串打印
/**********************************************************/
void Menu_ShowCursor(float Target_Cur_X, float Target_Cur_Y, float Target_Cur_W, float Target_Cur_H, enum CursorStyle CurStyle, float Cur_Act_Speed);//显示光标
float Menu_CurveMigration(float Actual_Value, float Target_Value, float Act_Speed);//平滑偏移数值
/**********************************************************/
void Menu_MenuClassInit(struct Menu_Class *MU, struct Option_Class *OL);//菜单对象初始化
void Menu_ShowWallpaper(const uint8_t *Wallpaper);//显示壁纸
/**********************************************************/
void Menu_RunMainMenu(void);
void Menu_RunToolsMenu(void);
void Menu_RunGamesMenu(void);
void Menu_RunSettingMenu(void);
void Menu_Information(void);
/**********************************************************/
int8_t Menu_RollEvent(void);
int8_t Menu_EnterEvent(void);
int8_t Menu_BackEvent(void);
/**********************************************************/

#endif
