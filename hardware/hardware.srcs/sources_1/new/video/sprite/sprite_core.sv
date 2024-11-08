module sprite_core #(parameter
	BITMAP_FILE = ""                             , // memory file
	H_SIZE      = 32                             , // horizontal size of sprite, must be power of 2
	V_SIZE      = 32                             , // vertical size of sprite, must be power of 2
	CD          = 12                             , // color depth
	CHROMA_KEY  = 0                              , // chroma key
	ADDR_WIDTH  = $clog2(H_SIZE) + $clog2(V_SIZE)  // number of address bits
) (
	input  logic                  clk         ,
	// x-and  y-coordinate
	input  logic [          10:0] x           ,
	input  logic [          10:0] y           ,
	// origin of sprite
	input  logic [          10:0] x0          ,
	input  logic [          10:0] y0          ,
	// sprite ram write
	input  logic                  we          ,
	input  logic [ADDR_WIDTH-1:0] addr_w      ,
	input  logic [        CD-1:0] pixel_in    ,
	output logic                  is_rendering,
	// pixel output
	output logic [        CD-1:0] sprite_rgb
);

	// localparam declaration
	localparam XR_WIDTH = $clog2(H_SIZE);
	localparam YR_WIDTH = $clog2(V_SIZE);

	// signal delaration
	// relative x/y position
	logic signed [          11:0] xr            ;
	logic signed [          11:0] yr            ;
	logic                         in_region     ;
	logic        [ADDR_WIDTH-1:0] addr_r        ;
	logic        [        CD-1:0] full_rgb      ;
	logic        [        CD-1:0] out_rgb       ;
	logic        [        CD-1:0] out_rgb_d1_reg;

	// body
	// instantiate sprite RAM
	sprite_ram #(
		.MEM_INIT_FILE(BITMAP_FILE),
		.ADDR_WIDTH   (ADDR_WIDTH ),
		.DATA_WIDTH   (CD         )
	) ram_unit (
		.clk   (clk     ),
		.we    (we      ),
		.addr_w(addr_w  ),
		.din   (pixel_in),
		.addr_r(addr_r  ),
		.dout  (full_rgb)
	);
	// relative coordinate calculation
	assign xr     = $signed({1'b0, x}) - $signed({1'b0, x0});
	assign yr     = $signed({1'b0, y}) - $signed({1'b0, y0});
	assign addr_r = {yr[YR_WIDTH-1:0], xr[XR_WIDTH-1:0]};
	// in-region comparison and multiplexing
	assign is_rendering = (0<=yr) && (yr<V_SIZE);
	assign in_region    = (0<=xr) && (xr<H_SIZE) && is_rendering;
	assign out_rgb      = in_region ? full_rgb : CHROMA_KEY;
	// rendering flag
	// output with a-stage delay line
	always_ff @(posedge clk)
		out_rgb_d1_reg <= out_rgb;
	assign sprite_rgb = out_rgb_d1_reg;
endmodule
