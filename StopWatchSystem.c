/*
 ============================================================================
 Name        : stopWatchSystem.c
 Author      : Abdelrahman Hesham
 Created on  : May 3, 2022
 Version     :
 Copyright   : Your copyright notice
 Description : A design for a stop watch using ATmega16/32 M.C.(with AVR M.P.) consisting
               of six 7-segments; two for seconds, another two for minutes and the last for hours.
               The 7-segments are always displaying their read without any off times unless we
               stopped the clock.
               Here we have 3 buttons; one for Reset the stop watch, one to stop it without powering off and
               the last one to resume.
 ============================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "macros.h"
#include "headers.h"
#include "Initialization.h"


int main(void){
	int i;
	Timer1_init_compare_mode();
	for(i=0;i<4;i++){
		SET_BIT(DDRC,i); // configure first four pins of PORTC as output pins
	}
	for(i=0;i<6;i++){
		SET_BIT(DDRA,i); // configure first six pins of PORTA as output pins
	}
	/*initialize the 7-segment with value 0 by clearing the first four bits in PORTC*/
	for(i=0;i<4;i++){
		CLR_BIT(PORTC,i);
	}
	/*initialize the 7-segments with value 1 by setting the first six bits in PORTA*/
	for(i=0;i<6;i++){
		SET_BIT(PORTA,i);
	}
	while(1){
		if(1==count){
			count=0;
			SEC1++;
			if(SEC1 == 10)
			{
				SEC1 = 0;
				SEC2++;
			}
			if(SEC2 == 6)
			{
				SEC2 = 0;
				MIN1++;
			}
			if(MIN1 == 10)
			{
				MIN1 = 0;
				MIN2++;
			}
			if(MIN2 == 6){
				MIN2 = 0;
				HOUR1++;
			}
			if(HOUR1 == 9){
				HOUR1 = 0;
				HOUR2++;
			}
			if(HOUR1 == 4 && HOUR2 == 2){
				SEC1  = 0;
				SEC2  = 0;
				MIN1  = 0;
				MIN2  = 0;
				HOUR1 = 0;
				HOUR2 = 0;
			}
		}
		SegmentBySegment(SEC1  , 0);
		SegmentBySegment(SEC2  , 1);
		SegmentBySegment(MIN1  , 2);
		SegmentBySegment(MIN2  , 3);
		SegmentBySegment(HOUR1 , 4);
		SegmentBySegment(HOUR1 , 5);

	}
}
