#include "../platform/init.h"
#include "LCD_IC2.h"

#include <stdarg.h>
#include <stdio.h>

I2cCore i2c(get_slot_addr(BRIDGE_BASE, S4_I2C));
static uint8_t address;

void lcd_init(uint8_t lcd_Addr) {
	address = lcd_Addr;
	i2c.set_freq(30000);
	lcd_send_command(0x02); // 4 bit mode
	lcd_send_command(0x28); // 2 line, 5x7 matrix
	lcd_send_command(0x0c); // Display on, cursor off
	lcd_send_command(0x06); // Increment cursor (shift cursor to right)
	lcd_send_command(0x01); // Clear display screen
}

void lcd_send_command(uint8_t cmd) {
	const uint8_t upper = cmd & 0xF0;
	const uint8_t lower = (cmd << 4) & 0xF0;

	const uint8_t en = 0x0c;  // BL EN RW RS = 1100
	const uint8_t _en = 0x08; // BL EN RW RS = 1000
	uint8_t packet[] = {
		upper | en,
		upper | _en,
		lower | en,
		lower | _en
	};

	for (int i = 0; i < sizeof(packet) / sizeof(packet[0]); i++) {
		i2c.write_transaction(address, &packet[i], 1, 0);
		sleep_us(100);
	}
}

void lcd_send_data(uint8_t data) {
	const uint8_t upper = data & 0xF0;
	const uint8_t lower = (data << 4) & 0xF0;

	const uint8_t en = 0x0d;  // BL EN RW RS = 1101
	const uint8_t _en = 0x09; // BL EN RW RS = 1001

	uint8_t packet[] = {
		upper | en,
		upper | _en,
		lower | en,
		lower | _en
	};

	for (int i = 0; i < sizeof(packet) / sizeof(packet[0]); i++) {
		i2c.write_transaction(address, &packet[i], 1, 0);
		sleep_us(100);
	}
}

void lcd_printf(const char *s, ...) {
	va_list args;
	va_start(args, s);
	char buffer[128];
	vsnprintf(buffer, sizeof(buffer), s, args);
	va_end(args);

	for (int i = 0; buffer[i]; i++) {
		lcd_send_data(buffer[i]);
	}
}

void lcd_gotoxy(uint8_t x, uint8_t y) {
	const uint8_t firstCharAdr[] = { 0x80, 0xC0 };
	lcd_send_command(firstCharAdr[y] + x);
}

void lcd_clear() {
	lcd_send_command(0x01);
}

void lcd_clear_line(uint8_t line) {
	lcd_gotoxy(0, line);
	for (int i = 0; i < 16; i++) {
		lcd_send_data(' ');
	}
	lcd_gotoxy(0, line);
}
