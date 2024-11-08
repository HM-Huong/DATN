/**********************************************************************
 * basic uart and timer functions
 *  - obtain BRIDGE_BASE from io_map.h

 *********************************************************************/

#pragma once

// library
#include "io_map.h"
#include "io_rw.h"
#include "timer_core.h"
#include "uart_core.h"

//  make uart visible by other code
extern UartCore terminal;

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_SLOT 0
#define UART_SLOT 1

/**
 * Current system "up time" in microsecond.
 */
unsigned long now_us();

/**
 * Current system "up time" in millisecond.
 */
unsigned long now_ms();

/**
 * idle for t microsecond.
 * @param t idle time
 */
void sleep_us(unsigned long int t);

/**
 * idle for t millisecond.
 * @param t idle time
 */
void sleep_ms(unsigned long int t);

#ifdef __cplusplus
} // extern "C"
#endif

/**********************************************************************
 * low-level bit-manipulation macros
 * @param n bit position
 *********************************************************************/
#define bit_set(data, n) ((data) |= (1UL << (n)))
#define bit_clear(data, n) ((data) &= ~(1UL << (n)))
#define bit_toggle(data, n) ((data) ^= (1UL << (n)))
#define bit_read(data, n) (((data) >> (n)) & 0x01)
#define bit_write(data, n, bitvalue) (bitvalue ? bit_set((data), n) : bit_clear((data), n))
#define bit(n) (1UL << (n))
