module frame_buffer_wrapper #(parameter
	CD = 12, // color depth
	DW = 9   // frame buffer RAM data width
) (
	input  logic          clk, reset,
	// frame counter
	input  logic [  10:0] x, y   ,
	// video slot interface
	input  logic          cs     ,
	input  logic          write  ,
	input  logic [  19:0] addr   ,
	input  logic [  31:0] wr_data,
	// stream interface
	input  logic [CD-1:0] si_rgb ,
	output logic [CD-1:0] so_rgb
);

	// delaration
	logic          wr_en     ;
	logic          wr_pix    ;
	logic          wr_bypass ;
	logic [CD-1:0] osd_rgb   ;
	logic [CD-1:0] fg_reg    ;
	logic [CD-1:0] bg_reg    ;
	logic [CD-1:0] frame_rgb ;
	logic          bypass_reg;

	// body
	// instantiate osd generator
	frame_buffer_core #(.CD(CD)) frame_src_unit (
		.clk        (clk            ),
		.x          (x              ),
		.y          (y              ),
		.fg_color   (fg_reg         ),
		.bg_color   (bg_reg         ),
		.addr_pix   (addr[18:0]     ),
		.wr_data_pix(wr_data[DW-1:0]),
		.write_pix  (wr_pix         ),
		.frame_rgb  (frame_rgb      )
	);
	// register
	always_ff @(posedge clk, posedge reset)
		if (reset) begin
			bypass_reg <= 0;
			fg_reg     <= 0;
			bg_reg     <= 0;
		end else if (wr_bypass) begin
			bypass_reg <= wr_data[0];
			bg_reg     <= wr_data[CD:1];
			fg_reg     <= wr_data[CD*2:CD+1];
		end
	// decoding
	assign wr_en     = write & cs;
	assign wr_bypass = wr_en && addr==20'hfffff;
	assign wr_pix    = wr_en && addr!=20'hfffff;
	// stream blending: mux
	assign so_rgb = bypass_reg ? si_rgb : frame_rgb;
endmodule
