.global idt_flush

idt_flush:
    movl 4(%esp), %eax  # Get the pointer to the IDT, passed as a parameter.
    lidt (%eax)        # Load the IDT pointer.
    ret

.global enable_interrupts
enable_interrupts:
    sti
    ret

.extern isr_handler

.macro isr_err_stub p
.global isr\p
isr\p:
    push $\p
    jmp isr_common_stub  
    iret 
.endm

.macro isr_no_err_stub p
.global isr\p
isr\p:
    push $0
    push $\p
     jmp  isr_common_stub  
    iret
.endm


isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_no_err_stub 17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31


isr_common_stub:
   pusha                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov %ds, %ax               # Lower 16-bits of eax = ds.
   push %eax                 # save the data segment descriptor

   mov $0x10, %ax # load the kernel data segment descriptor
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %ds
   mov %ax, %gs

    push %esp
   call isr_handler
   add $4, %esp 

   pop %eax        # reload the original data segment descriptor
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs

   popa                     # Pops edi,esi,ebp...
   add $8, %esp    # Cleans up the pushed error code and pushed ISR number   
   iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

