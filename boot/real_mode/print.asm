print:
	pusha
	mov cx,0

print_real_mode:
	mov al, [bx]
	cmp al,0
	je finish

	mov ah, 0x0e
	int 0x10
	add bx, 1
	add cx, 1

	jmp print_real_mode

finish: 	;clear the rest of this line
	mov al, ''
	cmp cx,80
	je end

	mov ah, 0x0e
	int 0x10
	add cx,1
	jmp finish

end:
	popa
	ret



