/*
 * Lab8.c
 *
 * Created: 12/1/2023 12:21:29 AM
 * Author : Aaron
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

uint8_t duty = 0;
uint8_t address = 0;

void turn_on(void) {
	PORTB |= (1<<5);	//turn on LED
}

void turn_off(void) {
	PORTB &= ~(1<<5);	//turn off LED
}

void init() {
	DDRB |= (1<<DDB5);	//output = LED
	
	//timer 0:
	TCCR0A |= (1<<WGM01);					//enable CTC mode
	TCCR0B |= (1<<CS00) | (1<<CS02);		//set prescaler to 1024 to allow for more accurate OCR0A value
	TIMSK0 |= (1<<OCIE0A) | (1<<OCIE0B);	//enable compare match a, b interrupts
	OCR0A = 77;
	//We want timer interrupt period to equal 5ms = 0.005
	//(0.001 * 16000000) / 1024 = 78.125 - 1, 77
}

ISR (TIMER0_COMPA_vect) {
	turn_on();
}

ISR (TIMER0_COMPB_vect) {
	turn_off();
}

int main(void) {
	init();
	duty = eeprom_read_byte(&address);
	
	if (duty < 100) {
		duty = duty + 10;	//add 10 to duty every time device is plugged and unplugged
	}
	else {
		duty = 0;			//once duty hits 100 set duty back to 0
	}
	
	eeprom_write_byte(&address, duty);
	OCR0B = OCR0A * (duty/100.00);
	sei();
	
	while (1) {
	}
}
