[bits 32]

clear_screen :
	pusha
	mov ecx,0
		
clear :
	mov al, ''
	mov ah, GREEN_ON_BLACK
	cmp ecx, 2*80*24-2
	je done ;
	
	mov [edx], ax 	; Store char and attributes at current
					; character cell.
	
	add edx, 2 ; Move to next character cell in vid mem.
	add ecx, 2
	
	jmp clear ; loop around to print the next char.
	
done :
	popa
	ret ; Return from the function
