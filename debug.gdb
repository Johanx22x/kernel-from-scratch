file /home/johanw/Projects/OS/build/bin/kernel.elf
target remote :1234
set disassembly-flavor intel
b _start
c
