#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "system.h"
#include "address.h"
#include "physical_memory.h"
#include "tlb.h"

typedef struct {
    page_t  page_num;
    frame_t frame_num;
    bool    valid;      /* indicate if this entry is valid */
} page_table_entry_t;

typedef struct {
    page_table_entry_t page_table_entry[NUM_PAGES];
    u_int_t       next_page_table_ptr;  /* next candidate entry to be replaced/used */ 
} page_table_t;

int page_table_init(page_table_t* page_table);
int search_page_table(page_t page_num, page_table_t page_table, 
						bool* is_page_fault, frame_t* frame_num);
int insert_page_table(page_t page_num, frame_t* frame_num, page_table_t* page_table);
int page_fault_handler(page_t page_num, frame_t* frame_num, physical_memory_t* physical_memory, 
						page_table_t* page_table, tlb_t* tlb); //NOPE (LAST)
#endif
