#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "stdio.h"
#include "string.h"


#define USART_REC_LEN 120

void Uart_Init(uint32_t bound);
void Uart2_SendDATA(u8*SendBuf,u8 len);
void Uart3_SendStr(char*SendBuf);

#endif


