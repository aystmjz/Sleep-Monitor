#ifndef __DMA_H__
#define __DMA_H__
#include "stm32f10x.h" // Device header
#include "Delay.h"

void DMA1_Init(DMA_Channel_TypeDef *DMA_CHx, uint32_t cpar, uint32_t cmar, uint16_t cndtr);
void DMA1_Transfer(DMA_Channel_TypeDef *DMA_CHx, uint16_t CounterSize);

#endif
