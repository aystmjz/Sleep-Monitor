#include "Menu.h"

struct Option_Class Menu_SettingOptionList[];

void Set_CursorStyle(void)
{
	static int8_t State = 0;

	if (State == 0)
	{
		Menu_Global.Cur_Style = mouse;
		Menu_SettingOptionList[1].String = "光标风格[鼠标]";
		State++;
	}
	else if (State == 1)
	{
		Menu_Global.Cur_Style = frame;
		Menu_SettingOptionList[1].String = "光标风格[矩形]";
		State++;
	}
	else // if(Menu_Global.Cur_Style == frame)
	{
		Menu_Global.Cur_Style = reverse;
		Menu_SettingOptionList[1].String = "光标风格[反相]";
		State = 0;
	}
}

void Set_animation_speed(void)
{
	static int8_t State = 0;

	if (State == 0)
	{
		Menu_Global.Cursor_ActSpeed = 0.2;
		Menu_Global.Slide_ActSpeed = 4;
		Menu_SettingOptionList[2].String = "动画速度[快]";
		State++;
	}
	else if (State == 1)
	{
		Menu_Global.Cursor_ActSpeed = 0.99;
		Menu_Global.Slide_ActSpeed = 16;
		Menu_SettingOptionList[2].String = "动画速度[关]";
		State++;
	}
	else if (State == 2)
	{
		Menu_Global.Cursor_ActSpeed = 0.1;
		Menu_Global.Slide_ActSpeed = 1.26;
		Menu_SettingOptionList[2].String = "动画速度[慢]";
		State++;
	}
	else
	{
		Menu_Global.Cursor_ActSpeed = 0.15;
		Menu_Global.Slide_ActSpeed = 2;
		Menu_SettingOptionList[2].String = "动画速度[中]";
		State = 0;
	}
	State %= 4;
}

void Menu_RunSettingMenu(void)
{
	static struct Menu_Class Menu = {.isInit = 1}; // 创建菜单实体
	if (Menu.isInit)
	{
		Menu_MenuClassInit(&Menu, Menu_SettingOptionList);
	}
	Menu_RunWindow(&Menu);
}
struct Option_Class Menu_SettingOptionList[] = {
	{"<<<"},
	{"光标风格[反相]", Set_CursorStyle},
	{"动画速度[中]", Set_animation_speed},
	{".."}};
