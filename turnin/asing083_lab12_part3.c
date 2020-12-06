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

enum States{init, s1, s2, s3, s4} LEDstate;

int tick(int state)
{
	static unsigned char pattern = 0x80;
	static unsigned char row = 0x00;
	switch(LEDstate)
	{
		case init:
			LEDstate = s1;
			break;
		case s1:
			LEDstate = s2;
			break;
		case s2:
			LEDstate = s3;
			break;
		case s3:
			LEDstate = s4;
			break;
		case s4:
			LEDstate = s1;
			break;
		default:
			LEDstate = init;
			break;

	}
	switch(LEDstate)
	{
		case init:
			break;
		case s1:
			pattern = 0x20;
			row = ~0x0E;
			break;
		case s2:
			pattern = 0x10;
			row = ~0x0A;
			break;
		case s3:
			pattern = 0x08;
			row = ~0x0A;
			break;
		case s4:
			pattern = 0x04;
			row = ~0x0E;
			break;
		default:
			break;
	}
			PORTC = pattern;
			PORTD = row;

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
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &tick;

	TimerSet(1);
	TimerOn();

    		unsigned short i;
    	while (1) {
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    	}
    	return 1;
}
