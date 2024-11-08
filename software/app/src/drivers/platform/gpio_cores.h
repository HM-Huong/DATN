#pragma once

#include "init.h"

/**********************************************************************
 * gpi (general-purpose input) core driver
 **********************************************************************/
/**
 * gpi (general-purpose input) core driver
 *  - retrieve data from MMIO gpi core.
 *
 * MMIO subsystem HDL parameter:
 *  - W (not used in driver): # bits of input register
 *   (unused bits return 0's)
 */
class GpiCore {
public:
	/**
	 * register map
	 *
	 */
	enum {
		DATA_REG = 0 /**< input data register */
	};
	/**
	 * constructor.
	 *
	 */
	GpiCore(uint32_t core_base_addr);
	~GpiCore(); // not used

	/* methods */
	/**
	 * read a 32-bit word
	 * @return 32-bit read data word
	 * @note unused bits return 0's
	 */
	uint32_t read();

	/**
	 * read a bit at a specific position
	 *
	 * @param bit_pos bit position
	 * @return 1-bit read data
	 *
	 */
	int read(int bit_pos);

private:
	uint32_t base_addr;
};

/**********************************************************************
 * gpo (general-purpose output) core driver
 **********************************************************************/
/**
 * gpo (general-purpose output) core driver
 *  - write data to MMIO gpo core.
 *
 * MMIO subsystem HDL parameter:
 *  - W (not used in driver): # bits of output register
 *   (unused bits have no effect)
 */
class GpoCore {
public:
	/**
	 * register map
	 *
	 */
	enum {
		DATA_REG = 0 /**< output data register */
	};
	/**
	 * constructor.
	 *
	 */
	GpoCore(uint32_t core_base_addr);
	~GpoCore(); // not used

	/**
	 * write a 32-bit word
	 * @param data 32-bit data
	 *
	 */
	void write(uint32_t data);

	/**
	 * write a bit at a specific position
	 *
	 * @param bit_value value
	 * @param bit_pos bit position
	 *
	 */
	void write(int bit_value, int bit_pos);

private:
	uint32_t base_addr;
	uint32_t wr_data; // same as GPO core data reg
};
