[bits 32]
; Define some constants

; prints a null - terminated string pointed to by EDX
clear_screen :
	pusha
	mov ecx,0
		
clear :
	mov al, '' ; Store the char at EBX  in AL
	mov ah, GREEN_ON_BLACK ; Store the attributes in AH
	cmp ecx, 2*80*25-2 ; if (al == 0), at end of string , so
	je done ; jump to done
	
	mov [edx], ax 	; Store char and attributes at current
					; character cell.
	
	add edx, 2 ; Move to next character cell in vid mem.
	add ecx, 2
	
	jmp clear ; loop around to print the next char.
	
done :
	popa
	ret ; Return from the function