[extern start]
[extern test_method]
global _start
global best_video_mode
_start:

[bits 16]
mov bx, MSG_SEARCHING_FOR_VIDEO_MODE
call println

call search_video_mode
continue_l:

call switch_to_pm

[bits 32]
BEGIN_PM:
mov edx, VIDEO_MEMORY+2*80
mov ebx, 0x35
call print_string_pm
call start
jmp $

%include "boot/pm/switch_to_pm.asm"
%include "boot/pm/gdt.asm"
%include "boot/vesa/vesa.asm"
%include "boot/real_mode/print.asm"
%include "boot/real_mode/hex_to_dec.asm"
%include "boot/pm/print_string_pm.asm"

MSG_SEARCHING_FOR_VIDEO_MODE db "Searching for proper video mode.",0
