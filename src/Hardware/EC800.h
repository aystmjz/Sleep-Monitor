#ifndef __EC800_H
#define __EC800_H
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "usart.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "Delay.h"
#include "cJSON.h"

typedef struct
{
    double Temp;
    double Hum;
    uint32_t Light;
    uint8_t Led;
    uint8_t Beep;

} Environment_DataTypeDef;

#define ToByte1(x) ((x) & 0xff)
#define ToByte2(x) ((x >> 8) & 0xff)
#define ToByte3(x) ((x >> 16) & 0xff)
#define ToByte4(x) ((x >> 24) & 0xff)

#define DATA1      (Data.Led)
#define DATA2      (ToByte1(Data.Light))
#define DATA3      (ToByte2(Data.Light))
#define DATA_NULL  0XFF
#define DATA_LEN   3

extern char Debug_str[100];
extern Environment_DataTypeDef Data;

#define ATcmd_Set(message) sprintf(AT_cmd, (message))
#define AT_cmd_Buff        128

void Debug_printf(char *SendBuf);
void Main_printf(char *SendBuf);
void ATcmd_Send(char *ATcmd);
uint8_t ATcmd_Scan(char *Ack);
uint8_t Data_Scan(char *Data, char *Flag, uint8_t Len);
uint16_t Data_ScanInt(char *Flag, uint8_t Len);
uint8_t ATcmd_Wait(char *Ack, char *Error, uint16_t Wait_Time, uint8_t Try_Time);
void ATcmd_Make(int num, const char *cmd, ...);
void ATcmd_MakeSend(int num, const char *cmd, ...);
void Clear_Buffer(void);
void EC800_Init(void);
void MQTT_Init(void);
uint16_t EC800_GetADC();
void Pub_String(const char *topic, char *str);
void Pub_Json(const char *topic, cJSON *json);
void Pub_Data(const char *topic);

#endif
