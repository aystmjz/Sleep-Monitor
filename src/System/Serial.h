#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void uartWriteBuf(uint8_t *buf, uint8_t len);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif
