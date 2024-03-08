#include "Tools_Remote.h"

struct Option_Class Remote_Option_List[];
void Disable_MUReadKey(void);

struct Tools_Remote_Class {
    uint8_t isInit;
    uint16_t Address;
    uint16_t Command;
    struct Menu_Class MU;
} Remote = {
    .Address = REMOTE_ID,
    .Command = CMD_START,
};

void Tools_Remote_Transmit(void)
{
    Remote_Transmit(Remote.Address, Remote.Command);
}

void Disable_MUReadKey_Frame(void)
{
    Menu_Global.RunningMenu->Cur_Style = frame;
    Menu_Global.RunningMenu->isReadKey = 0;
}

void Disable_MUReadKey_Mouse(void)
{
    Menu_Global.RunningMenu->Cur_Style = mouse;
    Menu_Global.RunningMenu->isReadKey = 0;
}

struct Option_Class Remote_Option_List[] = {
    {"<<<"},
    {"关闭", Menu_TurnOffMenu},
    {"地址:%d", Disable_MUReadKey_Mouse, &Remote.Address},
    {"命令:%d", Disable_MUReadKey_Mouse, &Remote.Command},
    {"发送命令", Tools_Remote_Transmit},
    {"风扇模式", Disable_MUReadKey_Frame},
    {".."},
};

void Tools_Remote(void)
{
    static float NumPow                      = 1;
    static struct Menu_Class Menu_RemoteMenu = {single, 1};
    uint8_t Key_Temp;
    int8_t Encoder_Temp;

    if (Menu_RemoteMenu.isInit) {
        Menu_MenuClassInit(&Menu_RemoteMenu, Remote_Option_List);
    }

    if (Menu_RemoteMenu.isReadKey == 0) // 如果菜单不读取按键则代表抓住了选项;
    {
        Key_Temp     = Menu_EnterEvent();
        Encoder_Temp = Encoder_Get_Div4();
        switch (Menu_RemoteMenu.Cat_i) // 抓住的选项
        {
            case 2:
                Remote.Address += Encoder_Temp * NumPow; // 编码器偏移量乘以位权
                if (Remote.Address >= 10000) Remote.Address = 0;
                break;
            case 3:
                Remote.Command += Encoder_Temp * NumPow;
                if (Remote.Command >= 10000) Remote.Command = 0;
                break;
            case 5:
                if (Encoder_Temp < 0) {
                    Remote_Transmit(REMOTE_ID, CMD_DOWN);
                }
                if (Encoder_Temp > 0) {
                    Remote_Transmit(REMOTE_ID, CMD_UP);
                }
                if (Key_Temp == 1) {
                    Remote_Transmit(REMOTE_ID, 64);
                }
                break;
            default:
                break;
        }
        if (Key_Temp == 2) // 长按放手
        {
            NumPow                    = 1;
            Menu_RemoteMenu.isReadKey = 1;
            Menu_RemoteMenu.Cur_Style = Menu_Global.Cur_Style;
        }
        if (Key_Temp == 1) // 短按加位权
        {
            NumPow *= 10; // 位权
            if (NumPow > 100) {
                NumPow = 1;
            }
        }
    } else {
        Remote.MU.Cur_Style = Menu_Global.Cur_Style;
    }

    Menu_RunWindow(&Menu_RemoteMenu);
}
