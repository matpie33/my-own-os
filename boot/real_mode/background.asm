set_background_color:
	pusha
	mov ah, 0x0B
	mov bh, 0x00
	mov bl, 0x09
	int 0x10
	popa
	ret
