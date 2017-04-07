[org 0x1000]

KERNEL_OFFSET equ 0x5000 ; This is the memory offset to which we will load our kernel
mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it 's

mov bp, 0x9000 ; Set -up the stack.
mov sp, bp


[bits 16] ; VESA setting mode code

;; how to multiply? move to bx value, and call mul bx, then ax will be multiplied by ax
;; result stored in dx together with ax
mov ax, 45
mov bx, 10
mul bx

call print_hex
mov dx, ax
call print_hex


mov bx, MSG_SEARCHING_FOR_VIDEO_MODE
call println

call search_video_mode

jmp $

continue_l:
mov bx, KERNEL_OFFSET   ; Set -up parameters for our disk_load routine , so
mov dh, 15		; that we load the first 15 sectors ( excluding
mov dl, [ BOOT_DRIVE ]  ; the boot sector ) from the boot disk ( i.e. our
mov cl, 0x06 ;which sector we load; this file is starting as sector 2, so we have to set this value as
			 ;number of 512 bytes sectors this file consists of + 2
call disk_load 	
call 0x5000;
jmp $

%include "boot/real_mode/print.asm"
%include "boot/real_mode/print_hex.asm"
%include "boot/real_mode/cursor.asm"
%include "boot/real_mode/hex_to_dec.asm"

%include "boot/disk/disk_load.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/print_string_pm.asm"
%include "boot/pm/clear_screen.asm"
%include "boot/vesa/vesa.asm"

MSG_SEARCHING_FOR_VIDEO_MODE db "Searching for proper video mode.",0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_WAIT db "Now waiting...", 0
MSG_PROT_MODE db "Successfully landed in 32- bit Protected Mode.", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0
BOOT_DRIVE db 0

times 2048-($-$$) db 0 ;at this moment this file is 2022 bytes in lenght, so I put 512*4 = 2048, gott
	;but I should not hardcode it like that




