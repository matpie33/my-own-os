#include "timer.h"
#include "isr.h"
#include "types.h"
#include "port_read_write.h"
#include "../libc/function.h"
#include "../libc/strings.h"

#define COMMAND_CHANNEL_0_LO_AND_HI_BYTE_SQUARE_WAVE_BINARY 0x36
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL_0_DATA_PORT 0x40
#define PIT_OSCILLATOR_FREQUENCY_HZ 1193180

uint32_t tick = 0;

static void timer_callback (registers_t* regs){
//	UNUSED(regs);
	tick++;
//
//	if (tick%100==0){
//		char tick_ascii [256];
//		int_to_ascii(tick/100, tick_ascii);
//	}
	UNUSED(regs);

}

void init_timer(uint32_t freq){
	register_interrupt_handler(IRQ0, timer_callback);

	uint32_t divisor = PIT_OSCILLATOR_FREQUENCY_HZ / freq;
	uint8_t low = (uint8_t) (divisor & 0xFF);
	uint8_t high = (uint8_t) ( (divisor>>8) & 0xFF);

	port_byte_out (PIT_COMMAND_PORT, COMMAND_CHANNEL_0_LO_AND_HI_BYTE_SQUARE_WAVE_BINARY);
	port_byte_out (PIT_CHANNEL_0_DATA_PORT, low);
	port_byte_out (PIT_CHANNEL_0_DATA_PORT, high);
}

void sleep (uint32_t time){
	uint32_t start = tick;
	while (tick<time/10+start){

	}
}






