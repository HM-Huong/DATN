#pragma once

#include "init.h"

class AES128 {
	/**
	 * Register map
	 */
	enum {
		CTRL_REG = 0,
		KEY_BASE_REG = 1,
		I_BLOCK_BASE_REG = 5,
		STATUS_REG = 9,
		O_BLOCK_BASE_REG = 10,
	};

	/**
	 * Field masks
	 */
	enum {
		// control register
		ENC_FIELD = 0x1,
		START_FIELD = 0x2,
		LOAD_KEY_FIELD = 0x4,

		// status register
		IDLE_FIELD = 0x1,
	};
	

public:
	AES128(uint32_t core_base_addr);
	~AES128();

	void set_key(const uint32_t key[4]);
	void set_key(const uint8_t key[16]);
	void encrypt(uint32_t data[4]);
	void encrypt(uint8_t data[16]);
	void decrypt(uint32_t data[4]);
	void decrypt(uint8_t data[16]);
	bool ready();

private:
	uint32_t base_addr;
};

void display_hex(uint8_t* data, int len);