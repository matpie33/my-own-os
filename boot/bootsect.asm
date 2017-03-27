[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; This is the memory offset to which we will load our kernel
mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it 's
; best to remember this for later.

mov bp, 0x9000 ; Set -up the stack.
mov sp, bp

; TODO print string routines should return cursor position
; also real mode print moves cursor, protected mode doesn't do it

call init_cursor_position
call load2ndStage

%include "boot/real_mode/print.asm"
%include "boot/disk/disk_load.asm"
%include "boot/real_mode/cursor.asm"

load2ndStage:
mov bx, KERNEL_OFFSET   ; Set -up parameters for our disk_load routine , so
mov dh, 2		; that we load the first 15 sectors ( excluding
mov dl, [ BOOT_DRIVE ]  ; the boot sector ) from the boot disk ( i.e. our
mov cl, 0x02
call disk_load 			; kernel code ) to address KERNEL_OFFSET

; This is where we arrive after  switching to and initialising protected mode.
mov bx, MSG_REAL_MODE
call println

mov bx, LOAD
call println

call KERNEL_OFFSET ; Now jump to the address of our loaded
mov bx, LOADED
call print
; kernel code , assume the brace position ,
; and cross your fingers. Here we go!
jmp $ ; Hang.
; Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_WAIT db "Now waiting...", 0
MSG_PROT_MODE db "Successfully landed in 32- bit Protected Mode.", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0
LOAD db "Switching to second bootloader.", 0
LOADED db "loaded already",0
; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
