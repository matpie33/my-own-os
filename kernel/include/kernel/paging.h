#ifndef PAGE_TABLE_ENTRY
#define PAGE_TABLE_ENTRY

#include <stdint.h>
#include <stdbool.h>

#define PAGES_PER_TABLE 1024
#define PAGE_TABLES_PER_DIRECTORY	1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)	
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)

#define PAGE_TABLE_ADDRESS_SPACE_SIZE 0x400000

#define DIRECTORY_TABLE_ADDRESS_SPACE_SIZE 0x100000000

#define SET_BIT(pte,bit) ((pte) | (1 << (bit) ))
#define UNSET_BIT(pte,bit) ((pte) & ~(1 << (bit) ))
#define IS_BIT_SET(pte,bit) ((pte) & (1 <<(bit)))
#define SET_FRAME(pte,address) ((pte) | (address) )
#define GET_FRAME(pte) ((pte) & 0xFFFFF000)

#define RECURSIVELY_MAPPED_PAGE_DIRECTORY_INDEX 1023

#define VIRTUAL_ADDRESS_OF_PAGE_TABLE_0 0xffc00000


#define PAGE_SIZE 4096

typedef uint32_t pt_entry;
typedef uint32_t pd_entry;

typedef uint32_t virtual_address;
typedef uint32_t physical_address;

enum PTE_BIT_NUMBERS {
 
	IS_PRESENT		=	0,		
	IS_WRITABLE		=	1,		
	USER_MODE		=	2,		
	WRITETHOUGH		=	3,		
	NOT_CACHEABLE	=	4,		
	IS_ACCESSED		=	5,		
	IS_DIRTY		=	6,		
	PAT			    =	7,		
	CPU_GLOBAL		=	8,		
	LV4_GLOBAL		=	9,		
};
uint32_t get_frame (uint32_t pte);
void set_up_paging ();
void map_page (uint32_t physical_addr, uint32_t virtual_address) ;

#endif