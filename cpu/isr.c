#include "isr.h"
#include "idt.h"
#include "types.h"
#include "../libc/strings.h"
#include "port_read_write.h"
#include "../graphics/draw_pixel.h"
#include "../graphics/draw_string.h"
#include "../libc/hex_to_string.h"

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
	set_idt_gate(0, (uint32_t)isr0);
	set_idt_gate(1, (uint32_t)isr1);
	set_idt_gate(2, (uint32_t)isr2);
	set_idt_gate(3, (uint32_t)isr3);
	set_idt_gate(4, (uint32_t)isr4);
	set_idt_gate(5, (uint32_t)isr5);
	set_idt_gate(6, (uint32_t)isr6);
	set_idt_gate(7, (uint32_t)isr7);
	set_idt_gate(8, (uint32_t)isr8);
	set_idt_gate(9, (uint32_t)isr9);
	set_idt_gate(10, (uint32_t)isr10);
	set_idt_gate(11, (uint32_t)isr11);
	set_idt_gate(12, (uint32_t)isr12);
	set_idt_gate(13, (uint32_t)isr13);
	set_idt_gate(14, (uint32_t)isr14);
	set_idt_gate(15, (uint32_t)isr15);
	set_idt_gate(16, (uint32_t)isr16);
	set_idt_gate(17, (uint32_t)isr17);
	set_idt_gate(18, (uint32_t)isr18);
	set_idt_gate(19, (uint32_t)isr19);
	set_idt_gate(20, (uint32_t)isr20);
	set_idt_gate(21, (uint32_t)isr21);
	set_idt_gate(22, (uint32_t)isr22);
	set_idt_gate(23, (uint32_t)isr23);
	set_idt_gate(24, (uint32_t)isr24);
	set_idt_gate(25, (uint32_t)isr25);
	set_idt_gate(26, (uint32_t)isr26);
	set_idt_gate(27, (uint32_t)isr27);
	set_idt_gate(28, (uint32_t)isr28);
	set_idt_gate(29, (uint32_t)isr29);
	set_idt_gate(30, (uint32_t)isr30);
	set_idt_gate(31, (uint32_t)isr31);
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
	set_idt_gate(32, (uint32_t)irq0);
	set_idt_gate(33, (uint32_t)irq1);
	set_idt_gate(34, (uint32_t)irq2);
	set_idt_gate(35, (uint32_t)irq3);
	set_idt_gate(36, (uint32_t)irq4);
	set_idt_gate(37, (uint32_t)irq5);
	set_idt_gate(38, (uint32_t)irq6);
	set_idt_gate(39, (uint32_t)irq7);
	set_idt_gate(40, (uint32_t)irq8);
	set_idt_gate(41, (uint32_t)irq9);
	set_idt_gate(42, (uint32_t)irq10);
	set_idt_gate(43, (uint32_t)irq11);
	set_idt_gate(44, (uint32_t)irq12);
	set_idt_gate(45, (uint32_t)irq13);
	set_idt_gate(46, (uint32_t)irq14);
	set_idt_gate(47, (uint32_t)irq15);
}

void isr_install() {
	installISRs();
	remapThePIC();
	installIRQs();
	set_idt(); // Load with ASM
}

void isr_handler(registers_t* r) {
    println("Received interrupt: ");
    char s[3];
    int_to_ascii(r->int_no, s);
    println(s);
    println(exception_messages[r->int_no]);
    if (r->int_no==0){
    	asm volatile("cli");
    	asm volatile("hlt");
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t* r) {
    /* Tell the PICs that we handled the IRQ, so that he can accept
     * more IRQs */

    if (r->int_no >= 40) port_byte_out(PIC_SLAVE_COMMAND, END_OF_INTERRUPT); /* slave */
    port_byte_out(PIC_MASTER_COMMAND, END_OF_INTERRUPT); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r->int_no] != 0) {

        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
}
