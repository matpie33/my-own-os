#include <stdint.h>
#include <kernel/interrupt_requests.h>

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA; 
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inbyte(port) | (1 << IRQline);
    outbyte(port, value);        
}
 
void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inbyte(port) & ~(1 << IRQline);
    outbyte(port, value);        
}
