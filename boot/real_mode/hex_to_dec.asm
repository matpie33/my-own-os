;input: dx
hex_to_dec:
	pusha
	mov cx, 0; power
	
loop:
	mov bx, dx
	and bx, 0x000f
	mov ax, 16
	mul cx ; multiply power * 16
	mov ax, bx
	mul cx ; multiply digit * 16^power