[extern start]
global _start
_start:

[bits 16]
mov bx, MSG_SEARCHING_FOR_VIDEO_MODE
call println

call search_video_mode
continue_l:

call switch_to_pm

[bits 32]
BEGIN_PM:
call start
jmp $

%include "boot/pm/switch_to_pm.asm"
%include "boot/pm/gdt.asm"
%include "boot/vesa/vesa.asm"
%include "boot/real_mode/print.asm"
%include "boot/real_mode/hex_to_dec.asm"

MSG_SEARCHING_FOR_VIDEO_MODE db "Searching for proper video mode.",0
