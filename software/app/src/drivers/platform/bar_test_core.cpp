#include "bar_test_core.h"

BarTestCore::BarTestCore(uint32_t core_base_addr) {
	base_addr = core_base_addr;
}
BarTestCore::~BarTestCore() {
}

void BarTestCore::bypass(int by) {
	io_write(base_addr, BYPASS_REG, (uint32_t)by);
}
