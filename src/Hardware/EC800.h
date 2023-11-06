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

extern char Debug_str[100];

typedef struct MessageTypeDef
{
    double Temp;
	double Hum;
	uint32_t Light;
	uint8_t Led;
	uint8_t Beep;

} MessageTypeDef;

#define ATcmd_Set(message) sprintf(AT_cmd,(message))
#define AT_cmd_Buff 128

void Debug_printf(char*SendBuf);
void Main_printf(char*SendBuf);
void ATcmd_Send(char* ATcmd);
uint8_t ATcmd_Scan(char* Ack);
uint8_t Data_Scan(char* Data,char* Flag,uint8_t Len);
uint16_t Data_ScanInt(char* Flag,uint8_t Len);
uint8_t ATcmd_Wait(char* Ack,char* Error,uint16_t Wait_Time,uint8_t Try_Time);
void ATcmd_Make(int num,const char* cmd, ...);
void ATcmd_MakeSend(int num,const char* cmd, ...);
void Clear_Buffer(void);
void EC800_Init(void);
void MQTT_Init(void);
uint16_t Get_ADC();
void Pub_String(char* topic,char* str);
void Pub_Json(char* topic,cJSON* json);

#endif

