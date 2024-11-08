#include "text_core.h"

#include <stdarg.h>
#include <stdio.h>

/**********************************************************************
 * OSD core methods
 *********************************************************************/
TextCore::TextCore(uint32_t core_base_addr) {
	base_addr = core_base_addr;
	set_color(0x0f0, CHROMA_KEY_COLOR); // green on black
}
TextCore::~TextCore() {
}
// not used

void TextCore::set_color(uint32_t fg_color, uint32_t bg_color) {
	io_write(base_addr, FG_CLR_REG, fg_color);
	io_write(base_addr, BG_CLR_REG, bg_color);
}

void TextCore::wr_char(uint8_t x, uint8_t y, char ch, int reverse) {
	uint32_t ch_offset;
	uint32_t data;

	ch_offset = (y << 7) + (x & 0x07f); // offset is concatenation of y and x
	if (reverse == 1)
		data = (uint32_t)(ch | 0x80);
	else
		data = (uint32_t)ch;
	io_write(base_addr, ch_offset, data);
	return;
}

void TextCore::wr_str(uint8_t x, uint8_t y, int reverse, const char *str, ...) {
	va_list args;
	char buffer[81];
	va_start(args, str);
	vsprintf(buffer, str, args);
	va_end(args);
	for (int i = 0; i < 81 && buffer[i]; i++) {
		wr_char(x + i, y, buffer[i], reverse);
	}
}

void TextCore::clr_screen() {
	int x, y;

	for (x = 0; x < CHAR_X_MAX; x++)
		for (y = 0; y < CHAR_Y_MAX; y++) {
			wr_char(x, y, NULL_CHAR);
		}
	return;
}

void TextCore::bypass(int by) {
	io_write(base_addr, BYPASS_REG, (uint32_t)by);
}
