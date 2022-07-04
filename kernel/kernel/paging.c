#include <kernel/paging.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/physical_memory_manager.h>
#include <string.h>
#include <kernel/math.h>
#include <stdio.h>
#include <kernel/memory_detecting.h>

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

void allocate_page_for_table (uint32_t table_virtual_address){
	uint32_t table_physical_address = (uint32_t) allocate_block();
	pt_entry page_directory_entry_for_table_address = current_directory[PAGE_TABLE_INDEX(table_virtual_address)];
	page_directory_entry_for_table_address = SET_FRAME(page_directory_entry_for_table_address, (physical_address)table_physical_address);
	page_directory_entry_for_table_address = SET_BIT(page_directory_entry_for_table_address, IS_PRESENT);
	current_directory[PAGE_TABLE_INDEX(table_virtual_address)] = page_directory_entry_for_table_address;
	flush_tlb_entry((uint32_t*)table_virtual_address);
}


void map_page (uint32_t physical_addr, uint32_t virtual_address) {


	pd_entry page_directory_entry = current_directory[PAGE_DIRECTORY_INDEX (virtual_address) ];
	uint32_t table_virtual_address = VIRTUAL_ADDRESS_OF_PAGE_TABLE_0 + PAGE_DIRECTORY_INDEX( virtual_address)*PAGE_SIZE;
	if (!IS_BIT_SET(page_directory_entry, IS_PRESENT)){
		allocate_page_for_table(table_virtual_address);
	}
	
	uint32_t* table = (uint32_t*)table_virtual_address ;
	pt_entry table_entry = table[ PAGE_TABLE_INDEX ( (uint32_t) virtual_address) ];
	table_entry = SET_FRAME(table_entry, (physical_address)physical_addr);
	table_entry = SET_BIT(table_entry, IS_PRESENT);
	table[PAGE_TABLE_INDEX (virtual_address)] = table_entry;
	flush_tlb_entry((uint32_t*)virtual_address);
}

uint32_t* allocate_block_for_page_table (){
	uint32_t* page_table = (uint32_t*) allocate_block();
	if (!page_table){
		return 0;
	}
    memset(page_table, 0, PAGES_PER_TABLE* sizeof(uint32_t));
	return page_table;
}

void map_range_of_virtual_to_physical_addresses (uint32_t physical_address, uint32_t virtual_address, uint32_t* table){
	for (uint32_t i=0; i<PAGES_PER_TABLE; i++) {

		pt_entry table_entry=0;
		table_entry = SET_BIT (table_entry, IS_PRESENT);
 		table_entry = SET_FRAME (table_entry, physical_address);

		table[PAGE_TABLE_INDEX (virtual_address) ] = table_entry;
		physical_address += BLOCKS_SIZE;
		virtual_address += BLOCKS_SIZE;
	}
}

void map_pd_entry_to_page_table (uint32_t* table_address, uint32_t page_directory_index, uint32_t* page_directory){
	uint32_t pd_entry = 0;
	pd_entry = SET_BIT (pd_entry, IS_PRESENT);
	pd_entry= SET_BIT (pd_entry, IS_WRITABLE);
	pd_entry = SET_FRAME (pd_entry, (physical_address)table_address);
	page_directory[page_directory_index]= pd_entry;
}

void set_up_paging () {

    uint32_t* page_dir = (uint32_t*) allocate_blocks(divide_round_up(sizeof(uint32_t) * PAGE_TABLES_PER_DIRECTORY, BLOCKS_SIZE));
    if (!page_dir){
        return;
    }
    memset(page_dir, 0, PAGE_TABLES_PER_DIRECTORY*4);

    uint32_t* default_page_table = allocate_block_for_page_table();
    uint32_t* kernel_page_table = allocate_block_for_page_table();
 
	map_range_of_virtual_to_physical_addresses(0, 0, default_page_table);
	map_range_of_virtual_to_physical_addresses(KERNEL_LOCATION_START, KERNEL_VIRTUAL_ADDRESS_START, kernel_page_table);  
    
	map_pd_entry_to_page_table (page_dir, RECURSIVELY_MAPPED_PAGE_DIRECTORY_INDEX, page_dir);
	map_pd_entry_to_page_table (default_page_table, 0, page_dir);
	map_pd_entry_to_page_table (kernel_page_table, PAGE_DIRECTORY_INDEX(KERNEL_VIRTUAL_ADDRESS_START), page_dir);

	switch_page_directory (page_dir);
 
	enable_paging ();


}

