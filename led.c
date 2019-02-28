/*
 * led.c
 *
 *  Created on: Dec 1, 2018
 *      Author: livingston
 */

#include "led.h"
#include "delay.h"
#include <inttypes.h>
#include <stdio.h>


//One solution for the current speed to have a local static variable in this file;

static int wait = 1000;
static uint8_t dial = 1;

void init_led(){
	//LED Pins: PB[15..12], PB[10..5]
	//enable RCC
	*(RCC_AHB1ENR) = *(RCC_AHB1ENR) | GPIOBEN;
	// set modes
	*(GPIOB_MODER) = *(GPIOB_MODER) | (0b010101010101)<<10;
	*(GPIOB_MODER) = *(GPIOB_MODER) | (0b010101010101)<<24;
}

void led_allOn(){
	//Insert code here (Hint use ODR)
	//protect other pins
	//clear
	*(GPIOB_ODR) = *(GPIOB_ODR) & ~(0b111111)<<5;
	*(GPIOB_ODR) = *(GPIOB_ODR) & ~(0b1111)<<12;
	//set
	*(GPIOB_ODR) = *(GPIOB_ODR) | (0b111111)<<5;
	*(GPIOB_ODR) = *(GPIOB_ODR) | (0b1111)<<12;
}

void led_allOff(){
	//Insert code here (Hint use ODR)
	//protect other pins
	//clear
	*(GPIOB_ODR) = *(GPIOB_ODR) & ~(0b111111)<<5;
	*(GPIOB_ODR) = *(GPIOB_ODR) & ~(0b1111)<<12;
}

void led_on(uint8_t ledIndex){
	//Insert code here (Hint use BSRR may be helpful)
	*(GPIOB_ODR) = *(GPIOB_ODR) | 1<<ledIndex;
}

void led_off(uint8_t ledIndex){
	//Insert code here (Hint use BSRR may be helpful)
	*(GPIOB_ODR) = *(GPIOB_ODR) & ~1<<ledIndex;
}

void led_scan(){
	//LED Pins: PB[15..12], PB[10..5]

		int i=15;
		led_on(i);
		delay_1ms(wait);
		for(i=15;i>12;i--){
			*(GPIOB_ODR) = *(GPIOB_ODR)>>1;
			delay_1ms(wait);
		}
		led_off(i);
		i=10;
		led_on(i);
		delay_1ms(wait);
		for(i=10;i>5;i--){
			*(GPIOB_ODR) = *(GPIOB_ODR)>>1;
			delay_1ms(wait);
		}
		//scan back
		for(i=5;i<10;i++){
			*(GPIOB_ODR) = *(GPIOB_ODR)<<1;
			delay_1ms(wait);
		}
		led_off(i);
		i=12;
		led_on(i);
		delay_1ms(wait);
		for(i=12;i<15;i++){
			*(GPIOB_ODR) = *(GPIOB_ODR)<<1;
			delay_1ms(wait);
		}
}


void led_flash(){
	for(int i=10; i>0; i--){
		led_allOn();
		delay_1ms(wait);
		led_allOff();
		delay_1ms(wait);
	}
}

/*You should also be able speed up and slow down the speed
 *of each pattern on a scale of 0-9 with zero being slow
 * and 9 being fast. T=1/f
 * 10 to 1 Hz, period 100 to 1000 ms */
void led_setSpeed(uint8_t speed){
	// speed 0 - 9
	dial = speed+1;
	wait = 1000/(dial);
}

void led_incSpeed(){
	if(dial<9){
		dial++;
		wait = 1000/dial;
	}
}

void led_decSpeed(){
	if(dial>0){
		dial--;
		wait = 1000/dial;
	}
}

uint8_t getCurrentSpeed(){
	return dial-1;
}

void led_temp(int F){
	//No lights 60F, All lights 80F
	int bar[10]={15,14,13,12,10,9,8,7,6,5};
	int light = (F-60)/2-1;
	for(int i=0;i<10;i++){
		if(i<=light){
			led_on(bar[i]);
		}else{
			led_off(bar[i]);
		}
	}

}
