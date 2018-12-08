#include <stdio.h>
#include <limits.h>
#include "system.h"
#include "address.h"
#include "page_table.h"
#include "tlb.h"
#include "physical_memory.h"
//#define DEBUG

int page_table_init(page_table_t* page_table) {
	for (int i = 0; i < NUM_PAGES; i++) {
		page_table_entry_t pte = {0, 0, FALSE}; //Initialize to invalid
		page_table->page_table_entry[i] = pte;
	}
	return 0;
}

int search_page_table(page_t page_num, page_table_t page_table, 
						bool* is_page_fault, frame_t* frame_num) {
	*is_page_fault = TRUE;
	for (int i = 0; i < NUM_PAGES; i++) {
		page_table_entry_t pte = page_table.page_table_entry[i];
		/* check for page_num in every page_table_entry */
		if (pte.page_num == page_num && pte.valid == TRUE) {
			*is_page_fault = FALSE;
			*frame_num = pte.frame_num;
		}
	}
	return 0;
}

/* 
 * Handling a page fault: Load a 256-byte page from backing_store             
 * into the simulated main memory.             
 * Attention: You need to call the page_table_update() and              
 * TLB_replacement_LRU() functions to implement the following              
 * page_fault_handler() function, where both page_table and              
 * sys_tlb must be updated.              
 * The prototype has been changed by adding frame_number as              
 * an input parameter.              
*/
int page_fault_handler(frame_t frame_num, physical_memory_t* physical_memory, 
						page_table_t* page_table, tlb_t* tlb) {
	// Pull page from backing store into physical memory
	load_frame_to_physical_memory(frame_num, *physical_memory);
	//Update the page table

	//Update the tlb
	return 0;
}