[bits 32]
; Define some constants
VIDEO_MEMORY equ 0xb8000
GREEN_ON_BLACK equ 0x0B

; prints a null - terminated string pointed to by EDX
print_string_pm :
	pusha
		
print_string_pm_loop :
	mov al, [ebx] ; Store the char at EBX in AL
	mov ah, GREEN_ON_BLACK ; Store the attributes in AH
	cmp al, 0 ; if (al == 0), at end of string , so
	je print_string_pm_done ; jump to done
	
	mov [edx], ax 	; Store char and attributes at current
					; character cell.
	add ebx, 1 ; Increment EBX to the next char in string.
	add edx, 2 ; Move to next character cell in vid mem.
	
	jmp print_string_pm_loop ; loop around to print the next char.
	
print_string_pm_done :
	popa
	add edx, 2*80 ;add new line TODO this adds new line to the line from which we started, not from which we ended up
	ret ; Return from the function
