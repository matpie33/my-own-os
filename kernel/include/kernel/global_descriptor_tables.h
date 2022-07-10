#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include <stdint.h>

struct gdt_entry_struct
{
   uint16_t limit_low;  // The lower 16 bits of the limit.
   uint16_t base_low;   // The lower 16 bits of the base.
   uint8_t base_middle; // The next 8 bits of the base.
   uint8_t access;      // Access flags
   uint8_t granularity;
   uint8_t base_high; // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry;

struct gdt_pointer_struct
{
   uint16_t limit; // The upper 16 bits of all selector limits.
   uint32_t base;  // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct gdt_pointer_struct gdt_pointer;

void init_gdt();

#endif