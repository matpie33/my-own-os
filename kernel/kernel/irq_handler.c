#include <stdint.h>
#include <kernel/irq_handler.h>


isr_t irq_handlers[16];

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t* registers_pointer)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (registers_pointer->irq_number >= 8)
   {
       // Send reset signal to slave.
       outbyte(0xA0, 0x20);
   }

   if (irq_handlers[registers_pointer->irq_number] != 0)
   {
       isr_t handler = irq_handlers[registers_pointer->irq_number];
       handler(registers_pointer);
   }

   // Send reset signal to master. (As well as slave, if necessary).
   outbyte(0x20, 0x20);
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  irq_handlers[n] = handler;
}

