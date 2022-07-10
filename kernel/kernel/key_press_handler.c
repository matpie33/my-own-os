#include <stdio.h>
#include <stdbool.h>
#include <kernel/key_press_handler.h>
#include <kernel/special_keys.h>
#include <stdint.h>

bool display_characters_on_screen;

static bool shift_on = KEY_NOT_PRESSED;
static bool capslock_on = KEY_NOT_PRESSED;

const char ascii_characters_capital[] = {'?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(',
										 ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{',
										 '}', '?', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '`', '?',
										 '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?', '*', '?', ' ', '?',
										 ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '7', '8', '9', '-',
										 '4', '5', '6', '+', '1', '2', '3', '0', ' ', ' ', ' ', ' ', ' ', ' '};

const char ascii_characters_lowercase[] = {'?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
										   '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
										   '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\',
										   'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '*', '?', ' ', '?', ' ',
										   ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '7', '8', '9', '-', '4',
										   '5', '6', '+', '1', '2', '3', '0', ' ', ' ', ' ', ' ', ' ', ' '};

bool capslock_and_shift_both_on_or_off()
{
	return (shift_on && capslock_on) || (!shift_on && !capslock_on);
}

void set_display_pressed_keys_on_screen(bool display)
{
	display_characters_on_screen = display;
}

bool is_letter(uint8_t scancode)
{
	return (scancode <= 0X19 && scancode >= 0X10) || (scancode <= 0X26 && scancode >= 0x1E) || (scancode <= 0X32 && scancode >= 0x2C);
}

char convert_to_character(uint8_t scancode)
{
	char letter;
	if ((is_letter(scancode) && !capslock_and_shift_both_on_or_off()) || (!is_letter(scancode) && shift_on))
	{
		letter = ascii_characters_capital[(int)scancode];
	}
	else if ((is_letter(scancode) && capslock_and_shift_both_on_or_off()) || (!is_letter(scancode) && !shift_on))
	{
		letter = ascii_characters_lowercase[(int)scancode];
	}

	return letter;
}

bool was_key_released(uint8_t scancode)
{
	return scancode & 0x80;
}

void handle_displayable_key(uint8_t scancode)
{
	if (display_characters_on_screen)
	{
		if (was_key_released(scancode))
		{
			return;
		}
		char character = convert_to_character(scancode);
		char char_sequence[2] = {character, '\0'};
		printf(char_sequence);
	}
}

bool is_displayable_character(uint8_t scancode)
{
	return (scancode >= 0x02 && scancode <= 0x0D) || (scancode >= 0x10 && scancode <= 0x1B) || (scancode >= 0x1E && scancode <= 0x29) || (scancode >= 0x2B && scancode <= 0x35) || scancode == 0x37 || scancode == 0x39 || (scancode >= 0x47 && scancode <= 0x53);
}

void handle_special_key(uint8_t scancode)
{

	switch (scancode)
	{
	case TAB:
		break;
	case LEFT_SHIFT_RELEASE:
		shift_on = false;
		break;
	case CAPS_LOCK:
		capslock_on = !capslock_on;
		break;
	case LEFT_SHIFT:
		shift_on = true;
		break;
	case LEFT_CONTROL:
		break;
	case LEFT_ALT:
		break;
	case RIGHT_SHIFT:
		break;
	case RIGHT_CONTROL:
		break;
	case RIGHT_ALT:
		break;
	case ENTER:
		break;
	case BACKSPACE:
		break;
	case F1:
		break;
	case F2:
		break;
	case F3:
		break;
	case F4:
		break;
	case F5:
		break;
	case F6:
		break;
	case F7:
		break;
	case F8:
		break;
	case F9:
		break;
	case F10:
		break;
	case F11:
		break;
	case F12:
		break;
	case ESC:
		break;
	}
}

void handle_key(uint8_t scancode)
{

	if (is_displayable_character(scancode))
	{
		handle_displayable_key(scancode);
	}
	else
	{
		handle_special_key(scancode);
	}
}