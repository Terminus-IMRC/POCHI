#include <stdint.h>
#include "entries.h"

#define PIN_0 2
#define PIN_0_0 3
#define PIN_0_1 4
#define PIN_1 5
#define PIN_1_0 6
#define PIN_1_1 7
#define PIN_SW0 8
#define PIN_SW1 9
#define PIN_SW2 10
#define PIN_SW3 11
#define PIN_L0 12
#define PIN_L1 13
#define PIN_SW4 A0
#define PIN_SW5 A1
#define PIN_SW6 A2
#define PIN_SW7 A3

void _panic(const char *filename, const int line, const char *str);
void write_d(const int pin, const int pin_0, const int pin_1, const int v0, const int v1);
void write_d0(const int v0, const int v1);
void write_d1(const int v0, const int v1);
void write_dn(const int direction, const int v0, const int v1);
void mode_test(const uint8_t u2);

#define HIGH_or_LOW(v) ((((v)) ? HIGH : LOW))
#define panic(str) _panic(__FILE__, __LINE__, str)

void _panic(const char *filename, const int line, const char *str)
{
	Serial.println("*** Panic ***");
	Serial.print(filename);
	Serial.print(":");
	Serial.print(line);
	Serial.print(": ");
	Serial.println(str);

	for (; ; ) {
		digitalWrite(PIN_L0, HIGH);
		digitalWrite(PIN_L1, LOW);
		delay(200);
		digitalWrite(PIN_L0, LOW);
		digitalWrite(PIN_L1, HIGH);
		delay(200);
	}
}

void write_d(const int pin, const int pin_0, const int pin_1, const int v0, const int v1)
{
	digitalWrite(pin, HIGH_or_LOW(v0));
	digitalWrite(pin_0, HIGH_or_LOW(v1));
	digitalWrite(pin_1, HIGH_or_LOW(!v1));
}

void write_d0(const int v0, const int v1)
{
	write_d(PIN_0, PIN_0_0, PIN_0_1, v0, v1);
}

void write_d1(const int v0, const int v1)
{
	write_d(PIN_1, PIN_1_0, PIN_1_1, v0, v1);
}

void write_dn(const int direction, const int v0, const int v1)
{
	switch (direction) {
		case 0:
			write_d0(v0, v1);
			break;
		case 1:
			write_d1(v0, v1);
			break;
		default:
			panic("invalid direction value");
	}
}

void mode_test(const uint8_t u2)
{
	switch (u2) {
		case 0:
			for (; ; ) {
				write_d0(1, 0);
				delay(1000);
				write_d0(1, 1);
				delay(1000);
				write_d1(1, 0);
				delay(1000);
				write_d1(1, 1);
				delay(1000);
			}
			break;
		case 1:
			for (; ; ) {
				if (digitalRead(PIN_SW4) == LOW)
					write_d0(1, 0);
				else if (digitalRead(PIN_SW5) == LOW)
					write_d0(1, 1);
				else
					write_d0(0, 0);
				if (digitalRead(PIN_SW6) == LOW)
					write_d1(1, 0);
				else if (digitalRead(PIN_SW7) == LOW)
					write_d1(1, 1);
				else
					write_d1(0, 0);
			}
			break;
		default:
			panic("invalid u2 value");
	}
}

void setup()
{
	uint8_t u1, u2;

	Serial.begin(115200);
	while (!Serial)
		;
	Serial.println("setup: started");

	pinMode(PIN_0, OUTPUT);
	pinMode(PIN_0_0, OUTPUT);
	pinMode(PIN_0_1, OUTPUT);
	pinMode(PIN_1, OUTPUT);
	pinMode(PIN_1_0, OUTPUT);
	pinMode(PIN_1_1, OUTPUT);
	pinMode(PIN_SW0, INPUT_PULLUP);
	pinMode(PIN_SW1, INPUT_PULLUP);
	pinMode(PIN_SW2, INPUT_PULLUP);
	pinMode(PIN_SW3, INPUT_PULLUP);
	pinMode(PIN_L0, OUTPUT);
	pinMode(PIN_L1, OUTPUT);
	pinMode(PIN_SW4, INPUT_PULLUP);
	pinMode(PIN_SW5, INPUT_PULLUP);
	pinMode(PIN_SW6, INPUT_PULLUP);
	pinMode(PIN_SW7, INPUT_PULLUP);

	write_d0(0, 0);
	write_d1(0, 0);
	digitalWrite(PIN_L0, LOW);
	digitalWrite(PIN_L1, LOW);

	u1 = 0;
	if (digitalRead(PIN_SW0) == LOW)
		u1 |= 1 << 0;
	if (digitalRead(PIN_SW1) == LOW)
		u1 |= 1 << 1;

	u2 = 0;
	if (digitalRead(PIN_SW2) == LOW)
		u2 |= 1 << 0;
	if (digitalRead(PIN_SW3) == LOW)
		u2 |= 1 << 1;

	switch (u1) {
		case 0:
			panic("test");
			break;
		case 2:
			mode_test(u2);
			break;
#if 0
		case 3:
			mode_entries(u2);
			break;
#endif
		default:
			panic("invalid u1 value");
	}

	Serial.println("setup: finished");
}

void loop()
{
}
