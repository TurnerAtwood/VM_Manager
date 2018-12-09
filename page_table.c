/*
 * Project 6: page_table.c
 * Turner Atwood
 */

#include <stdio.h>
#include <limits.h>
#include "system.h"
#include "address.h"
#include "page_table.h"
#include "tlb.h"
#include "physical_memory.h"

int page_table_init(page_table_t* page_table) {
	for (int i = 0; i < NUM_PAGES; i++) {
		page_table_entry_t pte = {0, 0, FALSE}; //Initialize to invalid
		page_table->page_table_entry[i] = pte;
	}
	page_table->next_page_table_ptr = 0;
	return 0;
}

/* Search for a page num in the page table */
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

/* Inserts a new pte into page_table and increments the p_t pointer */
int insert_page_table(page_t page_num, frame_t* frame_num, page_table_t* page_table) {
	// Assign the page to the next available frame
	*frame_num = page_table->next_page_table_ptr;

	// Create a new pte and insert it in the page table
	page_table_entry_t pte = {page_num,*frame_num,TRUE};
	page_table->page_table_entry[page_table->next_page_table_ptr] = pte;

	// Increment p_t next pointer (Loop around to 0 if off the end)
	page_table->next_page_table_ptr = (page_table->next_page_table_ptr+1)%NUM_PAGES;
	return 0;
}


/* Handles a page fault: Loads a page to memory, inserts it in the page table and  TLB */
/* Only called when the provided frame cannot be found */
int page_fault_handler(page_t page_num, frame_t* frame_num, physical_memory_t* physical_memory, 
						page_table_t* page_table, tlb_t* tlb, int strategy) {
	//Update the page table (get a frame_num for the page)
	insert_page_table(page_num, frame_num, page_table);

	// Pull page from backing store into physical memory (at frame_num)
	load_frame_to_physical_memory(page_num, *frame_num, *physical_memory);

	//Update the tlb
	if (strategy == 1) {
    	tlb_replacement_FIFO(page_num, *frame_num, tlb);
    }
    else {
        tlb_replacement_LRU(page_num, *frame_num, tlb);
    }
	return 0;
}