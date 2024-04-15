/*
 * Lab4.c
 *
 * Created: 10/6/2023 12:28:10 AM
 * Author : Aaron
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
//#define BAUD 9600 // define baud
//#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) // set baud rate value for UBRR
#include <avr/io.h>
#include <util/delay.h>

void init(void) {
	TCCR0A |= (1 << WGM01); //enable CTC mode
	DDRB &= ~(1<<DDB7);		//button = input
	DDRB |= (1<<DDB5);		//LED = output
	PORTB |= (1<<DDB7);		//set pull-up resistor on button
	
	OCR0A = 0x9B; //Output compare register
	//We want period to equal 10ms = 0.01
	//(0.01 * 16000000) / 1024 = 156.25 - 1, about 9B
	//Cannot use lower prescale than 1024 otherwise OCR0A will have to be > than 255
	//and 0 is only 8 bit timer
	OCR0B = 0x00;	//0% initial duty cycle
	
	TCCR0B |= (1<<CS02) | (1<<CS00);	//set prescaler to 1024
}

void turn_on(void) {
	PORTB |= (1<<5);	//turn on LED
}

void turn_off(void) {
	PORTB &= ~(1<<5);	//turn off LED
}

int main(void)
{
    init();
	turn_off();
    while (1) {
		if (OCR0B != 0) {
			turn_on();
		}
		if (!(PINB & (1<<7)) && (OCR0B < OCR0A)) {	//if button pressed and duty cycle < 100%
			++OCR0B;								//increase duty cycle
		}
		while ((TIFR0 & (1<<OCF0B)) == 0);	//wait for overflow
		TIFR0 |= (1<<OCF0B);				//reset overflow bit
		turn_off();
		if ((PINB & (1<<7)) && (OCR0B > 0)) {
			--OCR0B;
		}
		while ((TIFR0 & (1<<OCF0A)) == 0);	//wait for overflow
		TIFR0 |= (1<<OCF0A);				//reset overflow bit
    }
}
