#include "screen.h"
#include "low_level.h"

int get_cursor () {
	/** The device uses its control register as an index
	*   to select its internal registers , of which we are
	*   interested in:
	*   reg 14: which is the high byte of the cursor 's offset
	*   reg 15: which is the low byte of the cursor 's offset
	
	*   Once the internal register has been selected , we may read or
	*   write a byte on the data register .
	*/ 
	
	port_byte_out ( REG_SCREEN_CTRL , 14);
	int offset = port_byte_in ( REG_SCREEN_DATA ) << 8;
	port_byte_out ( REG_SCREEN_CTRL , 15);
	offset += port_byte_in ( REG_SCREEN_DATA );
	
	/**
	*   Since the cursor offset reported by the VGA hardware is the
	*   number of characters , we multiply by two to convert it to
	*   a character cell offset .
	*/ 
	return offset *2;
}

void set_cursor (int offset ) {
	
	offset /= 2; 
	
	/** Convert from cell offset to char offset .
	*   This is similar to get_cursor , only now we write
	*   bytes to those internal device registers .
	*/
	
	port_byte_out ( REG_SCREEN_CTRL , 14);
	port_byte_out ( REG_SCREEN_DATA , ( unsigned char )( offset >> 8));
	port_byte_out ( REG_SCREEN_CTRL , 15);
	port_byte_out ( REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int print_char ( char character , int col , int row , char attribute_byte ) {
	
	unsigned char *vidmem = ( unsigned char*) VIDEO_ADDRESS ;

	/* If attribute byte is zero , assume the default style . */
		if (! attribute_byte ) {
			attribute_byte = WHITE_ON_BLACK ;
		}
	int offset ;
		if ( col >= 0 && row >= 0) {
			offset = get_screen_offset (row,col );
		} 
		else {
			offset = get_cursor ();
		}

	/** If we see a newline character , set offset to the end of
	 *  current row , so it will be advanced to the first col
	 *  of the next row.
	 */
	 
		if ( character == '\n') {	
			int rows = offset / (2* MAX_COLS );
			offset = get_screen_offset (rows, 79);
		} 
		else {
			vidmem [ offset ] = character ;
			vidmem [ offset +1] = attribute_byte ;
		}
	offset += 2;
	offset = handle_scrolling ( offset );
	 
	set_cursor ( offset );
	return offset;
}

int get_screen_offset(int row, int column){
	
	return (MAX_COLS*row+column)*2;
}

void print_at ( char* message , int col , int row ) {
	
int i=0;   
   while (message[i]!=0){
	   print_char(message[i],col,row,WHITE_ON_BLACK);
	   i++;	   
   }   	
}

int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }


void print ( char * message ) {
	
	print_at ( message , -1, -1);
}

void println(char * message){	
	print_at(message, -1, -1);
	print_at("\n", -1, -1);
}

void clear_screen () {
	
int row = 0;
int col = 0;
	for (row =0; row < MAX_ROWS ; row ++) {
		for (col =0; col < MAX_COLS ; col ++) {
			print_char (' ', col,row , 0x0f );
		}
	}
set_cursor ( get_screen_offset (0, 0));
}

int handle_scrolling ( int cursor_offset ) {
	
if ( cursor_offset < MAX_ROWS * MAX_COLS *2) {
	return cursor_offset ;
}
/* Shuffle the rows back one . */
int i;
for (i =1; i< MAX_ROWS ; i ++) {
	memory_copy ( get_screen_offset (i, 0) + VIDEO_ADDRESS ,
	get_screen_offset (i -1, 0) + VIDEO_ADDRESS ,
	MAX_COLS *2
	);
}
/* Blank the last line by setting all bytes to 0 */
char* last_line = (char*)(get_screen_offset (MAX_ROWS -1, 0) + VIDEO_ADDRESS) ;
for (i =0; i < MAX_COLS *2; i++) {
	last_line [i] = 0;
}
// Move the offset back one row , such that it is now on the last
// row , rather than off the edge of the screen .
cursor_offset -= 2* MAX_COLS ;
// Return the updated cursor position .
return cursor_offset ;
}


