#pragma once

#include "init.h"

class BarTestCore {
public:
	/**
	 * Register map
	 */
	enum {
		BYPASS_REG = 0x2000 /**< bypass control register */
	};
	/* methods */
	BarTestCore(uint32_t core_base_addr);
	~BarTestCore(); // not used

	void bypass(int by);
private:
	uint32_t base_addr;
};
