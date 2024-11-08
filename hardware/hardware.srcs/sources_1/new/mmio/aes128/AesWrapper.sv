module AesWrapper (
	input  logic        clk    ,
	input  logic        reset  ,
	// slot interface
	input  logic        cs     ,
	input  logic        read   ,
	input  logic        write  ,
	input  logic [ 4:0] addr   ,
	input  logic [31:0] wr_data,
	output logic [31:0] rd_data
);

// signal declaration
	logic [127:0] key         ;
	logic [127:0] iBlock      ;
	logic [127:0] oBlock      ;
	logic         loadKey     ;
	logic         start       ;
	logic         encrypt     ;
	logic         idle        ;
	logic         wr_en       ;
	logic         wr_ctrl     ;
	logic         wr_key_b0   ;
	logic         wr_key_b1   ;
	logic         wr_key_b2   ;
	logic         wr_key_b3   ;
	logic         wr_iBlock_b0;
	logic         wr_iBlock_b1;
	logic         wr_iBlock_b2;
	logic         wr_iBlock_b3;

// instantiate aes core
	AesCore AesCore (
		.clk    (clk    ),
		.rst    (reset  ),
		.load   (loadKey),
		.key    (key    ),
		.start  (start  ),
		.encrypt(encrypt),
		.iBlock (iBlock ),
		.oBlock (oBlock ),
		.idle   (idle   )
	);

// decoding
	always_comb begin
		wr_en   = cs && write;
		wr_ctrl = wr_en && (addr == 0);

		wr_key_b0 = wr_en && (addr == 1);
		wr_key_b1 = wr_en && (addr == 2);
		wr_key_b2 = wr_en && (addr == 3);
		wr_key_b3 = wr_en && (addr == 4);

		wr_iBlock_b0 = wr_en && (addr == 5);
		wr_iBlock_b1 = wr_en && (addr == 6);
		wr_iBlock_b2 = wr_en && (addr == 7);
		wr_iBlock_b3 = wr_en && (addr == 8);
	end

// registers
	always_ff @(posedge clk, posedge reset)
		begin
			if (reset) begin
				key    <= 128'h0;
				iBlock <= 128'h0;
				encrypt <= 0;
				start <= 0;
				loadKey <= 0;
			end else begin
				if (wr_ctrl) begin
					encrypt <= wr_data[0];
					start   <= wr_data[1];
					loadKey <= wr_data[2];
				end else begin
					start   <= 0;
					loadKey <= 0;
				end

				if (wr_key_b0)
					key[31:0] <= wr_data;
				if (wr_key_b1)
					key[63:32] <= wr_data;
				if (wr_key_b2)
					key[95:64] <= wr_data;
				if (wr_key_b3)
					key[127:96] <= wr_data;

				if (wr_iBlock_b0)
					iBlock[31:0] <= wr_data;
				if (wr_iBlock_b1)
					iBlock[63:32] <= wr_data;
				if (wr_iBlock_b2)
					iBlock[95:64] <= wr_data;
				if (wr_iBlock_b3)
					iBlock[127:96] <= wr_data;
			end
		end

	// read multiplexer
	always_comb
		begin
			case (addr)
				9       : rd_data = {31'h0, idle};
				10      : rd_data = oBlock[31:0];
				11      : rd_data = oBlock[63:32];
				12      : rd_data = oBlock[95:64];
				13      : rd_data = oBlock[127:96];
				default : rd_data = 32'h0;
			endcase
		end
endmodule
