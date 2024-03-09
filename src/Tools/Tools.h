#ifndef __TOOLS_H
#define __TOOLS_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Menu.h"


void Tools_Remote(void);
void Tools_Reader(void);
void Disable_MUReadKey_Frame(void);
void Disable_MUReadKey_Mouse(void);

#endif

