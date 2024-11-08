module ila_io (
	input  logic        clk       ,
	input  logic        reset_n   ,
	input  logic [13:0] arduino_io,
	output logic [ 3:0] led
);
	logic reset;
	assign reset = !reset_n;

	(* mark_debug = "true" *)	logic io0;
	(* mark_debug = "true" *)	logic io1;
	(* mark_debug = "true" *)	logic io2;
	(* mark_debug = "true" *)	logic io3;
	(* mark_debug = "true" *)	logic io4;
	(* mark_debug = "true" *)	logic io5;
	(* mark_debug = "true" *)	logic io6;
	(* mark_debug = "true" *)	logic io7;
	(* mark_debug = "true" *)	logic io8;
	(* mark_debug = "true" *)	logic io9;
	(* mark_debug = "true" *)	logic io10;
	(* mark_debug = "true" *)	logic io11;
	(* mark_debug = "true" *)	logic io12;
	(* mark_debug = "true" *)	logic io13;

	assign led[0] = io0 | io4 | io8 | io12;
	assign led[1] = io1 | io5 | io9 | io13;
	assign led[2] = io2 | io6 | io10;
	assign led[3] = io3 | io7 | io11;

	always_ff @(posedge clk, posedge reset)
		if (reset)
			{io13, io12, io11, io10, io9, io8, io7, io6, io5, io4, io3, io2, io1, io0} <= 14'b0;
		else
			{io13, io12, io11, io10, io9, io8, io7, io6, io5, io4, io3, io2, io1, io0} <= arduino_io;

endmodule