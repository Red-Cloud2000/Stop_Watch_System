
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "macros.h"

/************************************************************************************************
 *                                        Variables                                         *
 ************************************************************************************************/
unsigned char count_up = 0; //our stop watch interrupt flag

/*6 digits 7-segment*/
unsigned char SEC1  = 0;
unsigned char SEC2  = 0;
unsigned char MIN1  = 0;
unsigned char MIN2  = 0;
unsigned char HOUR1 = 0;
unsigned char HOUR2 = 0;

unsigned char i; //for iterating in loops

/***************************************************************************************************
 *                                     Interrupt Functions                                    *
 ***************************************************************************************************/


/************************************************************************************
 * Service Name    : Timer1_init_compare_mode + Its ISR
 * Parameters (in) : None
 * Parameters (out): None
 * Return value    : None
 * Description     : A function that consists of the initialization of timer1
 *                   registers specially; its interrupt and the global interrupt
 ************************************************************************************/
void Timer1_init_compare_mode(void){
	/*an included AVR function that sets the global interrupt
	 * ---the ones that clear it is cli()---
	 */
	sei();
	TCCR1A|=0;                             //CTC register
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS11); //CTC register
	TCNT1=0;                               //Timer start value
	TIMSK |= (1<<OCIE1A);                  //timer1 interrupt
	OCR1A= 15625;                          //compare match register
}
/*the ISR of timer1 that here we made it changes a variable value to use it as
 * flag in the while loop not to waste time here in the ISR and may lead to make
 * other critical interrupts wait till this one finishes
 */
ISR(TIMER1_COMPA_vect){
	count_up = 1;
}

/************************************************************************************
 * Service Name    : INT0_Init + Its ISR
 * Parameters (in) : None
 * Parameters (out): None
 * Return value    : None
 * Description     : A function that consists of the initialization of interrupt0
 *                   registers which we uses here as a reset button.
 *                   specially; its interrupt and the global interrupt
 *                   so it only waits until the interrupt flag is raised when
 *                   the button is pressed to complete the interrupt AND gate
 *                   then generate the interrupt and call the ISR of that interrupt
 ************************************************************************************/
void INT0_Init(void){
	/*configure pin 2 of PORTD as input pin for reset*/
	CLR_BIT(DDRD ,2);
	/*enabling pull-up as the button is active low and we want to generate
	 *the interrupt when a falling edge comes
	 */
	SET_BIT(PORTD,2);

	/*a falling edge will generate the interrupt
	 * So as the this button is Active low means that when you press/close the
	 * switch, then the signal sent to the MCU will be LOW. As you can see from
	 * schematic in protues, when the switch is open, the signal sent to MCU
	 * is actually HIGH, and when the switch is closed, the MCU pin will be
	 * directly connected to GND. That is why this kind of configuration
	 * is Active-LOW configuration.
	 * */
	MCUCR = (1<<ISC01);
	GICR |= (1<<INT0);     //INT0 interrupt
	sei();                 //global interrupt
}
/*The ISR of INT0 is used for reseting the stop watch*/
ISR(INT0_vect){
	SEC1  = 0;
	SEC2  = 0;
	MIN1  = 0;
	MIN2  = 0;
	HOUR1 = 0;
	HOUR2 = 0;
}

/************************************************************************************
 * Service Name    : INT1_Init + Its ISR
 * Parameters (in) : None
 * Parameters (out): None
 * Return value    : None
 * Description     : A function that consists of the initialization of interrupt1
 *                   registers which we uses here as a pause button.
 *                   specially; its interrupt and the global interrupt
 *                   so it only waits until the interrupt flag is raised when
 *                   the button is pressed to complete the interrupt AND gate
 *                   then generate the interrupt and call the ISR of that interrupt
 ************************************************************************************/
