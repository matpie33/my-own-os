#include "../cpu/port_read_write.h"
#include <stdint.h>
#include "../libc/printf.h"
#include "../cpu/timer.h"
#include "../cpu/isr.h"
#include "../libc/function.h"
#include "../graphics/draw_string.h"
#include "../util/util.h"
#include "../graphics/mouse_cursor.h"

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
uint8_t byte_number_counter;
uint8_t bytes_in_packet;
boolean left_button_pressed;
boolean right_button_pressed;
boolean middle_button_pressed;
 //TODO 2 printf signed int

void wait_before_sending(){
	while ((port_byte_in(COMMAND_PORT) & 2) != 0 ){
		sleep(sleep_time);
	}
}


void wait_before_getting(){
	uint8_t hey = port_byte_in(COMMAND_PORT);
	while ( (hey & 1) != 1 ){ //TODO use my utils instead
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

	send_to_command_port(COMMAND_GET_COMPAQ_STATUS_BYTE); //0x20
	uint8_t status = get_from_port(DATA_PORT);
	status = enable_irq_disable_clock_on_status_byte(status);
	send_to_command_port(COMMAND_SET_COMPAQ_STATUS_BYTE); //0x60
	send_to_data_port(status);
}

uint8_t get_mouse_id(){
	send_command(COMMAND_GET_MOUSE_ID);
	uint8_t mouse_id = get_from_port(DATA_PORT);
	//TODO handle in printf the situation when we use % but don't give the next variable
	return mouse_id;

}

boolean is_left_button_pressed (uint8_t packet_first_byte){
	return check_if_bit_is_set(packet_first_byte, 0);
}

boolean is_right_button_pressed (uint8_t packet_first_byte){
	return check_if_bit_is_set(packet_first_byte, 1);
}

boolean is_middle_button_pressed (uint8_t packet_first_byte){
	return check_if_bit_is_set(packet_first_byte, 2);
}

void check_for_clicking (uint8_t packet_first_byte){
	if (byte_number_counter ==1){
			boolean left_button = is_left_button_pressed(packet_first_byte);
			if (left_button){ //TODO when we hold the click and move mouse we should not get "left click message"
				left_button_pressed = true;
			}
			else if (left_button_pressed){
				left_button_pressed = false;
			}
			boolean right_button = is_right_button_pressed (packet_first_byte);
			if (right_button){
				right_button_pressed = true;
			}
			else if (right_button_pressed){
				right_button_pressed = false;
			}
			boolean middle_button = is_middle_button_pressed (packet_first_byte);
			if (middle_button){ //TODO not working properly
				middle_button_pressed = true;
			}
			else if (middle_button_pressed){
				middle_button_pressed = false;
			}
		}
}


void move_mouse (int8_t value){
	if (byte_number_counter == 2){
		move_cursor_horizontally(value);
	}
	if (byte_number_counter == 3){
		value*=-1;
		move_cursor_vertically(value);
	}
}

static void mouse_callback(registers_t* regs){

	if (byte_number_counter > bytes_in_packet){
		byte_number_counter=1;
	}
	int8_t value = get_from_port(DATA_PORT);
	check_for_clicking(value);
	move_mouse (value);
	byte_number_counter ++;
	UNUSED(regs);
}

void enable_mouse_wheel(){
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(200);
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(100);
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(80);
	uint8_t mouse_id = get_mouse_id();
	if (mouse_id == 0x03){
		bytes_in_packet = 4;
	}
}

void enable_packets (){
	send_command(COMMAND_SET_SAMPLE_RATE);
	send_command(100);
	send_command(COMMAND_ENABLE_PACKET_STREAMING);
}

void initiate_variables (){
	byte_number_counter = 1;
	bytes_in_packet = 3;
	left_button_pressed = false;
	right_button_pressed = false;
	middle_button_pressed = false;
}

void init_mouse(){
	initiate_variables();
	enable_irq_12();
	get_mouse_id();
	enable_mouse_wheel();
	enable_packets();
	register_interrupt_handler(IRQ12, mouse_callback);
}
