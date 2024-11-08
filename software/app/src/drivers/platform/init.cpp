#include "init.h"

/**********************************************************************
 * basic uart and timer functions
 *  - define basic timing function
 *  - define the basic char stream serial port "uart"
 *  - obtain BRIDGE_BASE from io_map.h
 *  - time slot is 0
 *  - uart slot is 1
 *********************************************************************/

TimerCore _sys_timer(get_slot_addr(BRIDGE_BASE, TIMER_SLOT));
UartCore terminal(get_slot_addr(BRIDGE_BASE, UART_SLOT));

// current system time in microsecond
unsigned long now_us() {
	return ((unsigned long)_sys_timer.read_time());
}

// current system time in ms
unsigned long now_ms() {
	return ((unsigned long)_sys_timer.read_time() / 1000);
}

// idle for t microseconds
void sleep_us(unsigned long int t) {
	_sys_timer.sleep(uint64_t(t));
}

// idle for t ms
void sleep_ms(unsigned long int t) {
	_sys_timer.sleep(uint64_t(1000 * t));
}
