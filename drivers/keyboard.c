#include "keyboard.h"
#include "../cpu/isr.h"
#include "../cpu/port_read_write.h"
#include "../graphics/draw_string.h"
#include "../libc/function.h"
#include "../libc/strings.h"
#include "../cpu/port_read_write.h"
#include "../kernel/kernel.h"

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

#define SC_MAX 57

static char key_buffer[256];

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
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' ', 'Q'};

const char sc_ascii_low[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' ', '?'};

static int shift_on = 0; // 0 is low, 1 is high
static int capslock_on = 0;

boolean capslock_and_shift_both_on_or_off (){
	return (shift_on && capslock_on) || (!shift_on && !capslock_on);
}

boolean is_letter(u8 scancode){
	return (scancode <=0X19 && scancode>=0X10)
			|| (scancode <=0X26 && scancode>=0x1E) || (scancode<=0X32 && scancode >=0x2C);
}

static void keyboard_callback(registers_t* regs) {
	/* The PIC leaves us the scancode in port 0x60 */
	    u8 scancode = port_byte_in(0x60);

	    //TODO implement numpad
	    if (scancode == LEFT_SHIFT){
			shift_on=1-shift_on;
		}
		else if (scancode == SHIFT_RELEASE){
			shift_on=1-shift_on;
		}
		else if (scancode == CAPSLOCK_PRESS){
			capslock_on = 1 - capslock_on;
		}

	    if (scancode > SC_MAX) return;
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
	    		|| scancode == LEFT_CTRL || scancode == ALT){
	    }

	    else {
	    	char letter;
	    	if ((is_letter(scancode) && !capslock_and_shift_both_on_or_off()) || (!is_letter(scancode) && shift_on)){
	    		letter = sc_ascii_capital[(int)scancode];
	    	}
	    	else if ((is_letter(scancode) && capslock_and_shift_both_on_or_off())|| (!is_letter(scancode)
	    			&& !shift_on)){
	    		letter = sc_ascii_low[(int) scancode];
	    	}
	    	if (strlen(key_buffer)>255){
				key_buffer[0]='\0';
	    	}
	        char str[2] = {letter, '\0'};
	        append(key_buffer, letter);
	        print_string(str);
	    }
	    UNUSED(regs);
}



void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback);
}


