module sprite_ram #(parameter
	MEM_INIT_FILE = "", // memory file
	DATA_WIDTH    = 12, // color depth
	ADDR_WIDTH    = 10  // number of address bits
) (
	input  logic                  clk   ,
	input  logic                  we    ,
	input  logic [ADDR_WIDTH-1:0] addr_r,
	input  logic [ADDR_WIDTH-1:0] addr_w,
	input  logic [DATA_WIDTH-1:0] din   ,
	output logic [DATA_WIDTH-1:0] dout
);

	// declaration
	logic [DATA_WIDTH-1:0] ram     [0:2**ADDR_WIDTH-1];
	logic [DATA_WIDTH-1:0] data_reg                   ;

	// specifies the initial values of ram
	initial begin
		if (MEM_INIT_FILE == "") begin
			$error("In %s, line %d: %s", `__FILE__, `__LINE__, "The MEM_INIT_FILE parameter must be specified.");
		end else begin
			$readmemh(MEM_INIT_FILE, ram);
		end
	end

	// body
	always_ff @(posedge clk)
		begin
			if (we)
				ram[addr_w] <= din;
			data_reg <= ram[addr_r];
		end
	assign dout = data_reg;
endmodule

