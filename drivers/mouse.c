#include "../cpu/port_read_write.h"
#include <stdint.h>
#include "../libc/printf.h"
#include "../cpu/timer.h"
#include "../cpu/isr.h"
#include "../libc/function.h"
#include "../graphics/draw_string.h"

#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

#define COMMAND_GET_MOUSE_ID 0xF2
#define ACK_BYTE 0xFA
#define AA_BYTE 0xAA
#define D4_BYTE 0xD4

#define COMMAND_GET_COMPAQ_STATUS_BYTE 0x20
#define COMMAND_SET_COMPAQ_STATUS_BYTE 0x60
#define COMMAND_ENABLE_AUXILIARY_DEVICE 0xA8
#define COMMAND_RESET 0xFF
#define COMMAND_SET_DEFAULTS 0xF6
#define COMMAND_ENABLE_PACKET_STREAMING 0xF4
#define COMMAND_SET_SAMPLE_RATE 0xF3

const uint16_t sleep_time = 1000;

void wait_before_sending(){
	while ((port_byte_in(COMMAND_PORT) & 2) != 0 ){
		printf("Wbs ");
		sleep(sleep_time);
	}
}


void wait_before_getting(){
	uint8_t hey = port_byte_in(COMMAND_PORT);
	while ( (hey & 1) != 1 ){
		hey = port_byte_in(COMMAND_PORT);
		sleep(sleep_time);
	}
}

void send_to_data_port (uint8_t data_byte){
	port_byte_out(DATA_PORT, data_byte);
}

void send_to_command_port (uint8_t command_byte){
	wait_before_sending();
	port_byte_out(COMMAND_PORT, command_byte);
}



uint8_t get_from_port (uint8_t port){
	wait_before_getting();
	return port_byte_in(port);
}

uint8_t enable_irq_disable_clock_on_status_byte(uint8_t status_byte){
	uint8_t new_status_byte = status_byte | 2;
	new_status_byte = new_status_byte & ~ 0x20;
	return new_status_byte;
}

void wait_for_ack(){
	uint8_t ack = get_from_port(DATA_PORT);
	while (ack != ACK_BYTE){
		sleep(sleep_time);
		ack = get_from_port(DATA_PORT);
	}
}

void send_command (uint8_t command_byte){
	send_to_command_port(D4_BYTE);
	send_to_data_port(command_byte);

	wait_for_ack();
}

void send_command2 (uint8_t command_byte){
	send_to_data_port(command_byte);
	wait_for_ack();
}

void send_data (uint8_t data_byte){
	send_to_command_port(D4_BYTE);
	send_to_command_port(data_byte);
	wait_for_ack();
}


void enable_irq_12 (){

//	printf("get status byte");
	send_to_command_port(COMMAND_GET_COMPAQ_STATUS_BYTE); //0x20
//	printf("waiting for status byte");
	uint8_t status = get_from_port(DATA_PORT);
//	printf("status byte:  %x",status);
	status = enable_irq_disable_clock_on_status_byte(status);
//	printf("setting compaq status command");
	send_to_command_port(COMMAND_SET_COMPAQ_STATUS_BYTE); //0x60
//	printf("sending status byte");
	send_to_data_port(status);
//	printf("sent new status byte with irq enabled");
//	printf("setting defaults");
}

uint8_t get_mouse_id(){
//	printf("Getting mouse id");
	send_command(COMMAND_GET_MOUSE_ID);
	uint8_t mouse_id = get_from_port(DATA_PORT);
	printf("mouse id is %x", mouse_id);
	return mouse_id;

}

static void mouse_callback(registers_t* regs){
	uint8_t value = get_from_port(DATA_PORT);
	print_hex(value);
	print_string(",");
	UNUSED(regs);
}

void enable_mouse_wheel(){
//	printf("setting sample rate");
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(200);
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(100);
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(80);
	uint8_t mouse_id = get_mouse_id();
	if (mouse_id == 0x03){
		printf("mouse wheel enabled");
	}
}

void enable_packets (){
	printf("set sample rate 40");
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(40);
	printf("done");
	send_command(COMMAND_ENABLE_PACKET_STREAMING);
	printf("enabled packet streaming");
}

void init_mouse(){
	enable_irq_12();
	get_mouse_id();
	enable_mouse_wheel();
	enable_packets();
	register_interrupt_handler(IRQ12, mouse_callback);
}
