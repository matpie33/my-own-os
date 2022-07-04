#include <stdio.h>
#include <kernel/common.h>


void isr_handler(registers_t* registers)
{
   registers_t reg_value = registers[0];
   printf("Received interrupt %d ", reg_value.irq_number);  
   for (;;){
      asm("hlt");
   } 
   
}