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
void mode_entries(const uint8_t u2);
void do_entries(const int n, const uint32_t entries_time[], const uint8_t entries_direction[], const int8_t entries_degrees[]);
int8_t msec_to_degrees(const uint8_t dir, const int8_t from_deg, uint32_t msec);
uint32_t degrees_to_msec(const uint8_t dir, const int8_t cur_deg, const int8_t deg);

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

void mode_entries(const uint8_t u2)
{
	switch (u2) {
		case 0:
			do_entries(ENTRIES_EXAMPLE_N, entries_example_time, entries_example_direction, entries_example_degrees);
			break;
		default:
			panic("invalid u2 value");
	}
}

void do_entries(const int n, const uint32_t entries_time[], const uint8_t entries_direction[], const int8_t entries_degrees[])
{
	int c;
	int is_moving[2] = {0, 0};
	uint32_t timing_pitch[2], timing_start[2];
	uint8_t prev_dir_dir[2];
	int8_t prev_cur_degrees[2] = {0, 0}, prev_degrees[2] = {0, 0};

	for (c = 0; c < n; ) {
		int i;
		uint32_t m = millis();
		uint32_t et, s;
		uint8_t dir, dir_dir;
		int8_t deg, cur_degrees;

		for (i = 0; i <= 1; i ++) {
			if (is_moving[i]) {
				if (timing_start[i] + timing_pitch[i] <= m) {
					write_dn(i, 0, 0);
					is_moving[i] = 0;
				}
			}
		}

		et = read_entry_time(entries_time, c);
		dir = read_entry_time(entries_direction, c);
		deg = read_entry_degrees(entries_degrees, c);

		if (et >= m) {
			if (is_moving[dir]) {
				int8_t d_offset = (prev_dir_dir[dir] == 0 ? 1 : -1) * msec_to_degrees(dir, prev_cur_degrees[dir], m - timing_start[dir]);
				cur_degrees = prev_cur_degrees[dir] + d_offset;
			} else {
				cur_degrees = prev_cur_degrees[dir] + prev_degrees[dir];
				if (cur_degrees == deg) {
					c ++;
					continue;
				}
			}

			dir_dir = (cur_degrees < deg ? 0 : 1);
			s = degrees_to_msec(dir, cur_degrees, deg);

			timing_pitch[dir] = s;
			timing_start[dir] = m;
			is_moving[dir] = !0;
			c ++;
			write_dn(dir, 1, dir_dir);
			prev_cur_degrees[dir] = cur_degrees;
			prev_degrees[dir] = deg;
			prev_dir_dir[dir] = dir_dir;
		}
	}
}

int8_t msec_to_degrees(const uint8_t dir, const int8_t from_deg, uint32_t msec)
{
	int8_t ret;

	switch (dir) {
		case 0:
			ret = msec * 20;
			break;
		case 1:
			ret = msec * 20;
			break;
		default:
			panic("invalid direction value");
	}

	return ret;
}

uint32_t degrees_to_msec(const uint8_t dir, const int8_t cur_deg, const int8_t deg)
{
	uint32_t ret;

	switch (dir) {
		case 0:
			ret = deg / 20.0;
			break;
		case 1:
			ret = deg / 20.0;
			break;
		default:
			panic("invalid direction value");
	}

	return ret;
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
		case 3:
			mode_entries(u2);
			break;
		default:
			panic("invalid u1 value");
	}

	Serial.println("setup: finished");
}

void loop()
{
}
