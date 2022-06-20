#include <stdint.h>
#include <kernel/descriptor_tables.h>

extern void gdt_flush(uint32_t);

static void init_gdt();
static void gdt_set_gate(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);

gdt_entry gdt_entries[5];
gdt_pointer  gdt_ptr;

void init_descriptor_tables()
{
   init_gdt();
}

static void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry) * 5) - 1;
   gdt_ptr.base  = (uint32_t)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush((uint32_t)&gdt_ptr);
}

static void gdt_set_gate(int32_t index, uint32_t base, uint32_t limit, uint8_t accessByte, uint8_t granularity)
{
   gdt_entries[index].base_low    = (base & 0xFFFF);
   gdt_entries[index].base_middle = (base >> 16) & 0xFF;
   gdt_entries[index].base_high   = (base >> 24) & 0xFF;

   gdt_entries[index].limit_low   = (limit & 0xFFFF);
   gdt_entries[index].granularity = (limit >> 16) & 0x0F;

   gdt_entries[index].granularity |= granularity & 0xF0;
   gdt_entries[index].access      = accessByte;
}