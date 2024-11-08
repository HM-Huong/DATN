#pragma once

#include "io_map.h" // to use SYS_CLK_FREQ
#include "io_rw.h"
/**
 * uart core driver
 * - transmit/receive data via MMIO uart core.
 * - display (print) number and string on serial console
 *
 */
class UartCore {
	/**
	 * register map
	 *
	 */
	enum {
		RD_DATA_REG = 0,   /**< rx data/status register */
		DVSR_REG = 1,	   /**< baud rate divisor register */
		WR_DATA_REG = 2,   /**< wr data register */
		RM_RD_DATA_REG = 3 /**< remove read data offset */
	};
	/**
	 * mask fields
	 *
	 */
	enum {
		TX_FULL_FIELD = 0x00000200, /**< bit 9 of rd_data_reg; full bit  */
		RX_EMPT_FIELD = 0x00000100, /**< bit 10 of rd_data_reg; empty bit */
		RX_DATA_FIELD = 0x000000ff	/**< bits 7..0 rd_data_reg; read data */
	};

public:
	/* methods */
	/**
	 * constructor.
	 *
	 * @note set the default rate to 9600 baud
	 */
	UartCore(uint32_t core_base_addr);
	~UartCore();

	/**
	 * set baud rate
	 *
	 * @param baud baud rate
	 * @note baud rate = sys_clk_freq/16/(dvsr+1)
	 */
	void set_baud_rate(int baud);

	/**
	 * check whether uart receiver fifo is empty
	 *
	 * @return 1: if empty; 0: otherwise
	 *
	 */
	int rx_fifo_empty();

	/**
	 * check whether uart transmitter fifo is full
	 *
	 * @return 1: if full; 0: otherwise
	 *
	 */
	int tx_fifo_full();

	/**
	 * transmit a byte
	 *
	 * @param byte data byte to be transmitted
	 *
	 * @note the function "busy waits" if tx fifo is full;
	 *       to avoid "blocking" execution, use tx_fifo_full() to check status as needed
	 */
	void tx_byte(uint8_t byte);

	/**
	 * receive a byte
	 *
	 * @return -1 if rx fifo empty; byte data other wise
	 *
	 * @note the function does not "busy wait"
	 */
	int rx_byte();

	/**
	 * display (print) a char on a serial terminal console
	 *
	 * @param ch char to be displayed
	 *
	 */
	void send(char ch);

	/**
	 * display (print) a string on a serial terminal console
	 *
	 * @param str pointer to the string to be displayed
	 *
	 */
	void send(const char *str, ...);
private:
	uint32_t base_addr;
	int baud_rate;
};
