#include "sprite_core.h"

SpriteCore::SpriteCore() {
}
SpriteCore::SpriteCore(uint32_t core_base_addr, int sprite_size) {
	base_addr = core_base_addr;
	size = sprite_size;
}
SpriteCore::~SpriteCore() {
}

void SpriteCore::wr_mem(int addr, uint32_t color) {
	io_write(base_addr, addr, color);
}

void SpriteCore::bypass(int by) {
	io_write(base_addr, BYPASS_REG, (uint32_t)by);
}

void SpriteCore::move_xy(int x, int y) {
	io_write(base_addr, X_REG, x);
	io_write(base_addr, Y_REG, y);
}
