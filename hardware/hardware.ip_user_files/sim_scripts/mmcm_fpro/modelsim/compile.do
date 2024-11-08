vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xpm
vlib modelsim_lib/msim/xil_defaultlib

vmap xpm modelsim_lib/msim/xpm
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib

vlog -work xpm -64 -incr -sv "+incdir+d:/DATN/hardware/hardware.srcs/sources_1/ip/mmcm_fpro" "+incdir+../../../../hardware.srcs/sources_1/ip/mmcm_fpro" \
"C:/Xilinx/Vivado/2019.2/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
"C:/Xilinx/Vivado/2019.2/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \

vcom -work xpm -64 -93 \
"C:/Xilinx/Vivado/2019.2/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work xil_defaultlib -64 -incr "+incdir+d:/DATN/hardware/hardware.srcs/sources_1/ip/mmcm_fpro" "+incdir+../../../../hardware.srcs/sources_1/ip/mmcm_fpro" \
"../../../../hardware.srcs/sources_1/ip/mmcm_fpro/mmcm_fpro_clk_wiz.v" \
"../../../../hardware.srcs/sources_1/ip/mmcm_fpro/mmcm_fpro.v" \

vlog -work xil_defaultlib \
"glbl.v"

