
set disassembly-flavor intel
set architecture i8086
target remote localhost:26000
symbol-file kernel/myos.kernel
b key_press_handler.c:134
