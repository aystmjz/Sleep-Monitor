#include "EC800.h"
#include "EC800_Cacert.h"

extern char  RxBuffer[100],RxCounter;
extern MessageTypeDef Data;

char Debug_str[100];

char AT_cmd[AT_cmd_Buff];


void Debug_printf(char*SendBuf)
{
	Uart3_SendStr(SendBuf);
}

void Main_printf(char*SendBuf)
{
	printf(SendBuf);
}

void ATcmd_Send(char* ATcmd)
{
	Main_printf(ATcmd);
	Delay_ms(100);
}

/// @brief 从接收缓存中扫描指定应答
/// @param Ack 指定应答
/// @return 存在指定应答返回1
uint8_t ATcmd_Scan(char* Ack)
{
	char *strx;
	strx=strstr((const char*)RxBuffer,(const char*)Ack);
	if(strx)return 1;
	else return 0;
}

/// @brief 从接收缓存中提取指定数据
/// @param Data 写入位置
/// @param Flag 数据标志
/// @param Len  数据长度
/// @return 成功提取返回1
uint8_t Data_Scan(char* Data,char* Flag,uint8_t Len)
{
	char* pstr,* pData=Data;
	uint8_t FlagLen=strlen(Flag);
	pstr=strstr((const char*)RxBuffer,(const char*)Flag);
	if(!pstr)return 0;
	pstr+=FlagLen;
	for(int i=0;i<Len;i++)
	{
		*pData=*pstr;
		pstr++;
		pData++;
	}
	*pData='\0';
	return 1;
}

/// @brief 从接收缓存中提取指定数据(整数)
/// @param Flag 数据标志
/// @param Len  数据长度
/// @return 提取的整数
uint16_t Data_ScanInt(char* Flag,uint8_t Len)
{
	char Data_Temp[10];
	if(!Data_Scan(Data_Temp,Flag,Len))return 0;
	return atoi(Data_Temp);
}


/// @brief 发送AT_cmd中指令并等待目标答复
/// @param Ack 目标答复
/// @param Error 错误提示
/// @param Wait_Time 等待答复时间
/// @param Try_Time 重试次数
/// @return 成功：1	失败：0
uint8_t ATcmd_Wait(char* Ack,char* Error,uint16_t Wait_Time,uint8_t Try_Time)
{
	Clear_Buffer();
	if(!Try_Time)Try_Time=0xff;
	for(uint8_t i=1;i<=Try_Time;i++)
	{
		ATcmd_Send(AT_cmd);
		//Debug_printf("Wait\r\n");
		uint16_t timeout = 1;
		while(!ATcmd_Scan(Ack))
		{
   	 		Delay_ms(1);
   			timeout ++;
    		if(timeout > Wait_Time){if(Error!=NULL)Debug_printf(Error);timeout=0;break;}
		}
		if(timeout)return 1;
	}
	return 0;
}

/// @brief 生成AT指令到AT_cmd
/// @param num 指令参数数量
/// @param cmd 指令名称
/// @param
void ATcmd_Make(int num,const char* cmd, ...)
{
	va_list arg_list;
	char cmd_temp[AT_cmd_Buff]={0};
	char *pstr_temp;
	va_start(arg_list, cmd);
	strcat(cmd_temp, "AT+");
	strcat(cmd_temp, cmd);
	if(num)strcat(cmd_temp, "=");
	for(int i=0;i<num;i++)
	{
		pstr_temp=va_arg(arg_list, char*);
		if(*pstr_temp<'0'||*pstr_temp>'9')
		{
			strcat(cmd_temp, "\"");
			strcat(cmd_temp, pstr_temp);
			strcat(cmd_temp, "\"");
		}
		else
		{
			strcat(cmd_temp, pstr_temp);
		}
		if(i<num-1)strcat(cmd_temp, ",");
	}
	strcat(cmd_temp, "\r\n");
	va_end(arg_list);
	sprintf(AT_cmd,cmd_temp);
}
/// @brief 生成AT指令并发送
/// @param num 指令参数数量
/// @param cmd 指令名称
/// @param
void ATcmd_MakeSend(int num,const char* cmd, ...)
{
	va_list arg_list;
	char cmd_temp[AT_cmd_Buff]={0};
	char *pstr_temp;
	va_start(arg_list, cmd);
	strcat(cmd_temp, "AT+");
	strcat(cmd_temp, cmd);
	if(num)strcat(cmd_temp, "=");
	for(int i=0;i<num;i++)
	{
		pstr_temp=va_arg(arg_list, char*);
		if(*pstr_temp<'0'||*pstr_temp>'9')
		{
			strcat(cmd_temp, "\"");
			strcat(cmd_temp, pstr_temp);
			strcat(cmd_temp, "\"");
		}
		else
		{
			strcat(cmd_temp, pstr_temp);
		}
		if(i<num-1)strcat(cmd_temp, ",");
	}
	strcat(cmd_temp, "\r\n");
	va_end(arg_list);
	ATcmd_Send(cmd_temp);
}

