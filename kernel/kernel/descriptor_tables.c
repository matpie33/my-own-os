#include <kernel/common.h>
#include <kernel/descriptor_tables.h>

extern void gdt_flush(u32int);

static void init_gdt();
static void gdt_set_gate(s32int,u32int,u32int,u8int,u8int);

gdt_entry gdt_entries[5];
gdt_pointer  gdt_ptr;

void init_descriptor_tables()
{
   init_gdt();
}

static void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry) * 5) - 1;
   gdt_ptr.base  = (u32int)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush((u32int)&gdt_ptr);
}

static void gdt_set_gate(s32int index, u32int base, u32int limit, u8int accessByte, u8int granularity)
{
   gdt_entries[index].base_low    = (base & 0xFFFF);
   gdt_entries[index].base_middle = (base >> 16) & 0xFF;
   gdt_entries[index].base_high   = (base >> 24) & 0xFF;

   gdt_entries[index].limit_low   = (limit & 0xFFFF);
   gdt_entries[index].granularity = (limit >> 16) & 0x0F;

   gdt_entries[index].granularity |= granularity & 0xF0;
   gdt_entries[index].access      = accessByte;
}