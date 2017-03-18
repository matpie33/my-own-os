[org 0x1000]

KERNEL_OFFSET equ 0x5000 ; This is the memory offset to which we will load our kernel
mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it 's

mov bp, 0x9000 ; Set -up the stack.
mov sp, bp


 





[bits 16]
; VESA setting mode code



push es					; some VESA BIOSes destroy ES, or so I read
	mov ax, 0x4F00				; get VBE BIOS info
	mov di, vbe_info_block
	int 0x10
	pop es

mov dx, vbe_info_block.video_memory
call print

jmp $


%include "boot/real_mode/print.asm"
%include "boot/real_mode/print_hex.asm"
%include "boot/real_mode/cursor.asm"

%include "boot/disk/disk_load.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/print_string_pm.asm"
%include "boot/pm/clear_screen.asm"

MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_WAIT db "Now waiting...", 0
MSG_PROT_MODE db "Successfully landed in 32- bit Protected Mode.", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0
BOOT_DRIVE db 0
vbe_info_block:
	.signature		db "VBE2"	; indicate support for VBE 2.0+
	.table_data:	times 512-4 db 0	; reserve space for the table below

