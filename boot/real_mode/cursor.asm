init_cursor_position:
	pusha
	mov ah, 02h
	mov bh, 00h
	mov dh, 00h
	mov dl, 00h
	int 0x10
	mov cx,0

loop:


	mov ah, 0x0e
	mov al, ' '
	int 0x10

	add cx,1
	cmp cx, 80*25
	jne loop

init:

	mov ah, 02h
	mov bh, 00h
	mov dh, 00h
	mov dl, 00h
	int 0x10
	popa
	ret
