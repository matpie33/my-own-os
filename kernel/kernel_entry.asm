[extern start]
global _start
_start:

[bits 16]
call switch_to_pm

[bits 32]
BEGIN_PM:
call start
jmp $ 

%include "boot/pm/switch_to_pm.asm"


