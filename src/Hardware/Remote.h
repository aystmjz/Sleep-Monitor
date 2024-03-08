#ifndef __RED_H
#define __RED_H
#include "Delay.h"
#include "sys.h"

#define REMOTE_ADDRESS_LEN 8
#define REMOTE_COMMAND_LEN 8
#define REMOTE_ID          0x00 // 红外遥控识别码(ID)

/*红外发送*/
typedef enum {
    REMOTE_Common_Verify,
    REMOTE_Repeat_Verify,
    REMOTE_No_Verify,
} RemoteMethod;

#define REMOTE_PIN       PAout(6)

#define START_LOW_TIME   9000
#define START_HIGH_TIME  4500
#define REPEAT_LOW_TIME  9000
#define REPEAT_HIGH_TIME 2500
#define ZERO_LOW_TIME    560
#define ZERO_HIGH_TIME   560
#define ONE_LOW_TIME     560
#define ONE_HIGH_TIME    1690

/*红外接收*/
#define REST                  0 // 空闲状态
#define BEGIN                 1 // 起始状态
#define RUN                   2 // 解码状态

#define ONE_TIME              (ONE_LOW_TIME + ONE_HIGH_TIME)
#define ZERO_TIME             (ZERO_LOW_TIME + ZERO_HIGH_TIME)
#define START_TIME            (START_LOW_TIME + START_HIGH_TIME)
#define REPEAT_TIME           (REPEAT_LOW_TIME + REPEAT_HIGH_TIME)
#define ERROR_TIME            500

#define REMOTE_ID_VERIFY      1
#define REMOTE_ADDRESS_VERIFY 0
#define REMOTE_COMMAND_VERIFY 1

#define CMD_START                 0x40
#define CMD_UP                    0x19
#define CMD_DOWN                  0x18

extern uint8_t Remote_RepeatCounter; // 按键按下的次数

void Remote_Init(RemoteMethod method);
uint16_t Remote_GetAddress(void);
uint16_t Remote_GetCommand(void);
void Remote_Transmit(uint16_t address, uint16_t command);
void Remote_SendRepeat(void);

#endif