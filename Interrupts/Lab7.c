/*
 * Lab7.c
 *
 * Created: 11/17/2023 12:28:10 AM
 * Author : Aaron
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t var = 0;

void turn_on(void) {
	PORTB |= (1<<5);	//turn on LED
}

void turn_off(void) {
	PORTB &= ~(1<<5);	//turn off LED
}

//826559013
//((0) + 1 = 1ms = 0.001s interrupt period to start A/D (sample rate)
//Potentiometer connected to A/D channel number (1)%6 = 1
//PWM frequency = ((3)+1)*100Hz = 400Hz = period: 0.0025s
void init_timer0() {
	//timer 0
	TCCR0A |= (1<<WGM01);					//enable CTC mode
	TCCR0B |= (1<<CS00) | (1<<CS01);		//set prescaler to 64 to allow for more accurate OCR0A value
	TIMSK0 |= (1<<OCIE0A);					//set overflow interrupt
	OCR0A = 249;
	//We want timer interrupt period to equal 1ms = 0.001
	//16000000/(64*1000/(0.001)) = 250 - 1, 249
	//(0.001 * 16000000) / 64 = 250 - 1, 249
}

void init_timer2(uint16_t var) {
	//timer 2
	TCCR2A |= (1<<WGM21);					//enable CTC mode
	TCCR2B |= (1<<CS22);					//set prescaler to 256 to allow for more accurate OCR0A value
	TIMSK2 |= (1<<OCIE2A) | (1<<OCIE2B);	//set overflow interrupts
	OCR2A = 155;
	//We want PWM frequency to equal 2.5ms = 0.0025
	//(0.0025 * 16000000) / 256 = 156.25 - 1, about 155
	OCR2B = OCR2A * (var/1024.00);
}

void init_adc() {
	DDRC &= ~(1 << PINC1);																//Setting input to 1%6 = 1
	DDRB |= (1<<DDB5);																	//Output = LED
	
	//VCC reference:
	ADMUX |= (1 << REFS0) | (1 << MUX0);	//MUX0: analog input channel, REFS1 = 0, REFS0 = 1: AVcc with external capacitor at AREF pin
	//or (ADMUX |= 0b01000001;)
	
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS1) | (1 << ADPS0);	//Enable ADC Auto Trigger & Conversion Complete Interrupt
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0);												//Compare Match
}

ISR (ADC_vect) {
	var = ADC;		//when ADC conversion complete, save value to var
	init_timer2(var);
}

ISR (TIMER0_COMPA_vect) {
	ADCSRA |= (1<<ADSC);	//start conversion when timer 0 overflow occurs
}

ISR (TIMER2_COMPA_vect) {
	turn_on();				//turn on LED
}

ISR (TIMER2_COMPB_vect) {
	turn_off();				//turn off LED
}

int main(void) {
	turn_on();
    init_timer0();
	init_adc();
	sei();			//enable interrupts
    while (1) {
	}
}
