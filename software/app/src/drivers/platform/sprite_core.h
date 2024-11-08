#pragma once

#include "init.h"

/**********************************************************************
 * Sprite Core
 *********************************************************************/
/**
 * sprite video core driver
 *
 * video subsystem HDL parameter:
 *  - CHROMA_KEY => CHROMA_KEY = 0
 *
 */
class SpriteCore {
public:
	/**
	 * Register map
	 *
	 */
	enum {
		BYPASS_REG = 0x2000, /**< bypass control register */
		X_REG = 0x2001,		 /**< x-axis of sprite orgin */
		Y_REG = 0x2002,		 /**< y-axis of sprite orgin */
	};
	/**
	 * symbolic constant
	 *
	 */
	enum {
		CHROMA_KEY = 0, /**< chroma-key color */
	};
	/* methods */
	SpriteCore();
	SpriteCore(uint32_t core_base_addr, int size);
	~SpriteCore(); // not used

	/**
	 * write a 32-bit word to memory module/registers of a video core
	 * @param addr offset adddress within core
	 * @param color data to be written
	 *
	 */
	void wr_mem(int addr, uint32_t color);

	/**
	 * move sprite to a location
	 * @param x x-coordinate of sprite origin
	 * @param y y-coordinate of sprite origin
	 *
	 * @note origin is the top-left conner of sprite
	 */
	void move_xy(int x, int y);

	/**
	 * enable/disable core bypass
	 * @param by 1: bypass current core; 0: not bypass
	 * @note type of command depends on each individual sprite core
	 */
	void bypass(int by);

private:
	uint32_t base_addr;
	int size; // sprite memory size
};
