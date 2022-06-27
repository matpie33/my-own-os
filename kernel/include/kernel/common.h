#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define GIGABYTE 4294967296

typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t irq_number, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

void outbyte(uint16_t port, uint8_t value);
uint8_t inbyte(uint16_t port);
uint16_t inword(uint16_t port);

#endif