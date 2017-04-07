width equ 1024
height equ 768
bpp equ 32

search_video_mode:

[bits 16]

; read information
push es						; some VESA BIOSes destroy ES, or so I read
mov ax, 0x4F00				; get VBE BIOS info
mov di, vbe_info_block
int 0x10
pop es
	
; check if success
cmp ax, 0x4F
jne failed_call_vesa
mov bx, MSG_SUCCESS_VESA
call println
call new_line

; read supported modes
mov ax, word [vbe_info_block.video_modes]
mov [offset], ax
mov ax, word [vbe_info_block.video_modes+2]
mov [segments], ax

mov ax, [segments]
mov fs, ax
mov si, [offset]

mov bx, MSG_READ_MODE
call println


mov cx, 1
push cx

find_mode:

	mov dx, [fs:si]
	add si, 2
	mov [offset], si
	mov [mode], dx
	mov ax, 0
	mov fs, ax
	
	cmp [mode], word 0xFFFF
	je	failed_mode
	
	push es 
	mov ax, 0x4F01
	mov cx, [mode]
	mov di, mode_info_block
	int 0x10
	pop es
	
	cmp ax, 0x004F
	jne failed_call_vesa
	
	call display_current_mode
	
	pop cx
	cmp cx, 4
	je reset_counter
	inc cx
	push cx

	check_mode:
		mov cx, 0; flag - 0 it's not better than currently chosen mode, 1 otherwise
		mov bx, [best_video_mode.width]
		cmp bx, [mode_info_block.width]
		jl save_and_continue
		je compare_height
		jmp next_mode
		
	compare_height:	
		mov bx, [best_video_mode.height]
		cmp bx, [mode_info_block.height]
		jl save_and_continue
		je compare_bpp
		jmp next_mode
		
	compare_bpp:
		mov dx, [best_video_mode.bpp]
		mov ax, [mode_info_block.bpp]
		and ax, 11111111b
		cmp dx, ax
		jl save_and_continue
		jmp next_mode
		
	save_and_continue:
		mov bx, [mode_info_block.width]
		mov [best_video_mode.width], bx
		
		shr bx,3
		dec bx
		mov [best_video_mode.x_cur_max], bx
		
		mov bx, [mode_info_block.height]
		mov [best_video_mode.height], bx
		
		shr bx, 4
		dec bx
		mov word [best_video_mode.y_cur_max], ax
		
		mov ebx, 0
		mov bl, [mode_info_block.bpp]
		mov [best_video_mode.bpp], bl
		shr ebx, 3
		mov dword [best_video_mode.bytes_per_pixel], ebx
		
		
		mov bx, [mode]
		mov [best_video_mode.mode], bx
		mov ebx, [mode_info_block.framebuffer]
		mov [best_video_mode.framebuffer], ebx
		mov bx, [mode_info_block.pitch]
		mov [best_video_mode.bytes_per_line],bx
		jmp next_mode

pop es	

jmp $

new_line_and_next_mode:
	call new_line
	mov cx,1
	push cx
	jmp next_mode

reset_counter:
	call new_line
	mov cx, 1
	push cx
	jmp check_mode

display_current_mode:

	mov ax, [mode_info_block.width]
	call hex_to_dec
	
	mov bx, X
	call print

	mov ax, [mode_info_block.height]
	call hex_to_dec
	
	mov bx, X
	call print
	
	mov cx, [mode_info_block.bpp]
	and cx, 11111111b
	mov ax, cx
	call hex_to_dec
	
	mov bx, MODE_SEPARATOR
	call print
	ret

failed_call_vesa:
	call new_line	
	mov bx, MSG_FAIL_VESA
	call println
	jmp $

failed_mode:
	call new_line
	mov bx, MSG_FAIL_MODE
	call println

set_mode:

	;push es
	;mov ax, 0x4F02
	;mov bx, [best_video_mode.mode]
	;or bx, 0x4000
	;mov di, 0
	;int 0x10
	;pop es
	
	;cmp ax, 0x4F
	;jne failed_call_vesa
	
	mov bx, MSG_SUCCESS_SETTING_MODE
	call println
	clc 
	call continue_l
	
	
	
highest_mode:
	call new_line
	mov bx, MSG_BEST_MODE
	call print
	mov ax, [best_video_mode.width]
	call hex_to_dec
	
	mov bx, X
	call print
	
	mov ax, [best_video_mode.height]
	call hex_to_dec
	
	mov bx, X
	call print
	
	mov ax, [best_video_mode.bpp]
	call hex_to_dec
	jmp $
	
next_mode:
	mov ax, [segments]
	mov fs, ax
	mov si, [offset]
	jmp find_mode

%include "boot/vesa/vesa_variables.asm"