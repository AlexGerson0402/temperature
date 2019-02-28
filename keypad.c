#include <inttypes.h>
#include <stdio.h>
#include "gpio.h"
#include "keypad.h"
#include "lcd.h"
#include "string.h"
#include "delay.h"

static volatile GPIO* GPIOC = 0x40020800;
static char keys[] = "123A456B789C*0#D";

/*Initialize the keypad GPIO port.
 *Depending on your scanning algorithm,
 *Depending there may not be any work to do in this method.
 * */
void key_init(){
	////RCC enable for GPIOC
	//PC[0..3]=COL[1..4], PC[4..7]=ROW[1..4]
	*(RCC_AHB1ENR) |= (GPIOC_EN);

	//ROW1-4 pull up
	//COL1-4 pull up
	GPIOC->PUPDR = GPIOC->PUPDR & ~(0xFFFF);
	GPIOC->PUPDR |= (0x5555);
}

//col in row out
void CoutRin(){
	/*MODER Port C
	  COL4-1 output PC3-0
	  ROW4-1 input PC7-4*/
	GPIOC->MODER = GPIOC->MODER & ~(0xFFFF);
	//C3-0 output, zero extend C7-4 input
	GPIOC->MODER |= (0x0055);

}

//col out row in
void CinRout(){
	/*MODER Port C
	  COL4-1 input PC3-0
	  ROW4-1 output PC7-4*/
	GPIOC->MODER = GPIOC->MODER & ~(0xFFFF);
	GPIOC->MODER |= 0x5500;
}

/*Returns a numeric code representing
 *button on the keypad that was pressed (1 to 16),
 *button or 0 if no button is pressed.
 *
 */
uint32_t key_getkey_noblock(){
	/*COL4-1 output PC3-0
	  ROW4-1 input PC7-4*/
	CoutRin();
	//send 0 to column
	GPIOC->ODR = GPIOC->ODR & ~(0xF);
	//10 us delay
	delay_1us(10);
	//read IDR of row
	uint32_t rowVal = GPIOC->IDR & 0xF<<4;
	rowVal = rowVal>>4;

	uint32_t rowNum=0; //initialize as no key press
	if(rowVal==0b1110){
		rowNum=1;
	}else if(rowVal==0b1101){
		rowNum=2;
	}else if(rowVal==0b1011){
		rowNum=3;
	}else if(rowVal==0b0111){
		rowNum=4;
	}

	/*COL4-1 PC3-0
	  ROW4-1 PC7-4*/
	CinRout();
	// put IDR value of row in its ODR
	GPIOC->ODR = GPIOC->ODR & ~(0xF0);
	GPIOC->ODR |= rowVal<<4;

	delay_1us(10);

	//read 0 on col IDR
	uint32_t colIDR = GPIOC->IDR & 0xF;
	uint32_t colNum = 0;
	if(colIDR==0b1110){
			colNum=1;
		}else if(colIDR==0b1101){
			colNum=2;
		}else if(colIDR==0b1011){
			colNum=3;
		}else if(colIDR==0b0111){
			colNum=4;
		}

	//return key press
	if(colNum==0){
		return 0;
	}else{
		return (rowNum-1)*4 + colNum;
	}

}


/*Same as key_getkey_noblock, but blocks
 *that is, waits until a key is pressed and then returns the key code.  
 *You may wish to return after said key is released.
 */
uint32_t key_getkey(){
	uint32_t stay = 1;
	uint32_t keyNum = 0;
	uint32_t temp = 0;
	while(stay){
		keyNum = key_getkey_noblock();
		if(keyNum != 0){
			stay = 0;
		}
	}
	temp = keyNum;
	stay = 1;
	while(stay){
		keyNum = key_getkey_noblock();
		if(keyNum == 0){
			stay = 0;
		}
	}
	return temp;
}


/*Similar to key_getkey,
 *but returns the ASCII code corresponding to the key press.  
 *This method blocks.
 */
char key_getchar(){
	return keys[key_getkey()-1];
}

/*return achii of key press no block*/
char key_noblock(){
	return keys[key_getkey_noblock()-1];
}


