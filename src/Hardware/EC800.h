#ifndef __EC800_H
#define __EC800_H
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "wdg.h"
#include "Delay.h"
#include "cJSON.h"

typedef struct
{
    float Temp;
    float Hum;
    float Light;
    uint16_t eco2;
    uint16_t tvoc;
    uint8_t Led;
    uint8_t Beep;

} Environment_DataTypeDef;

#define ToByte1(x) ((x) & 0xff)
#define ToByte2(x) ((x >> 8) & 0xff)
#define ToByte3(x) ((x >> 16) & 0xff)
#define ToByte4(x) ((x >> 24) & 0xff)

#define DATA1      ((uint16_t)Data.Temp)
#define DATA2      ((uint16_t)(Data.Temp * 100) % 100)
#define DATA3      ((uint16_t)Data.Hum)
#define DATA4      ((uint16_t)(Data.Hum * 100) % 100)
#define DATA5      ((uint16_t)Data.Light)
#define DATA6      ((uint16_t)(Data.Light * 100) % 100)
#define DATA7      (ToByte2((uint16_t)Data.eco2))
#define DATA8      (ToByte1((uint16_t)Data.eco2))
#define DATA9      (ToByte2((uint16_t)Data.tvoc))
#define DATA10     (ToByte1((uint16_t)Data.tvoc))
#define DATA_NULL  0XFF
#define DATA_LEN   10+1

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
