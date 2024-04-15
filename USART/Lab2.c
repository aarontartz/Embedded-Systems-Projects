/*
 * Lab2.c
 *
 * Created: 9/16/2023 11:52:49 PM
 * Author : Aaron Tartz
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
#define BAUD 9600 // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) // set baud rate value for UBRR
#include <avr/io.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>

// initialize UART
void uart_init (void) {
	UBRR0H = (BAUDRATE>>8); // shift the register right by 8 bits
	UBRR0L = BAUDRATE; // set baud rate
	UCSR0B|= (1<<TXEN0)|(1<<RXEN0); // enable receiver and transmitter
	UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01); // 8bit data format
}

// send data
void uart_transmit (char data) {
	while (!(UCSR0A & (1<<UDRE0))); // wait while register is free
	UDR0 = data; // load data in the register
}

/*
// receive data
unsigned char uart_receive (void) {
	while (!(UCSR0A & (1<<RXC0))); // wait while data is being received
	return UDR0; // return 8-bit data
}
*/

int main(void) {
	unsigned char redID[] = "826559013\r\n";
	//unsigned char data;
	
	uart_init();
	
	while (1) {
		int i = 0;
		//data = uart_receive();
		//for (int j = 0; j < strlen(redID); j++) {
		while (redID[i] != 0) {
			uart_transmit(redID[i]);
			i++;
		}
		_delay_ms(500);
	}
}
