#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "uart_driver.h"
#include "gpio.h"
#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "keypad.h"
#include "ADC.h"
#include <math.h>

static volatile GPIO* GPIOA = 0x40020000;
static volatile GPIO* GPIOB = 0x40020400;
static volatile GPIO* GPIOC = 0x40020800;

void ADC_init(){ //DONE!!!
	//RCC GPIO PB0
	*RCC_AHB1ENR |= GPIOB_EN;
	//RCC ADC 1EN
	*RCC_APB2ENR |= ADC1_EN;
	//temp on PB0
	GPIOB->MODER |= 0b11; //analog mode
	//turn on ADC1
	*ADC1_CR2 |= 1; //ADON
	//select channel 8, one channel scan
	*ADC1_SQR3 &= ~(0b11111);
	*ADC1_SQR3 |= 8;
}

//return ADC_DR
uint32_t measure(){ // DONE!!!
	// START conversion of regular channel
	*ADC1_CR2 |= 1<<30;
	//poll status Reg for EOC
	uint32_t EOC = *ADC1_SR & 1<<1;
	while(EOC != 1<<1){
		EOC = *ADC1_SR & 1<<1; // EOC is bit 1
		//check if conversion is completed
	}
	return *ADC1_DR;
}

//Done!!!
float DRtoTEMP(int DR){
	float DRf = (float) DR;
	// ADC_DR come in parameter
	// DR=DR*10000
	// 119470 in DR is 1 degree increase from -57
	//1 place after decimal
	return (DRf*10000/119470-57);
}



//DONE!!!
float Ftemp(float Ctemp){
	return Ctemp*9/5+32;
}
