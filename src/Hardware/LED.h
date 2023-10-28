/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-07-27 20:28:03
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-08 17:37:40
 * @FilePath: \mqtt\HARDWARE\LED\led.h
 * @Description: ??????,???`customMade`, ??koroFileHeader???? ????: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"                  // Device header


#define LED1 PBout(0)
#define LED2 PAout(8)
#define PWRKEY PAout(0)
void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);


#endif
