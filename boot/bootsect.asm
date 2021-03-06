[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ;This is the memory offset to which we will load our kernel
mov [BOOT_DRIVE], dl ;BIOS stores our boot drive in DL , so it 's
; best to remember this for later.

mov bp, 0x9000 ; Set -up the stack.
mov sp, bp

call init_cursor_position
call load2ndStage

[bits 16]
load2ndStage:
mov bx, KERNEL_OFFSET   ; Set -up parameters for our disk_load routine , so
mov dh, 45		; that we load the first 15 sectors ( excluding
mov dl, [ BOOT_DRIVE ]  ; the boot sector ) from the boot disk ( i.e. our
mov cl, 0x02
call disk_load 			; kernel code ) to address KERNEL_OFFSET
call KERNEL_OFFSET ; Now jump to the address of our loaded

jmp $ ; Hang.

%include "boot/real_mode/print.asm"
%include "boot/disk/disk_load.asm"
%include "boot/real_mode/cursor.asm"

BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
LOAD db "Loading kernel", 0
LOADED db "loaded already",0

times 510-($-$$) db 0
dw 0xaa55
