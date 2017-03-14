[extern start]
global _start
_start:

[bits 16]
call switch_to_pm

[bits 32]
BEGIN_PM:

mov edx, VIDEO_MEMORY+3*2*80 ;  Set edx to the start of vid mem.
call clear_screen

mov edx, VIDEO_MEMORY+2*2*80 ;  Set edx to the start of vid mem.
mov ebx, MSG_PROT_MODE ; Use our 32- bit print routine to
call print_string_pm ; announce we are in protected mode
call start
jmp $ 

%include "boot/pm/print_string_pm.asm"
%include "boot/pm/switch_to_pm.asm"
%include "boot/pm/clear_screen.asm"

MSG_PROT_MODE db "Successfully landed in 32- bit Protected Mode.", 0
BOOT_DRIVE db 0

