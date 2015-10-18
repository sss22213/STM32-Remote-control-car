#ifndef __Delay_H
#define __Delay_H


#include "stm32f10x_lib.h"
void systick_configature(void);
void Delay_Ms(int ms);
void Delay_S(int s);
void Delay_us(int us);

#endif
