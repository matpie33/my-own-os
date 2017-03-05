init_cursor_position:
	pusha
	mov ah, 02h
	mov bh, 00h
	mov dh, 00h
	mov dl, 00h
	int 0x10
	popa
	ret
