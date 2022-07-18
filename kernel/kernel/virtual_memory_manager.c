#include <kernel/virtual_memory_manager.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/physical_memory_manager.h>
#include <string.h>
#include <kernel/math.h>
#include <stdio.h>
#include <kernel/memory_detecting.h>
#include <kernel/heap.h>
#include <kernel/buddy_alocator.h>

uint32_t *current_directory = 0;

extern void load_page_directory(uint32_t *);
extern void enable_paging();
extern void flush_tlb_entry(uint32_t *);

free_pages_region_info_t *address_of_first_free_region_kernelspace;
uint32_t next_free_address_for_heap = KERNEL_VIRTUAL_ADDRESS_START + KERNEL_IMAGE_SIZE;

virtual_address next_free_virtual_address;

void allocate_page_for_table(uint32_t table_virtual_address)
{
	physical_address table_physical_address = (physical_address)allocate_block();
	pt_entry page_directory_entry_for_table_address = current_directory[PAGE_TABLE_INDEX(table_virtual_address)];
	page_directory_entry_for_table_address = SET_FRAME(page_directory_entry_for_table_address, table_physical_address);
	page_directory_entry_for_table_address = SET_BIT(page_directory_entry_for_table_address, IS_PRESENT);
	current_directory[PAGE_TABLE_INDEX(table_virtual_address)] = page_directory_entry_for_table_address;
	flush_tlb_entry((uint32_t *)table_virtual_address);
}

void map_page(physical_address physical_addr, virtual_address virtual_addr)
{

	pd_entry page_directory_entry = current_directory[PAGE_DIRECTORY_INDEX(virtual_addr)];
	virtual_address table_virtual_address = VIRTUAL_ADDRESS_OF_PAGE_TABLE_0 + PAGE_DIRECTORY_INDEX(virtual_addr) * PAGE_SIZE;
	if (!IS_BIT_SET(page_directory_entry, IS_PRESENT))
	{
		allocate_page_for_table(table_virtual_address);
	}

	uint32_t *table = (uint32_t *)table_virtual_address;
	pt_entry table_entry = table[PAGE_TABLE_INDEX((uint32_t)virtual_addr)];
	table_entry = SET_FRAME(table_entry, physical_addr);
	table_entry = SET_BIT(table_entry, IS_PRESENT);
	table[PAGE_TABLE_INDEX(virtual_addr)] = table_entry;
	flush_tlb_entry((uint32_t *)virtual_addr);
}

uint32_t *allocate_pages(uint32_t number_of_pages)
{

	uint32_t *virtual_address;
	free_pages_region_info_t *address_of_free_space = address_of_first_free_region_kernelspace;
	while (address_of_free_space)
	{
		if (address_of_free_space->number_of_pages > number_of_pages)
		{
			physical_address first_block_address = (physical_address)allocate_blocks(number_of_pages);
			virtual_address = (uint32_t *)address_of_free_space->address;
			for (uint32_t j = 0; j < number_of_pages; j++)
			{
				map_page(first_block_address + j * BLOCK_SIZE, address_of_free_space->address + j * PAGE_SIZE);
			}
			address_of_free_space->address += number_of_pages * PAGE_SIZE;
			address_of_free_space->number_of_pages -= number_of_pages;
			printf("addresses now: address %d, number of pages: %d", address_of_free_space->address, address_of_free_space->number_of_pages);
			return virtual_address;
		}
		address_of_free_space = address_of_free_space->address_of_next_region_info;
	}

	return 0;
}

void unmap_page(virtual_address virtual_address)
{
	uint32_t *table = (uint32_t *)(VIRTUAL_ADDRESS_OF_PAGE_TABLE_0 + PAGE_DIRECTORY_INDEX(virtual_address) * PAGE_SIZE);
	physical_address frame_address = GET_FRAME(table[PAGE_TABLE_INDEX(virtual_address)]);
	printf("freeing block: %d", frame_address);
	free_block((void *)frame_address);
	table[PAGE_TABLE_INDEX(virtual_address)] = 0;
}

void free_page(uint32_t *virtual_address, uint32_t number_of_pages)
{

	for (uint32_t i = 0; i < number_of_pages; i++)
	{
		unmap_page((uint32_t)virtual_address + PAGE_SIZE * i);
	}
}

bool switch_page_directory(uint32_t *directory)
{

	if (!directory)
		return false;

	current_directory = directory;
	load_page_directory(directory);
	return true;
}

uint32_t *allocate_block_for_page_table()
{
	uint32_t *page_table = (uint32_t *)allocate_block();
	if (!page_table)
	{
		return 0;
	}
	memset(page_table, 0, PAGES_PER_TABLE * sizeof(uint32_t));
	return page_table;
}

void identity_map_first_4_megabyte(physical_address physical_address, virtual_address virtual_address, uint32_t *table)
{
	for (uint32_t i = 0; i < PAGES_PER_TABLE; i++)
	{

		pt_entry table_entry = 0;
		table_entry = SET_BIT(table_entry, IS_PRESENT);
		table_entry = SET_FRAME(table_entry, physical_address);

		table[PAGE_TABLE_INDEX(virtual_address)] = table_entry;
		physical_address += BLOCK_SIZE;
		virtual_address += BLOCK_SIZE;
	}
}

