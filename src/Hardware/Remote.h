#ifndef __RED_H
#define __RED_H
#include "stm32f10x.h" // Device header

#define REST                  0 // 空闲状态
#define BEGIN                 1 // 起始状态
#define RUN                   2 // 解码状态

#define HIGH_TIME             2250
#define LOW_TIME              1120
#define START_TIME            13500
#define REPEAT_TIME           11250
#define ERROR_TIME            500

#define REMOTE_ADDRESS_LEN    16
#define REMOTE_COMMAND_LEN    16

#define REMOTE_ID_VERIFY      1
#define REMOTE_ADDRESS_VERIFY 0
#define REMOTE_COMMAND_VERIFY 0
#define REMOTE_ID             2 // 红外遥控识别码(ID)

extern uint8_t Remote_RepeatCounter; // 按键按下的次数

void Remote_Init(void);
uint16_t Remote_GetAddress(void);
uint16_t Remote_GetCommand(void);
#endif