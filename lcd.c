
#include <inttypes.h>
#include <stdio.h>
#include "lcd.h"
#include "gpio.h"
#include "delay.h"

static volatile GPIO* GPIOA = 0x40020000;
static volatile GPIO* GPIOB = 0x40020400;
static volatile GPIO* GPIOC = 0x40020800;

//command byte comes in LSB of parameter
//DONE!!!
void lcd_cmd(uint32_t CD){
	//make sure rw(PC9) and rs(PC8) are low, C[9..8]=00
	GPIOC->ODR = GPIOC->ODR & ~(0b11)<<8;

	lcd_exec(CD);
}

//data byte comes in LSB of parameter
//DONE!!!
void lcd_data(uint32_t CD){
	//make sure rw(PC9) is low, and rs(PC8) is high, bring E high
	//c[9..8]=01
	GPIOC->ODR = GPIOC->ODR & ~(0b11)<<8;
	GPIOC->ODR |= (0b01)<<8;

	lcd_exec(CD);
}

/*command or data byte comes in LSB of parameter
  only called from lcd_cmd or lcd_data*/
//DONE!!!
void lcd_exec(uint32_t CD){
	//place byte of command/data byte on output
	lcd_set(CD);
	lcd_latch();
	delay_1us(40); //delay 40 us
}

/* places command or data in parameter
   onto PA4-PA11*/
//DONE!!!
void lcd_set(uint32_t CD){
	GPIOA->ODR = GPIOA->ODR & ~(0xFF)<<4; //clear
	GPIOA->ODR |= (CD<<4); //set
	delay_1us(40); //delay 40 us
}

//pulse E
//DONE!!!
void lcd_latch(){
	//bring E high C[10]=1
	GPIOC->ODR |= 1<<10;
	delay_1us(1); //delay 1 us
	//bring E low to latch C[10]=0
	GPIOC->ODR &= ~(1<<10);
	delay_1us(1); // about 1 us
}

//• Initializes I/O to interface to communicate with LCD module
//• Clears and homes the display
//• No arguments or return
//DONE!!!
void lcd_init(){
	//power on delay 5000us
	delay_1us(5000);

	//enable GPIOA and GPIOC
	*(RCC_AHB1ENR) |= (GPIOA_EN|GPIOC_EN);
	//PC8-10 to output
	//clear
	GPIOC->MODER = GPIOC->MODER & ~(0b111111)<<16;
	//set
	GPIOC->MODER |= (0b010101)<<16;

	//PA4-11 to output
	//clear
	GPIOA->MODER = GPIOA->MODER & ~(0xFFFF)<<8;
	//set
	GPIOA->MODER |= (0x5555)<<8;

	//ports are setup, ready to init display
	lcd_cmd(0x38);// 8-bit mode
	lcd_cmd(0x01);// clear
	delay_1us(1500);// clear needs extra time
	lcd_cmd(0x02);// home
	delay_1us(1500);// home needs extra time
	lcd_cmd(0x06);// Entry mode, move right, no shift

	lcd_cmd(0x0F);// Display on, cursor
}

//		• clears the display
//		• no arguments or return
//		• includes necessary delay*
//DONE!!!
void lcd_clear(){
	lcd_cmd(1);
	delay_1us(1520);
}


//		• moves cursor to the home position
//		• no arguments or return
//		• includes necessary delay*
//DONE!!!
void lcd_home(){
	lcd_cmd(0b11);
	delay_1us(1520);
}

//• moves cursor to the position indicated
//• requires two arguments – a zero-based row, and a zero-based column, no return value
//• includes necessary delay*
//DONE!!!
void lcd_set_position(uint32_t row, uint32_t col){
	uint32_t addr = 1<<7;
	addr |= (row<<6); //row bit AC6
	addr |= col; //col in lower 4 bits;

	lcd_cmd(addr);
}

//• prints a null terminated string to the display
//• accepts the (pointer to) a null-terminated string, returns the number of characters written to display
//• includes necessary delay*
//DONE!!!
void lcd_print_string(char *s){
	while(*s != 0){ // null char
		lcd_data(*s);
		s++;
	}

}


//• prints a (decimal) number to the display
//• accepts the number to be printed, returns the number of characters written to display
//• includes necessary delay*
//DONE!!!
void lcd_print_num(int n){
	char buff[50];
	sprintf(buff, "%d", n);
	//pointer parameter
	lcd_print_string(buff);
}

void lcd_print_float(float n){
	char buff[50];
		sprintf(buff, "%.1f", n);
		//pointer parameter
		lcd_print_string(buff);
}