void map_pd_entry_to_page_table(uint32_t *table_address, uint32_t page_directory_index, uint32_t *page_directory)
{
	uint32_t pd_entry = 0;
	pd_entry = SET_BIT(pd_entry, IS_PRESENT);
	pd_entry = SET_BIT(pd_entry, IS_WRITABLE);
	pd_entry = SET_FRAME(pd_entry, (physical_address)table_address);
	page_directory[page_directory_index] = pd_entry;
}

void map_kernel_to_3_gb(physical_address physical_address, virtual_address virtual_address, uint32_t *page_dir)
{
	for (uint32_t i = 0; i < KERNEL_IMAGE_SIZE / (PAGES_PER_TABLE * PAGE_SIZE); i++)
	{
		uint32_t *table_physical_and_virtual_address = allocate_block_for_page_table();
		for (uint32_t i = 0; i < PAGES_PER_TABLE; i++)
		{

			pt_entry table_entry = 0;
			table_entry = SET_BIT(table_entry, IS_PRESENT);
			table_entry = SET_FRAME(table_entry, physical_address);

			table_physical_and_virtual_address[PAGE_TABLE_INDEX(virtual_address)] = table_entry;
			physical_address += BLOCK_SIZE;
			virtual_address += BLOCK_SIZE;
		}
		map_pd_entry_to_page_table(table_physical_and_virtual_address, PAGE_DIRECTORY_INDEX(KERNEL_VIRTUAL_ADDRESS_START + BLOCK_SIZE * PAGES_PER_TABLE * i), page_dir);
	}
}

free_region_info_t *allocate_pages_for_heap(uint32_t number_of_pages)
{
	if (next_free_address_for_heap + number_of_pages * PAGE_SIZE >= VIRTUAL_MEMORY_LIMIT_32_BIT)
	{
		return 0;
	}
	for (uint32_t i = 0; i < number_of_pages; i++)
	{
		uint32_t free_block = (uint32_t)allocate_block();
		map_page(free_block, next_free_address_for_heap + i * PAGE_SIZE);
	}
	free_region_info_t *free_region_info = (free_region_info_t *)next_free_address_for_heap;
	size_t size_of_info = sizeof(free_region_info_t);
	memset(free_region_info, 0, size_of_info);
	free_region_info->address = next_free_address_for_heap + size_of_info;
	free_region_info->length = number_of_pages * PAGE_SIZE - size_of_info;

	printf("address of heap: %d", next_free_address_for_heap);
	next_free_address_for_heap += number_of_pages * PAGE_SIZE;
	return free_region_info;
}

void initialize_addresses_list()
{
	physical_address physical_address = (uint32_t)allocate_block();
	map_page(physical_address, FREE_VIRTUAL_REGIONS_STRUCT_ADDRESS);
	address_of_first_free_region_kernelspace = (free_pages_region_info_t *)FREE_VIRTUAL_REGIONS_STRUCT_ADDRESS;
	address_of_first_free_region_kernelspace->address = FREE_VIRTUAL_REGIONS_STRUCT_ADDRESS + PAGE_SIZE;
	address_of_first_free_region_kernelspace->number_of_pages = (KERNEL_VIRTUAL_ADDRESS_START - address_of_first_free_region_kernelspace->address) / PAGE_SIZE;
	address_of_first_free_region_kernelspace->address_of_next_region_info = 0;
	printf("addresses at start: address %d, number of pages: %d", address_of_first_free_region_kernelspace->address, address_of_first_free_region_kernelspace->number_of_pages);
}

void set_up_paging()
{

	uint32_t *page_dir = (uint32_t *)allocate_blocks(divide_round_up(sizeof(uint32_t) * PAGE_TABLES_PER_DIRECTORY, BLOCK_SIZE));
	if (!page_dir)
	{
		return;
	}
	memset(page_dir, 0, PAGE_TABLES_PER_DIRECTORY * sizeof(uint32_t));

	uint32_t *identity_map_page_table = allocate_block_for_page_table();

	identity_map_first_4_megabyte(0, 0, identity_map_page_table);

	map_pd_entry_to_page_table(page_dir, RECURSIVELY_MAPPED_PAGE_DIRECTORY_INDEX, page_dir);
	map_pd_entry_to_page_table(identity_map_page_table, 0, page_dir);

	map_kernel_to_3_gb(KERNEL_LOCATION_START, KERNEL_VIRTUAL_ADDRESS_START, page_dir);

	switch_page_directory(page_dir);

	enable_paging();
	initialize_addresses_list();
	virtual_address virtual_addresses_start = FREE_VIRTUAL_REGIONS_STRUCT_ADDRESS + PAGE_SIZE;
	virtual_address address_for_buddy_region_infos = virtual_addresses_start + power(2, log2(KERNEL_VIRTUAL_ADDRESS_START - virtual_addresses_start));
	physical_address physical_block = (physical_address)allocate_block();

	map_page(physical_block, address_for_buddy_region_infos);
	initialize_buddy_blocks(KERNEL_VIRTUAL_ADDRESS_START, address_for_buddy_region_infos);
	next_free_virtual_address = address_for_buddy_region_infos + PAGE_SIZE;
}

virtual_address allocate_page()
{
	void *physical_block = allocate_block();
	if (!physical_block || next_free_virtual_address == KERNEL_VIRTUAL_ADDRESS_START)
	{
		return 0;
	}
	virtual_address virtual_addr = next_free_virtual_address;
	map_page((physical_address)physical_block, virtual_addr);
	next_free_virtual_address += PAGE_SIZE;
	return virtual_addr;
}
