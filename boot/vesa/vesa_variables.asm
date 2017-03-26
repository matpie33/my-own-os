X db "X",0
MODE_SEPARATOR db "; ", 0
; VESA

MSG_WIDTH_OK db "Width is ok", 0
MSG_HEIGHT_OK db "Height ok", 0
MSG_BPP_OK db "bpp ok", 0
MSG_FOUND_MODE db "Successfully found desired mode", 0
MSG_SUCCESS_VESA db "Successfully performed call to VESA", 0
MSG_FAIL_VESA db "Failed to read vesa information", 0
MSG_FAIL_MODE db "Failed to read mode - it ended in FFFF", 0

MSG_READ_MODE db "Available video modes in structure: width X height X bits per pixel:", 0

vbe_info_block:
	.signature					db "VBE2"	; indicate support for VBE 2.0+
	.version					dw 0
	.cem						dd 0
	.capabilities 				dd 0
	.video_modes				dd 0
	.video_memory				dw 0
	.software_rev				dw 0
	.vendor						dd 0
	.product_name				dd 0
	.product_rev				dd 0
	.reserved					times 222 db 0
	.cem_data					times 256 db 0
	
mode_info_block:
	.attributes					dw 0
	.window_a					db 0
	.window_b					db 0
	.granularity				dw 0
	.window_size				dw 0
	.segment_a					dw 0
	.segment_b					dw 0
	.win_func_ptr			 	dd 0
	.pitch						dw 0
	.width						dw 0
	.height						dw 0
	.w_char						db 0
	.y_char						db 0
	.planes						db 0
	.bpp						db 0
	.banks						db 0
	.memory_model				db 0
	.bank_size					db 0
	.image_pages				db 0
	.reserved0					db 0
	
	.red_mask					db 0
	.red_position				db 0
	.green_mask					db 0
	.green_position				db 0
	.blue_mask					db 0
	.blue_position				db 0
	.reserved_mask				db 0
	.reserved_position			db 0
	.direct_color_attributes	db 0
	
	.framebuffer				dd 0					
	.off_screen_mem_off			dd 0
	.off_screen_mem_size		dw 0	
	.reserved1					times 206 db 0
	
	
segments dw 0
offset  dw 0
mode 	dw 0

