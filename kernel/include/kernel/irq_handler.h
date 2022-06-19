#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include <stdint.h>
#include <kernel/common.h>

#define IRQ0 0
#define IRQ1 1
#define IRQ2 2
#define IRQ3 3
#define IRQ4 4
#define IRQ5 5
#define IRQ6 6
#define IRQ7 7
#define IRQ8 8
#define IRQ9 9
#define IRQ10 10
#define IRQ11 11
#define IRQ12 12
#define IRQ13 13
#define IRQ14 14
#define IRQ15 15

#define INTERRUPT_NUMBER_FOR_FIRST_IRQ (32)


typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);


#endif