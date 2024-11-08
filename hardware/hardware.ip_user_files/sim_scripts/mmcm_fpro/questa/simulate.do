onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib mmcm_fpro_opt

do {wave.do}

view wave
view structure
view signals

do {mmcm_fpro.udo}

run -all

quit -force
