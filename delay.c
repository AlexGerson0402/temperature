/*
 * delay.c
 *
 * Starter Code
 */
#include <stdio.h>
#include <inttypes.h>
#include "delay.h"  //include declaration header file

void delay_1ms(uint32_t n){
	//loop n times
	for(int i=0; i<n; i++){
		//load starting to #numbers of ticks to make 1ms
		*(STK_LOAD) = 16000;

		*(STK_CTRL) = (1<<EN) | (1<<CLKSOURCE);
			//start counter

		uint32_t currFlag = 0;
		uint8_t done =0;
		while (done == 0){
			currFlag = *(STK_CTRL) & (1<<COUNTFLAG);
			if (currFlag == (1<<COUNTFLAG)){
				done = 1;
			}
		}
			//check the flag

				//if flag is 1 done
	}

}

void delay_1us(uint32_t n){
	//loop n times
		for(int i=0; i<n; i++){
			//load starting to #numbers of ticks to make 1us
			*(STK_LOAD) = 16;

			*(STK_CTRL) = (1<<EN) | (1<<CLKSOURCE);
				//start counter

			uint32_t currFlag = 0;
			uint8_t done =0;
			while (done == 0){
				currFlag = *(STK_CTRL) & (1<<COUNTFLAG);
				if (currFlag == (1<<COUNTFLAG)){
					done = 1;
				}
			}
				//check the flag

					//if flag is 1 done
		}
}

void delay_1s(uint32_t n){
	//loop n times
		for(int i=0; i<n; i++){
			//load starting to #numbers of ticks to make 1s
			*(STK_LOAD) = 16000000;

			*(STK_CTRL) = (1<<EN) | (1<<CLKSOURCE);
				//start counter

			uint32_t currFlag = 0;
			uint8_t done =0;
			while (done == 0){
				currFlag = *(STK_CTRL) & (1<<COUNTFLAG);
				if (currFlag == (1<<COUNTFLAG)){
					done = 1;
				}
			}
				//check the flag

					//if flag is 1 done
		}
}
