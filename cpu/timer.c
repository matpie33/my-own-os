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

u32 tick = 0;

static void timer_callback (registers_t* regs){
	UNUSED(regs);
	tick++;

	if (tick%100==0){
		char tick_ascii [256];
		int_to_ascii(tick/100, tick_ascii);
	}

}

void init_timer(u32 freq){
	register_interrupt_handler(IRQ0, timer_callback);

	u32 divisor = PIT_OSCILLATOR_FREQUENCY_HZ / freq;
	u8 low = (u8) (divisor & 0xFF);
	u8 high = (u8) ( (divisor>>8) & 0xFF);

	port_byte_out (PIT_COMMAND_PORT, COMMAND_CHANNEL_0_LO_AND_HI_BYTE_SQUARE_WAVE_BINARY);
	port_byte_out (PIT_CHANNEL_0_DATA_PORT, low);
	port_byte_out (PIT_CHANNEL_0_DATA_PORT, high);
}