void INT1_Init(void){
	/*configure pin 3 of PORTD as input pin for pause*/
	CLR_BIT(DDRD,3);
	/*a rising edge will generate the interrupt*/
	/*same idea as we discussed at INT0 but button this time is Active high*/
	MCUCR |= (1<<ISC11)|(1<<ISC10);
	GICR  |= (1<<INT1);     //INT0 interrupt
	sei();                  //global interrupt
}
/*The ISR of INT1 is used for pausing the stop watch by clearing pre_scaler bits (64) */
ISR(INT1_vect){
	TCCR1B = (1<<WGM12);
}

/************************************************************************************
 * Service Name    : INT2_Init + Its ISR
 * Parameters (in) : None
 * Parameters (out): None
 * Return value    : None
 * Description     : A function that consists of the initialization of interrupt2
 *                   registers which we uses here as a resume button.
 *                   specially; its interrupt and the global interrupt
 *                   so it only waits until the interrupt flag is raised when
 *                   the button is pressed to complete the interrupt AND gate
 *                   then generate the interrupt and call the ISR of that interrupt
 ************************************************************************************/
void INT2_Init(void){
	/*configure pin 2 of PORTB as input pin for resume*/
	CLR_BIT(DDRB ,2);

	/*enabling pull-up as the button is active low and we want to generate
	 *the interrupt when a falling edge comes
	 */
	SET_BIT(PORTB,2);
	/*a falling edge will generate the interrupt (AVR data sheet page 69)*/
	/*same idea as we discussed at INT0*/
	CLR_BIT(MCUCSR ,ISC2);
	GICR |= (1<<INT2);     //INT0 interrupt
	sei();                 //global interrupt
}
/*The ISR of INT2 is used for resuming the stop watch by setting pre_scaler bits (64) */
ISR(INT2_vect){
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS11);
}


/***************************************************************************************************
 *                            General Functions                                           *
 ***************************************************************************************************/


/************************************ ************************************************
 * Service Name     : _7segment_config
 * Parameters (in)  : None
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function to configure the seven segment inputs through
 *                    the outputs of the ATmega16 M.C.
 ************************************************************************************/
void _7segment_config(void){
	/*configure pin zero of PORTD as input pin for minutes incrementing*/
	CLR_BIT(DDRD ,0);
	/*configure pin zero of PORTD as input pin for hours incrementing*/
	CLR_BIT(DDRD ,1);
	/*configure first four pins of PORTC as output pins for the decoder for the multiplexed technique*/
	for(i=0;i<4;i++){
		SET_BIT(DDRC,i);
	}
	/* configure first six pins of PORTA as output pins for the multiplexed technique as switch on/off npn transistor*/
	for(i=0;i<6;i++){
		SET_BIT(DDRA,i);
	}
	/*initialize the 7-segment with value 0 by clearing the first four bits in PORTC*/
	for(i=0;i<4;i++){
		CLR_BIT(PORTC,i);
	}
	/*initialize the 7-segments with value 1 to enable the 6 digits 7-segment by setting the first six bits in PORTA*/
	for(i=0;i<6;i++){
		SET_BIT(PORTA,i);
	}
}

/************************************ ************************************************
 * Service Name     : countUpController
 * Parameters (in)  : None
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function to control the 24h stop watch count up
 ************************************************************************************/
void countUpController(void){
	if(1==count_up){	/*checking the interrupt flag to ensure the compare match occurred or not*/

		count_up=0;		/*return the flag to zero not to increment without an interrupt happens*/

		SEC1++;		    /*Incrementing seconds each compare match*/
		/*------------------------------------------------------------------------------*/
		if(SEC1 == 10)		/*some ifs and elses to control the 24h stop watch count up*/
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

			BLINK_EVERY_HOUR(PORTA,200)     /*every hour blink for (10)times the stop watch with zeros displayed on the 6 digits 7-segment
			 ******************************************
			 *****you will find a description in the video****
			 *******************************************/

			SEC1+=4;                       /*conquering blinking delay
			 * (((calculated)))*/
		}
		if(HOUR1 == 10){
			HOUR1 = 0;
			HOUR2++;
		}
		if(HOUR1 == 4 && HOUR2 == 2){
			SEC1  = 4;
			SEC2  = 0;
			MIN1  = 0;
			MIN2  = 0;
			HOUR1 = 0;
			HOUR2 = 0;
		}
	}
	/*--------------------------------end of if else timer control-----------------------------------------------*/
}

