module frame_buffer_palette (
	input  logic        color_in ,
	input  logic [11:0] fg_color ,
	input  logic [11:0] bg_color ,
	output logic [11:0] color_out
);
	// logic [3:0] r_out;
	// logic [3:0] g_out;
	// logic [3:0] b_out;

	// body
	// assign r_out     = {color_in, color_in, color_in};
	// assign g_out     = 0;//{color_in, color_in, color_in};
	// assign b_out     = 0;//{color_in, color_in, color_in};
	assign color_out = color_in ? fg_color : bg_color;
endmodule

