new_line:
	mov ah, 0x0e
	mov al, 13
	int 0x10
	mov al, 10
	int 0x10
	ret

println:
	call print
	call new_line
	ret

print:
	pusha
	mov cx, 0
	jmp print_real_mode


print_real_mode:
	mov al, [bx]
	cmp al,0
	je finish

	mov ah, 0x0e
	int 0x10
	add bx, 1
	add cx, 1

	jmp print_real_mode

finish:
	popa
	ret


