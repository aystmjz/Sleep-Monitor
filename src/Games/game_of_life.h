#ifndef __GAME_OF_LIFE_H
#define __GAME_OF_LIFE_H

#include <string.h>
#include <stdlib.h>
#include "OLED.h"
#include "Delay.h"
#include "Menu.h"
#include "Encoder.h"
#include "DS3231.h"


void Game_Of_Life_Play(void);		//开始游戏
void Game_Of_Life_Seed(int16_t seed);		//游戏初始化种子

#endif

