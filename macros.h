#define SET_BIT(reg,bit)        reg|=  (1<<bit)        //setting  a bit
#define CLR_BIT(reg,bit)        reg&=(~(1<<bit))       //clearing a bit
#define IS_BIT_SET(reg,bit)    (reg&   (1<<bit))>>bit  //checking for 1
#define IS_BIT_CLR(reg,bit)  !((reg&   (1<<bit))>>bit) //checking for 0

/* time to wait while "de-bouncing" button */
#define DEBOUNCE_TIME 25
/* time to wait after a button press */
#define LOCK_INPUT_TIME 180

/************************************************************************************************************************************************
 * These are also some general functions but I decided to make them as macros to practice using macros and get more about it's problems and uses *
 *************************************************************************************************************************************************/

/************************************************************************************
 * Service Name     : BLINK_EVERY_HOUR
 * Parameters (in)  : pin port connected to the push button and the delay for after each blink
 * Parameters (out) : None
 * Return value     : None
 * Description      : A function to blink the 7 segment 10 times with a zero reading
 *                    each 1 hour
 ************************************************************************************/
#define BLINK_EVERY_HOUR(PORTx,DELAY)\
		{\
	for(i=0;i<4;i++){\
		CLR_BIT(PORTC,i);\
	}\
	for(i = 0 ; i < 10 ; i++){\
		PORTx &= ~0x3F;\
		_delay_ms(DELAY);\
		PORTx |= 0x3F;\
		_delay_ms(DELAY);\
	}\
		}
