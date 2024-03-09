#include "Tools.h"

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