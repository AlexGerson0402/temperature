#ifndef LCD_H_
#define LCD_H_

#include "gpio.h"
#include <inttypes.h>
#include <stdio.h>



#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define GPIOB_EN 1<<1
#define GPIOA_EN 0b1
#define GPIOC_EN 0b100


void lcd_cmd(uint32_t CD);
void lcd_data(uint32_t CD);
void lcd_exec(uint32_t CD);
void lcd_set(uint32_t CD);
void lcd_latch();

//		• Initializes I/O to interface to communicate with LCD module
//		• Clears and homes the display
//		• No arguments or return
void lcd_init();

//		• clears the display
//		• no arguments or return
//		• includes necessary delay*
void lcd_clear();


//		• moves cursor to the home position
//		• no arguments or return
//		• includes necessary delay*
void lcd_home();

//• moves cursor to the position indicated
//• requires two arguments – a zero-based row, and a zero-based column, no return value
//• includes necessary delay*
void lcd_set_position(uint32_t row, uint32_t col);

//• prints a null terminated string to the display
//• accepts the (pointer to) a null-terminated string, returns the number of characters written to display
//• includes necessary delay*
void lcd_print_string(char *s);


//• prints a (decimal) number to the display
//• accepts the number to be printed, returns the number of characters written to display
//• includes necessary delay*
void lcd_print_num(int n);

void lcd_print_float(float n);


#endif
