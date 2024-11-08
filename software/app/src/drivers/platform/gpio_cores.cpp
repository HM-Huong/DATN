#include "gpio_cores.h"

/**********************************************************************
 * GpiCore
 **********************************************************************/
GpiCore::GpiCore(uint32_t core_base_addr) {
	base_addr = core_base_addr;
}
GpiCore::~GpiCore() {
}

uint32_t GpiCore::read() {
	return (io_read(base_addr, DATA_REG));
}

int GpiCore::read(int bit_pos) {
	uint32_t rd_data = io_read(base_addr, DATA_REG);
	return ((int)bit_read(rd_data, bit_pos));
}

/**********************************************************************
 * GpoCore
 **********************************************************************/
GpoCore::GpoCore(uint32_t core_base_addr) {
	base_addr = core_base_addr;
	wr_data = 0;
}

GpoCore::~GpoCore() {
}

void GpoCore::write(uint32_t data) {
	wr_data = data;
	io_write(base_addr, DATA_REG, wr_data);
}

void GpoCore::write(int bit_value, int bit_pos) {
	bit_write(wr_data, bit_pos, bit_value);
	io_write(base_addr, DATA_REG, wr_data);
}
