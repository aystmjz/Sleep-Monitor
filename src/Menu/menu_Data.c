#include "Menu.h"

int8_t Menu_Start(int8_t Shift)
{
    if (Shift == 0) {
        if (Menu_Global.RunningMenu->isInit) {
            return 0;
        } else {
            Menu_RunMainMenu();
            return 1;
        }
    } else if (Shift == 1) {
        Menu_RunMainMenu();
    } else // if(Shift == -1)
    {
        Menu_Global.RunningMenu->isInit = 1;
        return 0;
    }
    return 0;
}

/*创建选项列表*/
struct Option_Class Menu_StartOptionList[] = {
    {"<<<"},
    {"示波器", Menu_RunToolsMenu},
    {"Games", Menu_RunGamesMenu},
    {"Setting", Menu_RunSettingMenu}, // 设置
    {"Info", Menu_Information},       // 信息
    {".."}};

/*创建开始菜单对象*/
struct Menu_Class Menu_StartMenu = {.isInit = 1};

void Menu_RunMainMenu(void)
{
    if (Menu_StartMenu.isInit) // 由于开始菜单没有上级, 需手动配置部分参数;
    {
        Menu_MenuClassInit(&Menu_StartMenu, Menu_StartOptionList);
        /*手动配置区*******/
        Menu_StartMenu.Target_Window_X = 0;
        Menu_StartMenu.Target_Window_Y = 0;
        Menu_StartMenu.Window_X        = 0;
        Menu_StartMenu.Window_Y        = 64;
        Menu_Global.Cursor_Actual_X    = 0;
        Menu_Global.Cursor_Actual_Y    = 64;

        /*******手动配置区*/
    }

    Menu_RunWindow(&Menu_StartMenu);
}

void MLX90640_Start(void)
{
    Menu_TurnOffMenu();
	LCD_Clear(WHITE);
	GlobalState = 1;
}

void Menu_RunToolsMenu(void)
{
    /*创建选项列表*/
    static struct Option_Class Menu_ToolsOptionList[] = {
        {"<<<"},
        {"定时器", Menu_RunGamesMenu}, // 6-1 定时器定时中断
        {"输入捕获"},                  // 6-6 输入捕获模式测频率
        {"PWM输出", Tools_PWM_Output}, // 6-3 PWM驱动LED呼吸灯
        {"PWM输入"},                   // 6-7 PWMI模式测频率占空比
        {"编码器"},                    // 6-8 编码器接口测速
        {"串口"},                      // 9-3 串口收发HEX数据包
        {"示波器", MLX90640_Start},    // 示波器
        {"ADC"},                       // 8-2 DMA+AD多通道
        {".."}};

    /*创建菜单对象*/
    static struct Menu_Class Menu_ToolsMenu = {.isInit = 1}; // 赋值初始化

    if (Menu_ToolsMenu.isInit) {
        Menu_MenuClassInit(&Menu_ToolsMenu, Menu_ToolsOptionList);
        /*手动配置区*******/

        /*******手动配置区*/
    }

    Menu_RunWindow(&Menu_ToolsMenu);
}

void Menu_RunGamesMenu(void)
{
    static struct Option_Class Menu_GamesOptionList[] = {
        {"<<<"},
        {"Snake", Game_Snake_Init},      // 贪吃蛇
        {"Snake II", Menu_RunToolsMenu}, // 贪吃蛇2
        //	{"Snake III", Game_Snake_Init},	//贪吃蛇3
        //	{"Snake IV", Game_Snake_Init},	//贪吃蛇4
        //	{"Snake V", Game_Snake_Init},	//贪吃蛇5
        //	{"Snake VI", Game_Snake_Init},	//贪吃蛇6
        //	{"Snake VII", Game_Snake_Init},	//贪吃蛇7
        //	{"Snake VIII", Game_Snake_Init},//贪吃蛇8
        //	{"Snake IX", Game_Snake_Init},	//贪吃蛇9
        //{"Snake X", Game_Snake_Init},		 // 贪吃蛇10
        {"康威生命游戏", Game_Of_Life_Play}, // 康威生命游戏,元胞自动机
        {".."}};
    static struct Menu_Class Menu_GamesMenu = {multi, 1};

    if (Menu_GamesMenu.isInit) {
        Menu_MenuClassInit(&Menu_GamesMenu, Menu_GamesOptionList);
        /*手动配置区*******/

        /*******手动配置区*/
    }
    Menu_RunWindow(&Menu_GamesMenu);
}

void Menu_Information(void)
{
    float Angle = 3640;
    while (1) {
        OLED_Clear();

        // OLED_ShowImage(88, 8, 32, 32, goutou);
        OLED_Rotation_Block(88 + 16, 8 + 16, 16, Angle);

        Angle += Menu_RollEvent() * 8;

        if (Angle > 1) {
            Angle -= Angle / 32 + 1;
        } else if (Angle < -1) {
            Angle -= Angle / 32 - 1;
        } else {
            Angle = 0;
        }
        // Angle += 2;
        // Angle %= 360;

        OLED_ShowString(2, 0, "Menu Window\nv1.2", OLED_8X16);
        OLED_ShowString(2, 32, "By:Adam", OLED_8X16);
        OLED_ShowString(2, 48, "UP:加油哦大灰狼", OLED_8X16);

        OLED_Update();
        if (Menu_EnterEvent()) {
            return;
        }
        if (Menu_BackEvent()) {
            return;
        }
    }
}

/**********************************************************/
void text()
{
    Menu_Run_Option_List(Menu_StartOptionList);
}