#pragma once

#include "init.h"

/**********************************************************************
 * Frame Core
 *********************************************************************/
/**
 * frame buffer core driver
 *
 */
class FrameCore {
public:
	/**
	 * Register map
	 *
	 */
	enum {
		BYPASS_REG = 0xfffff /**< bypass control register */
	};
	/**
	 * Symbolic constants for frame buffer size
	 *
	 */
	enum {
		HMAX = 640, /**< 640 pixels per row */
		VMAX = 480	/**< 480 pixels per row */
	};
	/* methods */
	FrameCore(uint32_t core_base_addr);
	~FrameCore(); // not used

	/**
	 * write a pxiel to frame buffer
	 * @param x x-coordinate of the pxixel (between 0 and HMAX)
	 * @param y y-coordinate of the pxixel (between 0 and VMAX)
	 * @param color pixel color
	 *
	 */
	void wr_pix(int x, int y, int color);

	/**
	 * clear frame buffer (fill the frame with a specific color)
	 * @param color color to fill the frame
	 *
	 */
	void clr_screen();

	/**
	 * generate pixels for a line in frame buffer (plot a line)
	 * @param x1 x-coordinate of starting point
	 * @param y1 y-coordinate of starting point
	 * @param x2 x-coordinate of ending point
	 * @param y2 y-coordinate of ending point
	 * @param color line color
	 *
	 */
	void plot_line(int x1, int y1, int x2, int y2, int color);

	void draw_rect(int x, int y, int w, int h, int color);
	void fill_rect(int x, int y, int w, int h, int color);

	/**
	 * enable/disable core bypass
	 * @param by 1: bypass current core; 0: not bypass
	 *
	 */
	void bypass(int by);

	void set_bg_color(uint16_t color);
	void set_fg_color(uint16_t color);

private:
	uint16_t bg_color, fg_color;
	bool bypassed;
	uint32_t base_addr;
	void swap(int &a, int &b);
};
