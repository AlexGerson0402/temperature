#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "gpio.h"
#include <inttypes.h>
#include <stdio.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define GPIOB_EN 1<<1
#define GPIOA_EN 0b1
#define GPIOC_EN 0b100

void key_init();
uint32_t key_getkey_noblock();
uint32_t key_getkey();
char key_getchar();
char key_noblock();

#endif
