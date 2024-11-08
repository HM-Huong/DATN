module sprite_wrapper #(parameter
	BITMAP_FILE = "", // memory file
	H_SIZE      = 32, // horizontal size of sprite
	V_SIZE      = 32, // vertical size of sprite
	CD          = 12, // color depth
	CHROMA_KEY   = 0
) (
	input  logic        clk    ,
	input  logic        reset  ,
	// frame counter
	input  logic [10:0] x      ,
	input  logic [10:0] y      ,
	// video slot interface
	input  logic        cs     ,
	input  logic        write  ,
	input  logic [13:0] addr   ,
	input  logic [31:0] wr_data,
	// stream interface
	input  logic [11:0] si_rgb ,
	output logic [11:0] so_rgb
);

	// localparam declaration
	localparam ADDR_WIDTH = $clog2(H_SIZE) + $clog2(V_SIZE);

	// delaration
	logic          wr_en       ;
	logic          wr_ram      ;
	logic          wr_reg      ;
	logic          wr_bypass   ;
	logic          wr_x0       ;
	logic          wr_y0       ;
	logic [CD-1:0] sprite_rgb   ;
	logic [CD-1:0] chrom_rgb   ;
	logic [  10:0] x0_reg      ;
	logic [  10:0] y0_reg      ;
	logic          bypass_reg  ;
	logic          is_rendering;

	// body
	// instantiate sprite generator
	sprite_core #(
		.BITMAP_FILE(BITMAP_FILE),
		.H_SIZE     (H_SIZE     ),
		.V_SIZE     (V_SIZE     ),
		.CD         (CD         ),
		.CHROMA_KEY  (CHROMA_KEY  ),
		.ADDR_WIDTH (ADDR_WIDTH )
	) sprite_unit (
		.clk         (clk                 ),
		.x           (x                   ),
		.y           (y                   ),
		.x0          (x0_reg              ),
		.y0          (y0_reg              ),
		.we          (wr_ram              ),
		.addr_w      (addr[ADDR_WIDTH-1:0]),
		.pixel_in    (wr_data[CD-1:0]     ),
		.is_rendering(is_rendering        ),
		.sprite_rgb   (sprite_rgb           )
	);

	// register
	always_ff @(posedge clk, posedge reset)
		if (reset) begin
			x0_reg     <= 0;
			y0_reg     <= 0;
			bypass_reg <= 0;
		end
		else begin
			if (wr_x0 && !is_rendering)
				x0_reg <= wr_data[10:0];
			if (wr_y0 && !is_rendering)
				y0_reg <= wr_data[10:0];
			if (wr_bypass)
				bypass_reg <= wr_data[0];
		end
	// decoding
	assign wr_en     = write & cs;
	assign wr_ram    = ~addr[13] && wr_en;
	assign wr_reg    = addr[13] && wr_en;
	assign wr_bypass = wr_reg && (addr[1:0]==2'b00);
	assign wr_x0     = wr_reg && (addr[1:0]==2'b01);
	assign wr_y0     = wr_reg && (addr[1:0]==2'b10);
	// chrome-key blending and multiplexing
	assign chrom_rgb = (sprite_rgb != CHROMA_KEY) ? sprite_rgb : si_rgb;
	assign so_rgb    = (bypass_reg) ? si_rgb : chrom_rgb;
endmodule

