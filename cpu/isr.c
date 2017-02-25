#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "types.h"
#include "../libc/strings.h"
#include "port_read_write.h"

#define PIC_MASTER_COMMAND 0x20
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_MASTER_DATA (PIC_MASTER_COMMAND +1)
#define PIC_SLAVE_DATA (PIC_SLAVE_COMMAND +1)

#define END_OF_INTERRUPT 0x20
#define INITIALIZATION_AND_CONNECTION 0x11
#define MASTER_VECTOR_OFFSET 0x20
#define SLAVE_VECTOR_OFFSET 0x28
#define SLAVE_TO_MASTER_IRQ 0x04
#define ARCHITECTURE_80_86 0x01
#define PIC_MASK 0x0




isr_t interrupt_handlers[256];

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved", 
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* Can't do this with a loop because we need the address
 * of the function names */

void installISRs(){
	set_idt_gate(0, (u32)isr0);
	    set_idt_gate(1, (u32)isr1);
	    set_idt_gate(2, (u32)isr2);
	    set_idt_gate(3, (u32)isr3);
	    set_idt_gate(4, (u32)isr4);
	    set_idt_gate(5, (u32)isr5);
	    set_idt_gate(6, (u32)isr6);
	    set_idt_gate(7, (u32)isr7);
	    set_idt_gate(8, (u32)isr8);
	    set_idt_gate(9, (u32)isr9);
	    set_idt_gate(10, (u32)isr10);
	    set_idt_gate(11, (u32)isr11);
	    set_idt_gate(12, (u32)isr12);
	    set_idt_gate(13, (u32)isr13);
	    set_idt_gate(14, (u32)isr14);
	    set_idt_gate(15, (u32)isr15);
	    set_idt_gate(16, (u32)isr16);
	    set_idt_gate(17, (u32)isr17);
	    set_idt_gate(18, (u32)isr18);
	    set_idt_gate(19, (u32)isr19);
	    set_idt_gate(20, (u32)isr20);
	    set_idt_gate(21, (u32)isr21);
	    set_idt_gate(22, (u32)isr22);
	    set_idt_gate(23, (u32)isr23);
	    set_idt_gate(24, (u32)isr24);
	    set_idt_gate(25, (u32)isr25);
	    set_idt_gate(26, (u32)isr26);
	    set_idt_gate(27, (u32)isr27);
	    set_idt_gate(28, (u32)isr28);
	    set_idt_gate(29, (u32)isr29);
	    set_idt_gate(30, (u32)isr30);
	    set_idt_gate(31, (u32)isr31);
}

void remapThePIC(){
	port_byte_out(PIC_MASTER_COMMAND, INITIALIZATION_AND_CONNECTION);
	port_byte_out(PIC_SLAVE_COMMAND, INITIALIZATION_AND_CONNECTION);
	port_byte_out(PIC_MASTER_DATA, MASTER_VECTOR_OFFSET);
	port_byte_out(PIC_SLAVE_DATA, SLAVE_VECTOR_OFFSET);
	port_byte_out(PIC_MASTER_DATA, SLAVE_TO_MASTER_IRQ);
	port_byte_out(PIC_SLAVE_DATA, 0x02);
	port_byte_out(PIC_MASTER_DATA, ARCHITECTURE_80_86);
	port_byte_out(PIC_SLAVE_DATA, ARCHITECTURE_80_86);
	port_byte_out(PIC_MASTER_DATA, PIC_MASK);
	port_byte_out(PIC_SLAVE_DATA, PIC_MASK);
}

void installIRQs(){
	set_idt_gate(32, (u32)irq0);
	set_idt_gate(33, (u32)irq1);
	set_idt_gate(34, (u32)irq2);
	set_idt_gate(35, (u32)irq3);
	set_idt_gate(36, (u32)irq4);
	set_idt_gate(37, (u32)irq5);
	set_idt_gate(38, (u32)irq6);
	set_idt_gate(39, (u32)irq7);
	set_idt_gate(40, (u32)irq8);
	set_idt_gate(41, (u32)irq9);
	set_idt_gate(42, (u32)irq10);
	set_idt_gate(43, (u32)irq11);
	set_idt_gate(44, (u32)irq12);
	set_idt_gate(45, (u32)irq13);
	set_idt_gate(46, (u32)irq14);
	set_idt_gate(47, (u32)irq15);
}

void isr_install() {
	installISRs();
	remapThePIC();
	installIRQs();
	set_idt(); // Load with ASM
}

void isr_handler(registers_t r) {
    print("received interrupt: ");
    char s[3];
    int_to_ascii(r.int_no, s);
    print(s);
    println(exception_messages[r.int_no]);
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /* Tell the PICs that we handled the IRQ, so that he can accept
     * more IRQs */
    if (r.int_no >= 40) port_byte_out(PIC_SLAVE_COMMAND, END_OF_INTERRUPT); /* slave */
    port_byte_out(PIC_MASTER_COMMAND, END_OF_INTERRUPT); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}
