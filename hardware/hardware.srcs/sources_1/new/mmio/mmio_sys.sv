`include "io_map.svh"

module mmio_sys
	#(
		parameter N_SW = 4,
		parameter N_LED = 4
	)
	(
		input logic clk,
		input  logic reset,
		// FPro bus
		input  logic mmio_cs,
		input  logic mmio_wr,
		input  logic mmio_rd,
		input  logic [20:0] mmio_addr, // 11 LSB used; 2^6 slots; 2^5 reg each
		input  logic [31:0] mmio_wr_data,
		output logic [31:0] mmio_rd_data,
		// switches and LEDs
		input logic [N_SW-1:0] sw,
		input logic [N_SW-1:0] btn,
		output logic [N_LED-1:0] led,
		// uart
		input logic rx,
		output logic tx,

		// spi
		output logic spi_sclk,
		output logic spi_mosi,
		input  logic spi_miso,
		output logic spi_ss_n,
		// i2c
		output logic i2c_scl,
		inout  tri i2c_sda
	);

// declaration
logic [63:0] mem_rd_array        ;
logic [63:0] mem_wr_array        ;
logic [63:0] cs_array            ;
logic [ 4:0] reg_addr_array[63:0];
logic [31:0] rd_data_array [63:0];
logic [31:0] wr_data_array [63:0];

// body
// instantiate mmio controller
mmio_controller ctrl_unit (
	.clk                (clk           ),
	.reset              (reset         ),
	.mmio_cs            (mmio_cs       ),
	.mmio_wr            (mmio_wr       ),
	.mmio_rd            (mmio_rd       ),
	.mmio_addr          (mmio_addr     ),
	.mmio_wr_data       (mmio_wr_data  ),
	.mmio_rd_data       (mmio_rd_data  ),
	// slot interface
	.slot_cs_array      (cs_array      ),
	.slot_mem_rd_array  (mem_rd_array  ),
	.slot_mem_wr_array  (mem_wr_array  ),
	.slot_reg_addr_array(reg_addr_array),
	.slot_rd_data_array (rd_data_array ),
	.slot_wr_data_array (wr_data_array )
);

// slot 0: system timer
timer timer_slot0 (
	.clk    (clk                          ),
	.reset  (reset                        ),
	.cs     (cs_array[`S0_SYS_TIMER]      ),
	.read   (mem_rd_array[`S0_SYS_TIMER]  ),
	.write  (mem_wr_array[`S0_SYS_TIMER]  ),
	.addr   (reg_addr_array[`S0_SYS_TIMER]),
	.rd_data(rd_data_array[`S0_SYS_TIMER] ),
	.wr_data(wr_data_array[`S0_SYS_TIMER] )
);

// slot 1: UART
uart_wrapper uart_slot1 (
	.clk    (clk                      ),
	.reset  (reset                    ),
	.cs     (cs_array[`S1_UART1]      ),
	.read   (mem_rd_array[`S1_UART1]  ),
	.write  (mem_wr_array[`S1_UART1]  ),
	.addr   (reg_addr_array[`S1_UART1]),
	.rd_data(rd_data_array[`S1_UART1] ),
	.wr_data(wr_data_array[`S1_UART1] ),
	.tx     (tx                       ),
	.rx     (rx                       )
);
//assign rd_data_array[1] = 32'h00000000;

// slot 2: gpo
gpo #(.W(N_LED)) gpo_slot2 (
	.clk    (clk                    ),
	.reset  (reset                  ),
	.cs     (cs_array[`S2_GPO]      ),
	.read   (mem_rd_array[`S2_GPO]  ),
	.write  (mem_wr_array[`S2_GPO]  ),
	.addr   (reg_addr_array[`S2_GPO]),
	.rd_data(rd_data_array[`S2_GPO] ),
	.wr_data(wr_data_array[`S2_GPO] ),
	.dout   (led                    )
);

// slot 3: gpi
gpi #(.W(8)) gpi_slot3 (
	.clk    (clk                    ),
	.reset  (reset                  ),
	.cs     (cs_array[`S2_GPI]      ),
	.read   (mem_rd_array[`S2_GPI]  ),
	.write  (mem_wr_array[`S2_GPI]  ),
	.addr   (reg_addr_array[`S2_GPI]),
	.rd_data(rd_data_array[`S2_GPI] ),
	.wr_data(wr_data_array[`S2_GPI] ),
	.din    ({sw, btn}              )
);

//// slot 4: i2c
i2c_wrapper i2c_slot4 (
	.clk    (clk                    ),
	.reset  (reset                  ),
	.cs     (cs_array[`S4_I2C]      ),
	.read   (mem_rd_array[`S4_I2C]  ),
	.write  (mem_wr_array[`S4_I2C]  ),
	.addr   (reg_addr_array[`S4_I2C]),
	.rd_data(rd_data_array[`S4_I2C] ),
	.wr_data(wr_data_array[`S4_I2C] ),
	.scl    (i2c_scl                ),
	.sda    (i2c_sda                )
);

endmodule
