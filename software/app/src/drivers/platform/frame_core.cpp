#include "frame_core.h"
#include <math.h>

/**********************************************************************
 * FrameCore core methods
 *********************************************************************/
FrameCore::FrameCore(uint32_t frame_base_addr) {
	base_addr = frame_base_addr;
}
FrameCore::~FrameCore() {
}
// not used

void FrameCore::wr_pix(int x, int y, int color) {
	uint32_t pix_offset;

	pix_offset = 640 * y + x;
	io_write(base_addr, pix_offset, color);
	return;
}

void FrameCore::clr_screen() {
	int x, y;

	for (x = 0; x < HMAX; x++)
		for (y = 0; y < VMAX; y++) {
			wr_pix(x, y, 0);
		}
	return;
}

void FrameCore::bypass(int by) {
	bypassed = by;
	io_write(base_addr, BYPASS_REG, uint32_t(fg_color << 13 | bg_color << 1 | bypassed));
}

void FrameCore::set_bg_color(uint16_t color) {
	bg_color = color;
	io_write(base_addr, BYPASS_REG, uint32_t(fg_color << 13 | bg_color << 1 | bypassed));
}

void FrameCore::set_fg_color(uint16_t color) {
	fg_color = color;
	io_write(base_addr, BYPASS_REG, uint32_t(fg_color << 13 | bg_color << 1 | bypassed));
}

// from AdaFruit
void FrameCore::plot_line(int x0, int y0, int x1, int y1, int color) {
	int dx, dy;
	int err, ystep, steep;

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	// slope is high
	steep = (abs(y1 - y0) > abs(x1 - x0)) ? 1 : 0;
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	dx = x1 - x0;
	dy = abs(y1 - y0);
	err = dx / 2;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	for (; x0 <= x1; x0++) {
		if (steep) {
			wr_pix(y0, x0, color);
		} else {
			wr_pix(x0, y0, color);
		}
		err = err - dy;
		if (err < 0) {
			y0 = y0 + ystep;
			err = err + dx;
		}
	}
}

void FrameCore::draw_rect(int x, int y, int w, int h, int color) {
	plot_line(x, y, x + w, y, color);
	plot_line(x, y, x, y + h, color);
	plot_line(x + w, y, x + w, y + h, color);
	plot_line(x, y + h, x + w, y + h, color);
}

void FrameCore::fill_rect(int x, int y, int w, int h, int color) {
	for (int i = 0; i < w; i++) {
		plot_line(x + i, y, x + i, y + h, color);
	}
}

void FrameCore::swap(int &a, int &b) {
	int tmp;

	tmp = a;
	a = b;
	b = tmp;
}
