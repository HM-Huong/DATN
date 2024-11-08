`include "io_map.svh"
module video_sys
	#(
		parameter CD = 12,            // color depth
		parameter VRAM_DATA_WIDTH = 1 //frame buffer data width
	)
	(
		input logic clk_sys,
		input logic clk_25M,
		input logic reset_sys,
		// FPro bus
		input logic video_cs,
		input logic video_wr,
		input logic [20:0] video_addr,
		input logic [31:0] video_wr_data,
		// to vga monitor
		output logic vsync,
		output logic hsync,
		output logic [11:0] rgb
	);

// costant declaration
localparam CHROMA_KEY = 0;
// signal declaration
logic [CD:0] line_data_in;
// frame counter
logic        inc        ;
logic        frame_start;
logic [10:0] x          ;
logic [10:0] y          ;
// delay line
logic frame_start_d1_reg;
logic frame_start_d2_reg;
logic inc_d1_reg        ;
logic inc_d2_reg        ;
// frame interface
logic        frame_wr     ;
logic        frame_cs     ;
logic [19:0] frame_addr   ;
logic [31:0] frame_wr_data;
// video core slot interface
logic [ 7:0] slot_cs_array           ;
logic [ 7:0] slot_mem_wr_array       ;
logic [13:0] slot_reg_addr_array[7:0];
logic [31:0] slot_wr_data_array [7:0];

// 2-stage delay line for start signal
always_ff @(posedge clk_sys) begin
	frame_start_d1_reg <= frame_start;
	frame_start_d2_reg <= frame_start_d1_reg;
	inc_d1_reg         <= inc;
	inc_d2_reg         <= inc_d1_reg;
end

// instantiate frame counter
frame_counter #(.HMAX(640), .VMAX(480)) frame_counter_unit (
	.clk        (clk_sys    ),
	.reset      (reset_sys  ),
	.inc        (inc        ),
	.hcount     (x          ),
	.vcount     (y          ),
	.frame_start(frame_start),
	.frame_end  (           )
);
// instantiate video decoding circuit
video_controller ctrl_unit (
	.clk                (clk                ),
	.reset              (reset              ),
	.video_cs           (video_cs           ),
	.video_wr           (video_wr           ),
	.video_addr         (video_addr         ),
	.video_wr_data      (video_wr_data      ),
	.frame_cs           (frame_cs           ),
	.frame_wr           (frame_wr           ),
	.frame_addr         (frame_addr         ),
	.frame_wr_data      (frame_wr_data      ),
	.slot_cs_array      (slot_cs_array      ),
	.slot_mem_wr_array  (slot_mem_wr_array  ),
	.slot_reg_addr_array(slot_reg_addr_array),
	.slot_wr_data_array (slot_wr_data_array )
);

// instantiate frame buffer
logic [CD-1:0] frame_rgb;
frame_buffer_wrapper #(.CD(CD), .DW(VRAM_DATA_WIDTH)) buf_unit (
	.clk    (clk_sys      ),
	.reset  (reset_sys    ),
	.x      (x            ),
	.y      (y            ),
	.cs     (frame_cs     ),
	.write  (frame_wr     ),
	.addr   (frame_addr   ),
	.wr_data(video_wr_data),
	.si_rgb (12'h222      ), // gray screen
	.so_rgb (frame_rgb    )
);

// instantiate bar generator
logic [CD-1:0] bar_rgb;
vga_bar_core #(.CD(CD)) v4_test_color_unit (
	.clk    (clk_sys                            ),
	.reset  (reset_sys                          ),
	.x      (x                                  ),
	.y      (y                                  ),
	.cs     (slot_cs_array[`V4_TEST_COLOR]      ),
	.write  (slot_mem_wr_array[`V4_TEST_COLOR]  ),
	.addr   (slot_reg_addr_array[`V4_TEST_COLOR]),
	.wr_data(slot_wr_data_array[`V4_TEST_COLOR] ),
	.si_rgb (frame_rgb                          ),
	.so_rgb (bar_rgb                            )
);


// instantiate mouse sprite
logic [CD-1:0] ball_rgb;
sprite_wrapper #(
	.BITMAP_FILE("ball.mem"),
	.H_SIZE     (16        ),
	.V_SIZE     (16        ),
	.CD         (CD        ),
	.CHROMA_KEY (CHROMA_KEY)
) v1_ball_unit (
	.clk    (clk_sys                      ),
	.reset  (reset_sys                    ),
	.x      (x                            ),
	.y      (y                            ),
	.cs     (slot_cs_array[`V3_BALL]      ),
	.write  (slot_mem_wr_array[`V3_BALL]  ),
	.addr   (slot_reg_addr_array[`V3_BALL]),
	.wr_data(slot_wr_data_array[`V3_BALL] ),
	.si_rgb (bar_rgb                      ),
	.so_rgb (ball_rgb                     )
);

// instantiate paddle sprite
logic [CD-1:0] paddle_rgb;
sprite_wrapper #(
	.BITMAP_FILE("paddle.mem"),
	.H_SIZE     (128         ),
	.V_SIZE     (16          ),
	.CD         (CD          ),
	.CHROMA_KEY (CHROMA_KEY  )
) v5_paddle_unit (
	.clk    (clk_sys                        ),
	.reset  (reset_sys                      ),
	.x      (x                              ),
	.y      (y                              ),
	.cs     (slot_cs_array[`V2_PADDLE]      ),
	.write  (slot_mem_wr_array[`V2_PADDLE]  ),
	.addr   (slot_reg_addr_array[`V2_PADDLE]),
	.wr_data(slot_wr_data_array[`V2_PADDLE] ),
	.si_rgb (ball_rgb                       ),
	.so_rgb (paddle_rgb                     )
);

// instantiate osd
logic [CD-1:0] text_rgb;
text_wrapper #(.CD(CD), .CHROMA_KEY(CHROMA_KEY)) v2_text_unit (
	// vga_dummy_core xxxx1 (
	.clk    (clk_sys                      ),
	.reset  (reset_sys                    ),
	.x      (x                            ),
	.y      (y                            ),
	.cs     (slot_cs_array[`V1_TEXT]      ),
	.write  (slot_mem_wr_array[`V1_TEXT]  ),
	.addr   (slot_reg_addr_array[`V1_TEXT]),
	.wr_data(slot_wr_data_array[`V1_TEXT] ),
	.si_rgb (paddle_rgb                   ),
	.so_rgb (text_rgb                     )
);

// merge start signal to rgb data stream
assign line_data_in = {text_rgb, frame_start_d2_reg};
// instantiate sync_core
vga_sync_wrapper #(.CD(CD)) v0_vga_sync_unit (
	.clk_sys (clk_sys                      ),
	.clk_25M (clk_25M                      ),
	.reset   (reset_sys                    ),
	.cs      (slot_cs_array[`V0_SYNC]      ),
	.write   (slot_mem_wr_array[`V0_SYNC]  ),
	.addr    (slot_reg_addr_array[`V0_SYNC]),
	.wr_data (slot_wr_data_array[`V0_SYNC] ),
	.si_data (line_data_in                 ),
	.si_valid(inc_d2_reg                   ),
	.si_ready(inc                          ),
	.hsync   (hsync                        ),
	.vsync   (vsync                        ),
	.rgb     (rgb                          )
);
endmodule
