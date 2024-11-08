`ifndef _IO_MAP_INCLUDED_
`define _IO_MAP_INCLUDED_

// system clock rate in MHz; used for timer, uart, ddfs etc
`define SYS_CLK_FREQ 100

//io base address for microBlaze MCS
`define BRIDGE_BASE 0xc0000000

// slot module definition
// format: SLOT`_ModuleType_Name
`define S0_SYS_TIMER    0
`define S1_UART1        1
`define S2_GPO          2
`define S2_GPI          3
`define S4_I2C          4
`define S5_SPI          5
`define S6_UART2        6
`define S7_AES128       7

// video module definition
`define V0_SYNC         0
`define V1_TEXT         1
`define V2_PADDLE       2
`define V3_BALL         3
`define V4_TEST_COLOR   4

// video frame buffer
`define FRAME_OFFSET 0x00c00000
`define FRAME_BASE   BRIDGE_BASE+FRAME_OFFSET

`endif //_IO_MAP_INCLUDED_
