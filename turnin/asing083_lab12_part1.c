/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <bit.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char get_press(unsigned char x, unsigned char k) {
    return !((x & (0x01 << k)) != 0);
}

enum States { init, DOWN, UP };
int tick(int state) {

	// Local Variables
	static unsigned char pattern = 0xFF;	
	static unsigned char row = 0xFE;	
							
							
	switch (state) {
		case init:
			if (get_press(PINA, 1)) {
				state = DOWN;
			}
			else if (get_press(PINA, 0)) {
				state = UP;
			}
			break;
		case DOWN:
			if (!get_press(PINA, 1)) {
				state = init;
				if (row != 0xEF) {
					row = ~( ~row << 1);
				}
			}
			break;
		case UP:
			if (!get_press(PINA, 0)) {
				state = init;
				if (row != 0xFE) {
					row = ~( ~row >> 1) | 0x80;
				}
			}
			break;
		default:
			state = init;
			break;
	}
	
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern
	
	return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    
	/* Insert your solutiion below */
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);



	task1.state = 0;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &tick;

	TimerSet(100);
	TimerOn();

    		unsigned short i;
    	while (1) {
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 100;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    	}
    	return 1;
}
