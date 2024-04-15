/*
 * Lab6.c
 *
 * Created: 10/27/2023 12:28:10 AM
 * Author : Aaron
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
//#define BAUD 9600 // define baud
//#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) // set baud rate value for UBRR
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

int grid[4][4] = {
	{1, 2, 3, 0},
	{4, 5, 6, 0},
	{7, 8, 9, 0},
	{0, 0, 0, 0},
};
int duty;

void turn_on(void) {
	PORTB |= (1<<5);	//turn on LED
}

void turn_off(void) {
	PORTB &= ~(1<<5);	//turn off LED
}

//826559013
//timer interrupt period = (0)+1 = 1ms
//PWM freq = ((3)+1)*100Hz = 400Hz = 2.5ms
void init() {
	//timer 0
	TCCR0A |= (1<<WGM01);					//enable CTC mode
	TCCR0B |= (1<<CS00) | (1<<CS01);		//set prescaler to 64 to allow for more accurate OCR0A value
	TIMSK0 |= (1<<OCIE0A);					//set overflow interrupt
	OCR0A = 249;
	//We want timer interrupt period to equal 1ms = 0.001
	//(0.001 * 16000000) / 64 = 250 - 1, 249

	//timer 2
	TCCR2A |= (1<<WGM21);					//enable CTC mode
	TCCR2B |= (1<<CS22);					//set prescaler to 256 to allow for more accurate OCR0A value
	TIMSK2 |= (1<<OCIE2A) | (1<<OCIE2B);	//set overflow interrupts
	OCR2A = 155;
	//We want PWM frequency to equal 2.5ms = 0.0025
	//(0.0025 * 16000000) / 256 = 156.25 - 1, about 155
	
	//input & outputs
	DDRD |= (1<<4) | (1<<5) | (1<<6) | (1<<7);		//set D4-D7 to rows/outputs
	DDRB &= ~(1<<0) & ~(1<<1) & ~(1<<2) & ~(1<<3);	//set B0-B3 to cols/inputs
	PORTD |= (1<<4|1<<5|1<<6|1<<7);					//set rows PD4,5,6,7 high on AVR
	PORTB |= (1<<0|1<<1|1<<2|1<<3);					//enable pull up resistors in cols PB0,1,2,3 on AVR
	
	DDRB |= (1<<DDB5);		//LED = output
}

ISR (TIMER2_COMPA_vect) {
	turn_on();
}

ISR (TIMER2_COMPB_vect) {
	turn_off();
}
 
ISR (TIMER0_COMPA_vect) {
	int i;
	int j;
	for (i = 4; i < 8; i++) {
		//D = row
		PORTD &= ~(1 << i);				//set each row low
		for (j = 0; j < 4; j++) {
			//B = column
			if (!(PINB & (1<<j))) {		//check each column
				duty = grid[i-4][j];
				OCR2B = (OCR2A / 10) * duty;	//ten possible values 0-9, multiplied by percentage of duty
			}
		}
		PORTD |= (1<<i);				//set each row back to high after checking
	}
}

int main(void) {
	turn_on();
    init();
	sei();	//enable interrupts
    while (1) {
    }
}
