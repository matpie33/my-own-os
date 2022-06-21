#include <kernel/keyboard.h>
#include <kernel/common.h>
#include <kernel/irq_handler.h>
#include <stdio.h>
#include <kernel/interrupt_requests.h>
#include <kernel/key_press_handler.h>

static void keyboard_callback(registers_t* regs) {
	uint8_t scancode = inbyte(0x60);
	handle_key(scancode);
}

void init_keyboard() {
    IRQ_clear_mask(1);
	register_interrupt_handler(IRQ1, keyboard_callback);
}