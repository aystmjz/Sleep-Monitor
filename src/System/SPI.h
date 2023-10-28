#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"                  // Device header
#include "sys.h"

#define W25Q128_SPI_SS_SET PBout(12)

void W25Q128_SPI_Init(void);
void W25Q128_SPI_Start(void);
void W25Q128_SPI_Stop(void);
uint8_t W25Q128_SPI_SwapByte(uint8_t ByteSend);

#endif
