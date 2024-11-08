#define _DEBUG
#include "AES128.h"
#include "init.h"

AES128::AES128(uint32_t core_base_addr) {
	base_addr = core_base_addr;
	io_write(base_addr, CTRL_REG, 0);
}

AES128::~AES128() {
}

void AES128::set_key(const uint32_t key[4]) {
	for (int i = 0; i < 4; i++) {
		io_write(base_addr, KEY_BASE_REG + i, key[i]);
	}

	// start key expansion
	io_write(base_addr, CTRL_REG, LOAD_KEY_FIELD);
	while (!ready())
		;
}

void AES128::set_key(const uint8_t key[16]) {
	uint32_t key32[4];
	for (int i = 0; i < 4; i++) {
		key32[3 - i] = key[i * 4] << 24 | key[i * 4 + 1] << 16 | key[i * 4 + 2] << 8 | key[i * 4 + 3];
	}
	set_key(key32);
}

void AES128::encrypt(uint32_t data[4]) {
	for (int i = 0; i < 4; i++) {
		io_write(base_addr, I_BLOCK_BASE_REG + i, data[i]);
	}
	io_write(base_addr, CTRL_REG, ENC_FIELD | START_FIELD);

	while (!ready())
		;

	for (int i = 0; i < 4; i++) {
		data[i] = io_read(base_addr, O_BLOCK_BASE_REG + i);
	}
}

void AES128::encrypt(uint8_t data[16]) {
	uint32_t data32[4];
	for (int i = 0; i < 4; i++) {
		data32[3 - i] = data[i * 4] << 24 | data[i * 4 + 1] << 16 | data[i * 4 + 2] << 8 | data[i * 4 + 3];
	}

	encrypt(data32);

	for (int i = 0; i < 4; i++) {
		data[i * 4] = (data32[3 - i] >> 24) & 0xff;
		data[i * 4 + 1] = (data32[3 - i] >> 16) & 0xff;
		data[i * 4 + 2] = (data32[3 - i] >> 8) & 0xff;
		data[i * 4 + 3] = (data32[3 - i]) & 0xff;
	}
}

void AES128::decrypt(uint32_t data[4]) {
	for (int i = 0; i < 4; i++) {
		io_write(base_addr, I_BLOCK_BASE_REG + i, data[i]);
	}
	io_write(base_addr, CTRL_REG, START_FIELD);

	while (!ready())
		;

	for (int i = 0; i < 4; i++) {
		data[i] = io_read(base_addr, O_BLOCK_BASE_REG + i);
	}
}

void AES128::decrypt(uint8_t data[16]) {
	uint32_t data32[4];
	for (int i = 0; i < 4; i++) {
		data32[3 - i] = data[i * 4] << 24 | data[i * 4 + 1] << 16 | data[i * 4 + 2] << 8 | data[i * 4 + 3];
	}

	decrypt(data32);

	for (int i = 0; i < 4; i++) {
		data[i * 4] = (data32[3 - i] >> 24) & 0xff;
		data[i * 4 + 1] = (data32[3 - i] >> 16) & 0xff;
		data[i * 4 + 2] = (data32[3 - i] >> 8) & 0xff;
		data[i * 4 + 3] = (data32[3 - i]) & 0xff;
	}
}

bool AES128::ready() {
	uint32_t status = io_read(base_addr, STATUS_REG);
	return (status & IDLE_FIELD);
}

void display_hex(uint8_t* data, int len) {
	for (int i = 0; i < len; i++) {
		terminal.send("%02x ", data[i]);
	}
	terminal.send("\n");
}