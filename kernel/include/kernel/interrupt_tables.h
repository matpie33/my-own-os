#include <kernel/common.h>

typedef struct {
	u16int    isr_low;      // The lower 16 bits of the ISR's address
	u16int    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	u8int     reserved;     // Set to zero
	u8int     attributes;   // Type and attributes; see the IDT page
	u16int    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry;

typedef struct {
	u16int	limit;
	u32int	base;
} __attribute__((packed)) idtr_t;

void init_idt();
void idt_set_gate(u8int,u32int,u16int,u8int);

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31 ();
