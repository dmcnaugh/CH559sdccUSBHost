#!/bin/zsh
#@echo off 
project_name=CH559USB
xram_size=0x0800
xram_loc=0x0600
code_size=0xEFFF
dfreq_sys=48000000
#
#if not exist "config.h" echo //add your personal defines here > config.h
#
sdcc -c -V -mmcs51 --model-large --xram-size $xram_size --xram-loc $xram_loc --code-size $code_size -I/ -DFREQ_SYS=$dfreq_sys  main.c
sdcc -c -V -mmcs51 --model-large --xram-size $xram_size --xram-loc $xram_loc --code-size $code_size -I/ -DFREQ_SYS=$dfreq_sys  util.c
sdcc -c -V -mmcs51 --model-large --xram-size $xram_size --xram-loc $xram_loc --code-size $code_size -I/ -DFREQ_SYS=$dfreq_sys  USBHost.c
sdcc -c -V -mmcs51 --model-large --xram-size $xram_size --xram-loc $xram_loc --code-size $code_size -I/ -DFREQ_SYS=$dfreq_sys  uart.c
#
sdcc main.rel util.rel USBHost.rel uart.rel -V -mmcs51 --model-large --xram-size $xram_size --xram-loc $xram_loc --code-size $code_size -I/ -DFREQ_SYS=$dfreq_sys  -o $project_name.ihx
#
packihx $project_name.ihx > $project_name.hex
#
makebin -p $project_name.hex $project_name.bin
#
rm $project_name.lk
rm $project_name.map
rm $project_name.mem
rm $project_name.ihx
#
rm *.asm
rm *.lst
rm *.rel
rm *.rst
rm *.sym
#
# This tool flashes the bin file directly to the ch559 chip, you need to install the libusb-win32 driver with the zadig( https://zadig.akeo.ie/ ) tool so the tool can access the usb device
# chflasher.exe $project_name.bin
#