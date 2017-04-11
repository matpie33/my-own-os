; ConvertNumber
;   Input:
;     ax = Number to be converted
;     bx = Base
;   
;   Output:
;     si = Start of NUL-terminated buffer
;          containing the converted number
;          in ASCII represention.

hex_to_dec:
    push ax            ; Save modified registers
    push bx
    push dx
    mov bx, 10
    mov si, bufferend  ; Start at the end
.convert:
    xor dx, dx         ; Clear dx for division
    div bx             ; Divide by base
    add dl, '0'        ; Convert to printable char
    cmp dl, '9'        ; Hex digit?
    jbe .store         ; No. Store it
    add dl, 'A'-'0'-10 ; Adjust hex digit
.store:
    dec si             ; Move back one position
    mov [si], dl       ; Store converted digit
    and ax, ax         ; Division result 0?
    jnz .convert       ; No. Still digits to convert

	mov bx, si
	call print

    pop dx             ; Restore modified registers	
    pop bx
    pop ax
    ret
    
buffer: times 16 db 0
bufferend:
    db 0