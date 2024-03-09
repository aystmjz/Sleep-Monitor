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
    {"工具", Menu_RunToolsMenu},
    {"数据", Menu_RunDataMenu},
    {"游戏", Menu_RunGamesMenu},
    {"设置", Menu_RunSettingMenu},
    {"信息", Menu_Information},
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

void Menu_StartThermalCamera(void)
{
    Menu_TurnOffMenu();
    GlobalState = InitThermalCamera;
}

void Menu_StartClock(void)
{
    Menu_TurnOffMenu();
    GlobalState = InitClock;
}

void Menu_RunToolsMenu(void)
{
    /*创建选项列表*/
    static struct Option_Class Menu_ToolsOptionList[] = {
        {"<<<"},
        {"热成像仪", Menu_StartThermalCamera},
        {"红外遥控", Tools_Remote},
        {"时钟", Menu_StartClock},
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

void Menu_StartNowData(void)
{
    Menu_TurnOffMenu();
    GlobalState = InitNowData;
}

void Menu_RunDataMenu(void)
{
    /*创建选项列表*/
    static struct Option_Class Menu_DataOptionList[] = {
        {"<<<"},
        {"实时环境数据", Menu_StartNowData},
        {"历史环境数据", Tools_Reader},
        {".."}};

    /*创建菜单对象*/
    static struct Menu_Class Menu_DataMenu = {.isInit = 1}; // 赋值初始化

    if (Menu_DataMenu.isInit) {
        Menu_MenuClassInit(&Menu_DataMenu, Menu_DataOptionList);
        /*手动配置区*******/

        /*******手动配置区*/
    }

    Menu_RunWindow(&Menu_DataMenu);
}

void Menu_RunGamesMenu(void)
{
    static struct Option_Class Menu_GamesOptionList[] = {
        {"<<<"},
        {"贪吃蛇", Game_Snake_Init},         // 贪吃蛇
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
