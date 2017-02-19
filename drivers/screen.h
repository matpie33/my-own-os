#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f

//Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
 
void print ( char * message );
int get_screen_offset(int row, int column);
int get_cursor ();
void set_cursor (int offset );
void print_at ( char* message , int col , int row );
int print_char ( char character , int col , int row , char attribute_byte );
void println(char * message);
