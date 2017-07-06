#include "keyboard.h"
#include "stdint.h"

#include "../cpu/isr.h"
#include "../cpu/port_read_write.h"
#include "../cpu/types.h"
#include "../graphics/draw_string.h"
#include "../kernel/kernel.h"
#include "../libc/function.h"
#include "../libc/strings.h"

#define BACKSPACE 0x0E
#define TAB 0x0F
#define ENTER 0x1C
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define SHIFT_RELEASE 0xAA
#define CAPSLOCK_PRESS 0x3A
#define ESCAPE 0x01
#define LEFT_CTRL 0x1d
#define ALT 0x38
#define ESCAPE_CODE 0xE0

#define KEY_PRESSED 1
#define KEY_NOT_PRESSED 0

#define KEY_BUFFER_SIZE 256
#define SC_MAX 57

static char key_buffer[KEY_BUFFER_SIZE];

const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
        "/", "RShift", "Keypad *", "LAlt", "Spacebar","Capslock"};

const char sc_ascii_capital[] = { '?', '?', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ':', '"', '`', '?', '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', '?', '*', '?', ' ', '?', ' ',' ',
		' ',' ',' ',' ', ' ', ' ', ' ', ' ', ' ',' ',
		'7','8','9','-','4','5','6','+','1', '2', '3', '0', ' ', ' ',
		' ', ' ', ' ', ' '};

const char sc_ascii_low[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '?', '?', '*', '?', ' ', '?', ' ',' ',
		' ',' ',' ',' ', ' ', ' ', ' ', ' ', ' ',' ',
		'7','8','9','-','4','5','6','+','1', '2', '3', '0', ' ', ' ',
		' ', ' ', ' ', ' '};

static int shift_on = KEY_NOT_PRESSED;
static int capslock_on = KEY_NOT_PRESSED; //TODO we are supposing that caps is off when OS starts which is wrong
static uint8_t previous_key = 0;

boolean capslock_and_shift_both_on_or_off (){
	return (shift_on && capslock_on) || (!shift_on && !capslock_on);
}

boolean is_letter(uint8_t scancode){
	return (scancode <=0X19 && scancode>=0X10)
			|| (scancode <=0X26 && scancode>=0x1E) || (scancode<=0X32 && scancode >=0x2C);
}

void handle_shift_and_capslock(uint8_t scancode){
	if (scancode == LEFT_SHIFT){
		shift_on=1-shift_on;
	}
	else if (scancode == SHIFT_RELEASE){
		shift_on=1-shift_on;
	}
	else if (scancode == CAPSLOCK_PRESS){
		capslock_on = 1 - capslock_on;
	}
}

void print_character(uint8_t scancode){
	char letter;
	if ((is_letter(scancode) && !capslock_and_shift_both_on_or_off()) || (!is_letter(scancode) && shift_on)){
		letter = sc_ascii_capital[(int)scancode];
	}
	else if ((is_letter(scancode) && capslock_and_shift_both_on_or_off())|| (!is_letter(scancode)
			&& !shift_on)){
		letter = sc_ascii_low[(int) scancode];
	}
	if (strlen(key_buffer)>KEY_BUFFER_SIZE-1){
		key_buffer[0]='\0';
	}
	char str[2] = {letter, '\0'};
	append(key_buffer, letter);
	print_string(str);
}

void handle_other_keys(uint8_t scancode){
	if (scancode == BACKSPACE) {
		backspace(key_buffer);
		print_backspace();
	}
	else if (scancode == ENTER) {
		println("");
		user_input(key_buffer);
		key_buffer[0] = '\0';
	}
	else if (scancode == TAB){
		print_tab();
	}
	else if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT || scancode == ESCAPE
			|| scancode == LEFT_CTRL || scancode == ALT || scancode == CAPSLOCK_PRESS){
	}
	else {
		print_character(scancode);
	}
}

boolean was_key_released(uint8_t scancode){
	return scancode & 0x80;
}

void handle_control_keys (uint8_t scancode){
	switch (scancode){
		case 0x35:
			print_string("/");
	}
	UNUSED(scancode);
}

static void keyboard_callback(registers_t* regs) {
		uint8_t scancode = port_byte_in(0x60);

	    handle_shift_and_capslock(scancode);
	    if (was_key_released(scancode)){
	    	previous_key = scancode;
	    	return;
	    }
	    if (previous_key == ESCAPE_CODE){
	    	handle_control_keys (scancode);
	    }
	    else{
	    	handle_other_keys(scancode);
	    }
	    previous_key = scancode;

	    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback);
}


