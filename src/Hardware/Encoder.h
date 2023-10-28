#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"                  // Device header

void Encoder_Init(void);
int16_t Encoder_Get(void);
void Encoder_Clear(void);

#endif
