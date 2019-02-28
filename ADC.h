#ifndef ADC_H_
#define ADC_H_

#include "gpio.h"
#include <inttypes.h>
#include <stdio.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830

#define RCC_APB1ENR (volatile uint32_t*) 0x40023840 // TIM3
#define RCC_APB2ENR (volatile uint32_t*) 0x40023844

#define GPIOB_EN 1<<1
#define GPIOA_EN 0b1
#define GPIOC_EN 0b100
#define ADC1_EN 1<<8

//ADC
#define ADC1_BASE (volatile uint32_t*) 0x40012000
#define ADC1_SR (volatile uint32_t*) 0x40012000
#define ADC1_CR2 (volatile uint32_t*) 0X40012008
#define ADC1_SQR3 (volatile uint32_t*) 0x40012034
#define ADC1_DR (volatile uint32_t*) 0X4001204C

void ADC_init();

//return ADC_DR
uint32_t measure();
float DRtoTEMP(int DR);
float Ftemp(float Ctemp);

#endif
