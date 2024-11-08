#pragma once

#include "../platform/i2c_core.h"
#include <inttypes.h>

extern I2cCore i2c;

void lcd_init(uint8_t lcd_Addr);
void lcd_send_command(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_printf(const char *s, ...);
void lcd_gotoxy(uint8_t x, uint8_t y);
void lcd_clear();
void lcd_clear_line(uint8_t line);
