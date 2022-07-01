#include <kernel/paging.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/physical_memory_manager.h>
#include <string.h>
#include <kernel/math.h>
#include <stdio.h>

uint32_t* current_directory = 0;

extern void load_page_directory(uint32_t*);
extern void enable_paging();
extern void flush_tlb_entry(uint32_t*);

bool allocate_page (uint32_t pt_entry) {
 
	void* block = allocate_block ();
	if (!block)
		return false;
 
	pt_entry= SET_FRAME (pt_entry, (physical_address)block);
	pt_entry= SET_BIT (pt_entry, IS_PRESENT);
 
	return true;
}

void free_page (uint32_t pt_entry) {
 
	void* frame_address = (void*)GET_FRAME (pt_entry);
	if (frame_address)
		free_block (frame_address);
 
	pt_entry= UNSET_BIT (pt_entry, IS_PRESENT);
}

inline uint32_t get_page_table_entry_from_address (uint32_t* page_table,virtual_address address) {
 
	if (page_table)
		return page_table[ PAGE_TABLE_INDEX (address) ];
	return 0;
}

inline uint32_t get_page_directory_entry_from_address (uint32_t* page_directory, virtual_address addr) {
 
	if (page_directory)
		return page_directory[ PAGE_DIRECTORY_INDEX (addr) ];
	return 0;
}



bool switch_page_directory (uint32_t* directory) {
 
	if (!directory)
		return false;
 
	current_directory = directory;
	load_page_directory (directory);
	return true;
}


 
uint32_t* get_directory () {
 
	return current_directory;
}


void map_page (void* physical_addr, void* virtual_address) {


   pd_entry entry = current_directory[PAGE_DIRECTORY_INDEX ((uint32_t) virtual_address) ];
   if ( !IS_BIT_SET(entry, IS_PRESENT)) {
        uint32_t* table = (uint32_t*) allocate_block();
        if (!table){
            return;
        }

        memset (table, 0, PAGES_PER_TABLE * sizeof(uint32_t));
        entry = SET_BIT(entry, IS_PRESENT);
        entry = SET_BIT(entry, IS_WRITABLE);
        entry = SET_FRAME(entry, (physical_address) table);

   }
    uint32_t* page_table = (uint32_t*) GET_FRAME ( entry );
    pt_entry page = page_table [ PAGE_TABLE_INDEX ( (uint32_t) virtual_address) ];
    page = SET_FRAME(page, (physical_address)physical_addr);
    page = SET_BIT(page, IS_PRESENT);
}

void set_up_paging () {
 
	uint32_t* default_page_table = (uint32_t*) allocate_block ();
	if (!default_page_table)
		return;
 
	uint32_t* kernel_page_table = (uint32_t*) allocate_block ();
	if (!kernel_page_table)
		return;

	memset(default_page_table, 0, PAGES_PER_TABLE*4);
	memset(kernel_page_table, 0, PAGES_PER_TABLE*4);

    for (uint32_t i=0, frame=0x0, virtual=0x00000000; i<1024; i++, frame+=4096, virtual+=4096) {

		pt_entry page=0;
		page = SET_BIT (page, IS_PRESENT);
 		page = SET_FRAME (page, frame);

		default_page_table[PAGE_TABLE_INDEX (virtual) ] = page;
	}

    for (uint32_t i=0, frame=0x100000, virtual=0xc0000000; i<1024; i++, frame+=4096, virtual+=4096) {

		pt_entry page=0;
		page = SET_BIT (page, IS_PRESENT);
		page = SET_FRAME (page, frame);

		kernel_page_table[PAGE_TABLE_INDEX (virtual) ] = page;
	}
    uint32_t* page_dir = (uint32_t*) allocate_blocks(divide_round_up(4 * PAGE_TABLES_PER_DIRECTORY, BLOCKS_SIZE));
    if (!page_dir){
        return;
    }
    memset(page_dir, 0, PAGE_TABLES_PER_DIRECTORY*4);

    uint32_t pd_entry = 0;
	pd_entry = SET_BIT (pd_entry, IS_PRESENT);
	pd_entry= SET_BIT (pd_entry, IS_WRITABLE);
	pd_entry = SET_FRAME (pd_entry, (physical_address)page_dir);
	page_dir[1023]= pd_entry;
	printf(" pd entry address: %d ", GET_FRAME(pd_entry));

	uint32_t default_entry = 0;
	default_entry = SET_BIT (default_entry, IS_PRESENT);
	default_entry= SET_BIT (default_entry, IS_WRITABLE);
	default_entry = SET_FRAME (default_entry, (physical_address)default_page_table);
	page_dir[PAGE_DIRECTORY_INDEX(0) ] = default_entry;
	printf(" default entry address: %d", GET_FRAME(default_entry));


	uint32_t kernel_entry = 0;
	kernel_entry = SET_BIT (kernel_entry, IS_PRESENT);
	kernel_entry= SET_BIT (kernel_entry, IS_WRITABLE);
	kernel_entry = SET_FRAME (kernel_entry, (physical_address)kernel_page_table);
	page_dir[PAGE_DIRECTORY_INDEX(0xc0000000)] = kernel_entry;
	printf(" kernel entry address: %d", GET_FRAME(kernel_entry));

	switch_page_directory (page_dir);
 
	enable_paging ();


}

