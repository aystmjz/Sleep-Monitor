#ifndef __SERIAL_H_
#define __SERIAL_H_
#include "stm32f10x.h"

void Usart3SerialInit(void);
uint8_t Usart3SerialGetRxFlag(void);
uint8_t Usart3SerialGetRxData(void);

#endif
