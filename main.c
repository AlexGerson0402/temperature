/*
 * main.c
 *
 *  Starter Code
 */
//Lab4 Temperature
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "uart_driver.h"
#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "keypad.h"
#include "ADC.h"


//systick addr already defined in delay.h
#define SYSTICK_BASE 0XE000E010

static float temp=0;
static float hot=-100;
static float cold=100;
static bool toggle = false; // C/F toggle
static char key = 0;
//function declarations
void trouble();
void intro();
void extreme();
void STK_init();
// main
int main(){

	init_led();
	lcd_init();
	key_init();
	lcd_clear();
	lcd_home();
	ADC_init();

	while(1){
		key=key_noblock();
		temp=DRtoTEMP(measure());
		led_temp(Ftemp(temp));

		if(temp>hot){
			hot=temp;
		}
		if(temp<cold){
			cold=temp;
		}

		if(key=='A'){
			intro();
		}else if(key=='B'){
			extreme();
		}else if(key=='C'){
			toggle = ~toggle;
			if(toggle){
				lcd_clear();
				lcd_set_position(1,0);
				lcd_print_float(Ftemp(temp));
				delay_1ms(500);
			}
		}else if(key=='D'){
			trouble();
		}else{
			lcd_clear();
			lcd_home();
			lcd_print_float(temp);
			delay_1ms(500);
		}

		key=0;
	}//while

	return 0;
}//main



//void SysTick_Handler(){
//	temp = DRtoTEMP(measure());
//	led_temp(Ftemp(temp));
//	if(temp>hot){
//		hot=temp;
//	}
//	if(temp<cold){
//		cold=temp;
//	}
//	if(toggle==false && key == 0){
//		lcd_clear();
//		lcd_home();
//		lcd_print_float(temp);
//	}else if(toggle==true && key == 0){
//		lcd_clear();
//		lcd_home();
//		lcd_print_float(Ftemp(temp));
//	}
//}

void STK_init(){ //DONE!!!
	//1 tick = 190ns
	//16000 ticks 1ms
	*(STK_LOAD) = (16000*1000);
	//ENABLE|CLKSOURCE|TICKINT
	*(STK_CTRL) |= 0b111;
	//CLOCK started
}

//3.3V 12bit DR, 2^12=4096
void trouble(){ //DONE!!!
	float volt = ((float) measure())/4096*3.3;
	char buff[50];
	lcd_set_position(0,0);
	sprintf(buff, "volt: %.2f", volt);
	lcd_print_string(buff);
	lcd_set_position(1,0);
	sprintf(buff, "temp: %.1f C", temp);//file static temp
	lcd_print_string(buff);
	delay_1s(1);
}

//intro/help screen outlines the basic API
void intro(){
	lcd_clear();
	lcd_home();
	char line0[] = "C toggle C/F";
	char line1[] = "A help info";
	char line2[] = "B extreme temp";
	char line3[] = "D trouble shoot";
	lcd_print_string(line0);
	lcd_set_position(1,0);
	lcd_print_string(line1);
	lcd_home();
	delay_1s(1);
	lcd_print_string(line2);
	lcd_set_position(1,0);
	lcd_print_string(line3);
	delay_1s(1);
}

//display warmest and coldest temperature
void extreme(){
	char hottest[]= "hottest";
	char coldest[] = "coldest";
	lcd_clear();
	lcd_home();
	lcd_print_string(hottest);
	lcd_set_position(1,0);
	lcd_print_float(hot);
	delay_1s(1);
	lcd_home();
	lcd_print_string(coldest);
	lcd_set_position(1,0);
	lcd_print_float(cold);
	delay_1s(1);
}




