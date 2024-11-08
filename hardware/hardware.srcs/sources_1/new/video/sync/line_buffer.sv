//======================================================================
// Description: line buffer for pixel data stream
// Design:
//   * to accomodate xilinx bram fifo buffer, the size is fixed at 1024
//   * use almost_full rather than full to provide some cushion in
//     processinf pipeline (e.g., some pixels may be partially processed)
// Note:
//   * the size of buffer is about a horizontal line to take advantage of
//     extra cusshion time in horizontal sync retrace interval
//======================================================================
module line_buffer #(parameter CD = 12 // color depth
) (
	input  logic        clk_stream_in ,
	input  logic        clk_stream_out,
	input  logic        reset         ,
	// stream in (sink)
	input  logic [CD:0] si_data       , // color+start
	input  logic        si_valid      ,
	output logic        si_ready      ,
	// stream out (source)
	output logic [CD:0] so_data       , // color+start
	output logic        so_valid      ,
	input  logic        so_ready
);

	// constant declaration
	localparam DW = CD + 1; // data width=colors+start
	// signal delaration
	logic       almost_full;
	logic       empty      ;

	// xilinx macro instantiation
	FIFO_DUALCLOCK_MACRO #(
		.ALMOST_EMPTY_OFFSET    (9'h080   ), // Sets the almost empty threshold
		.ALMOST_FULL_OFFSET     (9'h080   ), // Sets almost full threshold
		.DATA_WIDTH             (DW       ), // 1-72 for 18Kb / 37-72 for 36Kb
		.DEVICE                 ("7SERIES"), // Target device: "7SERIES"
		.FIFO_SIZE              ("18Kb"   ), // Target BRAM: "18Kb" or "36Kb"
		.FIRST_WORD_FALL_THROUGH("TRUE"   )
	) bram_fifo_unit (
		.RST        (reset         ),
		// read port
		.RDCLK      (clk_stream_out), // read clock
		.DO         (so_data       ), // read data out
		.RDEN       (so_ready      ), // remove word from head
		.EMPTY      (empty         ), // fifo empty
		.ALMOSTEMPTY(              ),
		.RDCOUNT    (              ),
		.RDERR      (              ), // read error
		// write port
		.WRCLK      (clk_stream_in ), // write clock
		.DI         (si_data       ), // write data in
		.WREN       (si_valid      ), // write enable
		.FULL       (              ), // fifo full
		.ALMOSTFULL (almost_full   ),
		.WRCOUNT    (              ),
		.WRERR      (              )  // write error
	);

	assign si_ready    = ~almost_full;
	assign so_valid    = ~empty;
endmodule
