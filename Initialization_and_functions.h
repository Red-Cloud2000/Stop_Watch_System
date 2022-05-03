void Timer1_init_compare_mode(void){
	sei();
	SREG|=(1<<7);
	TCCR1A|=0;
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TCNT1=0;
	TIMSK |= (1<<OCIE1A);
	OCR1A= 1000;
}
ISR(TIMER1_COMPA_vect)
{
	count = 1;
}

//---------------------------

void SegmentBySegment(unsigned char SMH , unsigned char spot){ //SMH: second-minute-hour
	int i;
	for(i=0;i<6;i++){
		CLR_BIT(PORTA,i);}
	SET_BIT(PORTA,spot);
	PORTC = (PORTC & 0xF0)|(SMH & 0x0F);
	_delay_ms(5);
}
