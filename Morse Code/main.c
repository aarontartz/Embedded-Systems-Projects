#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void dot() {
	PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
	_delay_ms(200); //delay 200ms
	PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(200);
}
void dash() {
	PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
	_delay_ms(600); //delay 600ms
	PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(200);
}
void space() {
	_delay_ms(800);
}
void morse() {
	char s[30] = "AARON TARTZ 826559013";
	int length = strlen(s);
	
	for (int i = 0; i < length; i++) {
		if (s[i] == 'A' || s[i] == 'a') {
			dot(); dash();
		}
		else if (s[i] == 'B' || s[i] == 'b') {
			dash(); dash(); dash(); dot();
		}
		else if (s[i] == 'C' || s[i] == 'c') {
			dash(); dot(); dash(); dot();
		}
		else if (s[i] == 'D' || s[i] == 'd') {
			dash(); dot(); dot();
		}
		else if (s[i] == 'E' || s[i] == 'e') {
			dot();
		}
		else if (s[i] == 'F' || s[i] == 'f') {
			dot(); dot(); dash(); dot();
		}
		else if (s[i] == 'G' || s[i] == 'g') {
			dash(); dot(); dot();
		}
		else if (s[i] == 'H' || s[i] == 'h') {
			dot(); dot(); dot(); dot();
		}
		else if (s[i] == 'I' || s[i] == 'i') {
			dot(); dot();
		}
		else if (s[i] == 'J' || s[i] == 'j') {
			dot(); dash(); dash(); dash();
		}
		else if (s[i] == 'K' || s[i] == 'k') {
			dash(); dot(); dash();
		}
		else if (s[i] == 'L' || s[i] == 'l') {
			dot(); dash(); dot(); dot();
		}
		else if (s[i] == 'M' || s[i] == 'm') {
			dash(); dash();
		}
		else if (s[i] == 'N' || s[i] == 'n') {
			dash(); dot();
		}
		else if (s[i] == 'O' || s[i] == 'o') {
			dash(); dash(); dash();
		}
		else if (s[i] == 'P' || s[i] == 'p') {
			dot(); dash(); dash(); dot();
		}
		else if (s[i] == 'Q' || s[i] == 'q') {
			dash(); dash(); dot(); dash();
		}
		else if (s[i] == 'R' || s[i] == 'r') {
			dot(); dash(); dot();
		}
		else if (s[i] == 'S' || s[i] == 's') {
			dot(); dot(); dot();
		}
		else if (s[i] == 'T' || s[i] == 't') {
			dash();
		}
		else if (s[i] == 'U' || s[i] == 'u') {
			dot(); dot(); dash();
		}
		else if (s[i] == 'V' || s[i] == 'v') {
			dot(); dot(); dot(); dash();
		}
		else if (s[i] == 'W' || s[i] == 'w') {
			dot(); dash(); dash();
		}
		else if (s[i] == 'X' || s[i] == 'x') {
			dash(); dot(); dot(); dash();
		}
		else if (s[i] == 'Y' || s[i] == 'y') {
			dash(); dot(); dash(); dash();
		}
		else if (s[i] == 'Z' || s[i] == 'z') {
			dash(); dash(); dot(); dot();
		}
		else if (s[i] == '1') {
			dot(); dash(); dash(); dash(); dash();
		}
		else if (s[i] == '2') {
			dot(); dot(); dash(); dash(); dash();
		}
		else if (s[i] == '3') {
			dot(); dot(); dot(); dash(); dash();
		}
		else if (s[i] == '4') {
			dot(); dot(); dot(); dot(); dash();
		}
		else if (s[i] == '5') {
			dot(); dot(); dot(); dot(); dot();
		}
		else if (s[i] == '6') {
			dash(); dot(); dot(); dot(); dot();
		}
		else if (s[i] == '7') {
			dash(); dash(); dot(); dot(); dot();
		}
		else if (s[i] == '8') {
			dash(); dash(); dash(); dot(); dot();
		}
		else if (s[i] == '9') {
			dash(); dash(); dash(); dash(); dot();
		}
		else if (s[i] == '0') {
			dash(); dash(); dash(); dash(); dash();
		}
		else if (s[i] == ' ') {
			space();
		}
		_delay_ms(400);
	}
}

int main(void)
{
	DDRB |= (1<<DDB5); //0x20 (hex) // Set port bit B5 in data direction register to 1: an OUTput
	while (1)  {
		morse();
	}
}