/************************************ ************************************************
 * Service Name     : SegmentBySegment_WRITE
 * Parameters (in)  : the value to display (second,minute,hour)
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function to write on the 7 segment and always display all
 *                    6 digits while the stop watch is on
 ************************************************************************************/
void SegmentBySegment_WRITE(unsigned char SMH , unsigned char spot){ //SMH: second-minute-hour
	for(i=0;i<6;i++){
		CLR_BIT(PORTA,i); /*disable all display pins*/
	}
	/*enable just 1pin at a time with a very small unrecognizable delay and update the reading every time*/
	SET_BIT(PORTA,spot);
	PORTC = (PORTC & 0xF0)|(SMH & 0x0F);
	_delay_ms(5);
}

/************************************************************************************
 * Service Name     : button_is_pressed
 * Parameters (in)  : the Pin Port and the specific pin to check if 0 or 1
 * Parameters (out) : just a flag
 * Return value     : 1 if if button is pushed and 0 if not
 * Description      : A function to check if a button is pressed or not and depending on
 *                    that returns a value like a flag to check it
 ************************************************************************************/
unsigned char button_is_pressed(unsigned char PINx , unsigned char spot)
{
	//the button is pressed when BUTTON_BIT is clear

	if(IS_BIT_SET(PINx,spot)){
		_delay_ms(DEBOUNCE_TIME); //to conquer debouncing
		if (IS_BIT_SET(PINx,spot))
			return 1;
	}
	return 0;
}

/************************************************************************************
 * Service Name     : MIN_INC
 * Parameters (in)  : pin port connected to the push button and the specific pin itself
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function to increment minutes by 1 to use it in setting the stop watch time
 ************************************************************************************/
void MIN_INC(unsigned char PINx,unsigned char spot)
{
	if (button_is_pressed(PINx ,spot))
	{
		MIN1++;
		if(MIN1 == 10)
		{
			MIN1 = 0;
			MIN2++;
		}
		if(MIN2 == 6)
		{
			MIN2 = 0;
			HOUR1++;
		}
		_delay_ms(LOCK_INPUT_TIME);
	}
}

/************************************************************************************
 * Service Name     : HOUR_INC
 * Parameters (in)  : pin port connected to the push button and the specific pin itself
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function to increment hours by 1 to use it in setting the stop watch time
 ************************************************************************************/
void HOUR_INC(unsigned char PINx,unsigned char spot)
{
	if (button_is_pressed(PINx ,spot))
	{
		HOUR1++;
		if(HOUR1 == 10)
		{
			HOUR1 = 0;
			HOUR2++;
		}
		if(HOUR1 == 4 && HOUR2 == 2)
		{
			SEC1  = 0;
			SEC2  = 0;
			MIN1  = 0;
			MIN2  = 0;
			HOUR1 = 0;
			HOUR2 = 0;
		}
		_delay_ms(LOCK_INPUT_TIME);
	}
}

/************************************************************************************
 * Service Name     : SEC58_MIN59
 * Parameters (in)  : None
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function for debugging that makes minutes be 59 and seconds 58
 *                    not to waste many time to see if all cases we made are working or not
 ************************************************************************************/
void SEC58_MIN59(void)
{
	if(button_is_pressed(PIND ,4))
	{
		SEC1=8;
		SEC2=5;
		_delay_ms(LOCK_INPUT_TIME);
	}
	if(button_is_pressed(PIND ,5))
	{
		MIN1=9;
		MIN2=5;
		_delay_ms(LOCK_INPUT_TIME);
	}
}