void Clear_Buffer(void)//清空缓存
{
		u8 i;
        Debug_printf(RxBuffer);
		for(i=0;i<RxCounter;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		IWDG_Feed();//喂狗
}

void test()
{
	ATcmd_Set("AT+QHTTPPOST=?\r\n");
	ATcmd_Make(5,"QMTPUBEX","0","0","0","0","aystmjz/topic/hxd");
	ATcmd_Send(AT_cmd);
	ATcmd_Wait("ok",NULL,200,1);
	//ATcmd_Make_Send(5,"QMTPUBEX","0","0","0","0","aystmjz/topic/hxd");

}

void EC800_Init(void)
{
	//ATcmd_Set("AT+QHTTPPOST=?\r\n");
	//ATcmd_Wait("OK",NULL,500,2);
	ATcmd_Set("AT\r\n");
	ATcmd_Wait("OK","AT失败\r\n",1000,0);

	ATcmd_Set("ATE0\r\n"); //关闭回显
	ATcmd_Wait("OK","回显关闭失败\r\n",1000,1);

	//ATcmd_Send("AT+CSQ\r\n");//检查CSQ

	//ATcmd_Send("AT+QMTDISC=0\r\n");	//关闭socket连接

	ATcmd_Set("AT+CPIN?\r\n");
	ATcmd_Wait("OK","未识别到SIM卡\r\n",2000,0);

	//ATcmd_Send("AT+QICLOSE=0\r\n");//断开连接

	ATcmd_Set("AT+CREG?\r\n");//查看是否注册GSM网络
	if(ATcmd_Wait("+CREG: 0,1",NULL,1000,2))
	{
		Debug_printf("已注册GSM网络\r\n");
	}
	else if (ATcmd_Scan("+CREG: 0,5"))
	{
		Debug_printf("已注册GSM网络(漫游)\r\n");
	}
	else Debug_printf("未注册GSM网络\r\n");

	ATcmd_Set("AT+CGREG?\r\n");//查看是否注册GPRS网络
	if(ATcmd_Wait("+CGREG: 0,1",NULL,1000,2))
	{
		Debug_printf("已注册GPRS网络\r\n");
	}
	else if (ATcmd_Scan("+CGREG: 0,5"))
	{
		Debug_printf("已注册GPRS网络(漫游)\r\n");
	}
	else Debug_printf("未注册GPRS网络\r\n");

	ATcmd_Send("AT+QIACT=1\r\n");//激活场景，注册基站，获取IP
	ATcmd_Send("AT+QIACT?\r\n");//查询IP
}

void MQTT_Init(void)
{
	//ATcmd_Make(1,"QFDEL","cacert.pem");
	// ATcmd_Make(3,"QFUPL","cacert.pem","1360","100");
	// if(!ATcmd_Wait("+CME ERROR: 407",NULL,200,1))
	// {
	// 	ATcmd_Send(cacert);
	// }
	ATcmd_MakeSend(4,"QMTCFG","recv/mode","0","0","1");
	ATcmd_MakeSend(4,"QMTCFG","SSL","0","0","2");
	// ATcmd_MakeSend(3,"QSSLCFG","seclevel","2","1");
	// ATcmd_MakeSend(3,"QSSLCFG","cacert","2","cacert.pem");
	// ATcmd_MakeSend(3,"QSSLCFG","sslversion","2","4");
	// ATcmd_MakeSend(3,"QSSLCFG","ciphersuite","2","0XFFFF");
	// ATcmd_MakeSend(3,"QSSLCFG","ignorelocaltime","2","1");

	for (uint8_t i = 1; i <=5; i++)
	{
		ATcmd_Make(3,"QMTOPEN","0","broker.emqx.io","1883");//通过TCP方式去连接MQTT服务器
		ATcmd_Wait("QMTOPEN: 0,0",NULL,5000,1);
		if(ATcmd_Scan("QMTOPEN: 0,2"))
		{Debug_printf("MQTT服务器已连接\r\n");break;}
		else Debug_printf("连接MQTT服务器失败\r\n");
		ATcmd_Make(4,"QMTCONN","0","hxd","aystmjz","mmjz2004");//登录MQTT服务器
		if(ATcmd_Wait("QMTCONN: 0,0,0","登录MQTT服务器失败\r\n",3000,1))break;
	}
    ATcmd_Make(4,"QMTSUB","0","1","testtopic/test","0");//订阅主题
	if(ATcmd_Wait("QMTSUB: 0,1,0,0","订阅失败\r\n",1000,5))Debug_printf("订阅成功\r\n");
	Clear_Buffer();
}

uint16_t Get_ADC()
{
	ATcmd_Make(1,"QADC","0");
	if(!ATcmd_Wait("QADC","电池电压获取失败\r\n",1000,1))return 0;
	return Data_ScanInt(",",4);
}




void Pub_String(char* topic,char* str)
{
	uint8_t json_len;
	char len[5];
	json_len = strlen(str)/sizeof(char);
	sprintf(len,"%d",json_len);
	ATcmd_Make(6,"QMTPUBEX","0","0","0","0",topic,len);
	ATcmd_Wait(">","发布错误\r\n",1000,1);
	Main_printf(str);
	uint16_t timeout = 1;
	while(!ATcmd_Scan("QMTPUBEX: 0,0,0"))
	{
   		Delay_ms(1);
   		timeout ++;
    	if(timeout > 2000){Debug_printf("发布失败\r\n");timeout=0;break;}
	}
	if(timeout)Debug_printf("发布成功\r\n");
}

void Pub_Json(char* topic,cJSON* json)
{
	char* str = cJSON_Print(json);
	Pub_String(topic,str);
	cJSON_free(str);
}