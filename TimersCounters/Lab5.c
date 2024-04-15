/*
 * Lab5.c
 *
 * Created: 10/19/2023 12:28:10 AM
 * Author : Aaron
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
//#define BAUD 9600 // define baud
//#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) // set baud rate value for UBRR
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

unsigned int OCR0A_vals[4][4] = {
	{141, 133, 126, 118},
	{112, 105, 99, 94},
	{88, 83, 79, 74},
	{70, 66, 62, 59},
};
//We want period to equal 1/(440*2^(1/12)^x)s
//((1/(440*2^(1/12)^x) * 16000000) / 1024 - 1, x = 0 -> 15

void init() {
	TCCR0A |= (1<<WGM01);				//enable CTC mode
	TCCR0B |= (1<<CS02);	//set prescaler to 256 to allow for more accurate OCR0A values
	
	DDRD |= (1<<4) | (1<<5) | (1<<6) | (1<<7);		//set D4-D7 to rows/outputs
	DDRB &= ~(1<<0) & ~(1<<1) & ~(1<<2) & ~(1<<3);	//set B0-B3 to cols/inputs
	PORTD |= (1<<4|1<<5|1<<6|1<<7);					//set rows PD4,5,6,7 high on AVR
	PORTB |= (1<<0|1<<1|1<<2|1<<3);		//enable pull up resistors in cols PB0,1,2,3 on AVR
	
	DDRD |= (1<<1);	//set PD1 as output on AVR (speaker)
}

void timer(int val) {
	OCR0A = val;					//Output compare register
	OCR0B = val / 2;			//50% initial duty cycle
}

void play(int j) {
	while (!(PINB & (1<<j))) {
		while ((TIFR0 & (1<<OCF0B)) == 0) {	//wait for overflow
			PORTD |= (1<<1);				//set speaker output HIGH
		}
		TIFR0 |= (1<<OCF0B);				//reset overflow flag
		
		while ((TIFR0 & (1<<OCF0A)) == 0) {	//wait for overflow
			PORTD &= ~(1<<1);				//set speaker output LOW
		}
		TIFR0 |= (1<<OCF0A);				//reset overflow flag
	}	
}

void keypad_scan(void) {
	int input;
	int i;
	int j;
	for (i = 4; i < 8; i++) {
		//D = row
		PORTD &= ~(1 << i);				//set each row low
		for (j = 0; j < 4; j++) {
			//B = column
			if (!(PINB & (1<<j))) {		//check each column
				input = OCR0A_vals[i-4][j];
				//while (!(PINB & (1<<j)));
				timer(input);
				play(j);
			}
		}
		PORTD |= (1<<i);				//set each row back to high after checking
	}
}

int main(void)
{
    init();
    while (1) {
		keypad_scan();
    }
}
