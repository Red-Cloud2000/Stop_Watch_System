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
               ##It's a 24h stop watch that resets when after reaching 23:59:59##
 ============================================================================
 */

#include <avr/io.h>

#include "StopWatchSystem.h"


/*******************************************************************************
 *                           Variables                              *
 *******************************************************************************/
/*As we use them here as parameters in for SegmentBySegment_WRITE function for displaying*/
extern unsigned char SEC1;
extern unsigned char SEC2;
extern unsigned char MIN1;
extern unsigned char MIN2;
extern unsigned char HOUR1;
extern unsigned char HOUR2;


int main(void){
	_7segment_config();
	/*calling timer1 init function to wait for the compare match then initialize the interrupt to count up*/
	Timer1_init_compare_mode();
	/*
	  calling INT functions for push buttons interrupt:
	  . INT0 : RESET
	  . INT1 : PAUSE
	  . INT2 : RESUME
	 */
	INT0_Init();
	INT1_Init();
	INT2_Init();

	/*Program infinite loop that makes it always work*/
	while(1){
		countUpController();
		MIN_INC (PIND,0);
		HOUR_INC(PIND,1);

		/*-------FOR DEBUGGING------*/
		/*---it makes seconds=58 & minutes=59 not to waste too much time in debugging and for better testing---*/
		SEC58_MIN59();
		/*--------------------------*/

		/*A function for always displaying the stop watch unless clock is off*/
		SegmentBySegment_WRITE(SEC1  , 0);
		SegmentBySegment_WRITE(SEC2  , 1);
		SegmentBySegment_WRITE(MIN1  , 2);
		SegmentBySegment_WRITE(MIN2  , 3);
		SegmentBySegment_WRITE(HOUR1 , 4);
		SegmentBySegment_WRITE(HOUR2 , 5);
	}
}
