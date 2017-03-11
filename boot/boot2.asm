[org 0x1000]

KERNEL_OFFSET equ 0x5000 ; This is the memory offset to which we will load our kernel
mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it 's

mov bp, 0x9000 ; Set -up the stack.
mov sp, bp
mov bx, MSG_REAL_MODE
call print
[bits 16]
load2ndStage:
mov bx, KERNEL_OFFSET   ; Set -up parameters for our disk_load routine , so
mov cl, 0x02
mov dh, 31				; that we load the first 15 sectors ( excluding
mov dl, [ BOOT_DRIVE ]  ; the boot sector ) from the boot disk ( i.e. our
call disk_load 			; kernel code ) to address KERNEL_OFFSET

call switch_to_pm
BEGIN_PM:





jmp $

%include "boot/real_mode/print.asm"
%include "boot/real_mode/print_hex.asm"
%include "boot/real_mode/cursor.asm"

%include "boot/disk/disk_load.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/print_string_pm.asm"
%include "boot/pm/switch_to_pm.asm"
%include "boot/pm/clear_screen.asm"

MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_WAIT db "Now waiting...", 0
MSG_PROT_MODE db "Successfully landed in 32- bit Protected Mode.", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0
BOOT_DRIVE db 0

