. settings64.sh
export TERM=xterm-color
arm-xilinx-eabi-gdb ../src/main.elf -ex "target remote :1234" -ex "layout asm"
