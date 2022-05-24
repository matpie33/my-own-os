
bits 16
start: jmp boot

msg db "Welcome to my operating system", 0ah, 0dh, 0h

boot:
	cli
	cld
	
	mov ax, 0x50
	mov es, ax
	xor bx, bx

	mov al, 2
	mov ch, 0 
	mov cl, 2
	mov dh, 0
	mov dl, 0
	mov ah, 0x02
	int 0x13
	jmp 0x50:0x0

	hlt
	
times 510 - ($-$$) db 0
dw 0xAA55
