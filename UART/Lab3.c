/*
 * Lab3.c
 *
 * Created: 9/22/2023 11:52:49 PM
 * Author : Aaron
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
#define BAUD 9600 // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) // set baud rate value for UBRR
#include <avr/io.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>

unsigned char grid[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};

void uart_init(void) {
	UBRR0H = (BAUDRATE>>8); // shift the register right by 8 bits
	UBRR0L = BAUDRATE;		// set baud rate
	DDRD |= (1<<4) | (1<<5) | (1<<6) | (1<<7); //set D4-D7 to rows/outputs (data direction config)
	/* NOTE:
	DDBx is to DDRB what PORTBx is to PORTB;
	e.g. you can write DDRB |= (1<<DDB5) to set 
	pin 5 of port B to output just like you 
	can write PORTB |= (1<<PORTB5) to set 
	pin 5 of port B high.
	*/
	//NOTE: can use DDDx/DDBx instead
	DDRB &= ~(1<<0) & ~(1<<1) & ~(1<<2) & ~(1<<3);	//set B0-B3 to cols/inputs (~(1) = 0)
													//(data direction config)
	
	//NOTE: bitwise |= sets 1, bitwise &= sets 0: can be verified with logic
	
	//NOTE: can use PORTDx/PORTBx instead
	PORTD |= (1<<4|1<<5|1<<6|1<<7);		//set rows PD4,5,6,7 high on AVR
	PORTB |= (1<<0|1<<1|1<<2|1<<3);		//enable pull up resistors in cols PB0,1,2,3 on AVR
	
	UCSR0B|= (1<<TXEN0)|(1<<RXEN0);		//enable receiver and transmitter
	UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);	//8bit data format
}

void uart_transmit(char character) {
	while (!(UCSR0A & (1 << UDRE0)));	//wait for register to be free
	UDR0 = character;
}

void keypad_scan(void) {
	unsigned char input;
	for (int i = 4; i < 8; i++) {
			//D = row
		PORTD &= ~(1 << i);				//set each row low
		for (int j = 0; j < 4; j++) {
			//B = column
			if (!(PINB & (1<<j))) {		//check each column
				input = grid[i-4][j];
				while (!(PINB & (1<<j)));
				uart_transmit(input);
			}
		}
		PORTD |= (1<<i);				//set each row back to high after checking
	}
}

int main(void) {
	uart_init();
	
	while (1) {
		keypad_scan();
		//_delay_ms(50);
	}
}
