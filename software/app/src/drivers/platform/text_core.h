#pragma once

#include "init.h"

/**********************************************************************
 * OSD Core
 *********************************************************************/
/**
 * osd (on-screen display) video core driver
 *
 * video subsystem HDL parameter:
 *  - CHROMA_KEY => CHROMA_KEY_COLOR = 0
 *
 */
class TextCore {
public:
	/**
	 * Register map
	 *
	 */
	enum {
		BYPASS_REG = 0x2000, /**< bypass control register */
		FG_CLR_REG = 0x2001, /**< foreground color register */
		BG_CLR_REG = 0x2002	 /**< background color register */
	};
	/**
	 * symbolic constants
	 *
	 */
	enum {
		CHROMA_KEY_COLOR = 0, // chroma key
		NULL_CHAR = 0x00,	  // signature for transparent char tile
		CHAR_X_MAX = 80,	  // 80 char per row
		CHAR_Y_MAX = 30		  // 30 char per column
	};
	/* methods */
	TextCore(uint32_t core_base_addr);
	~TextCore();
	// not used

	/**
	 * set foreground/background text display colors
	 * @param fg_color foreground text display color
	 * @param bg_color background text display color
	 *
	 */
	void set_color(uint32_t fg_color, uint32_t bg_color);

	/**
	 * write a char to tile RAM
	 * @param x x-coordinate of the tile (between 0 and CHAR_X_MAX)
	 * @param y y-coordinate of the tile (between 0 and CHAR_Y_MAX)
	 * @param ch char to be written
	 * @param reverse 0: normal display; 1: reversed display
	 *
	 * @note reversed display swaps the foreground/background colors
	 *
	 */
	void wr_char(uint8_t x, uint8_t y, char ch, int reverse = 0);

	void wr_str(uint8_t x, uint8_t y, int reverse, const char *str, ...);

	/**
	 * clear tile RAM (by writing NULL_CHAR to all tiles)
	 *
	 */
	void clr_screen();

	/**
	 * enable/disable core bypass
	 * @param by 1: bypass current core; 0: not bypass
	 *
	 */
	void bypass(int by);

private:
	uint32_t base_addr;
};
