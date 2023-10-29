#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "stdio.h"
#include "string.h"


#define USART_REC_LEN 120


void uart1_init(uint32_t bound);
void uart2_init(uint32_t bound);
void uart3_init(uint32_t bound);
void Uart_Init(uint32_t bound);
void Uart2_SendDATA(u8 *SendBuf, u8 len);
void Uart3_SendStr(char *SendBuf);
void USART1_Send_byte(uint8_t byte);
void USART1_Send_bytes(uint8_t *Buffer, uint8_t Length);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#endif


