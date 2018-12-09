/*
 * Project 6: physical_memory.h
 * This is a header file of physical_memory.c
 * Turner Atwood
 */

#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include "system.h"
#include "address.h"
#include "page_table.h"

typedef byte physical_memory_t[PAGE_SIZE*NUM_PAGES];
typedef byte value_t;

/* Reading from memory and from the backingstore to memory */
int init_physical_memory(physical_memory_t physical_memory);
int read_physical_memory(paddress_t physical_address, physical_memory_t physical_memory, 
							value_t* value);
int load_frame_to_physical_memory(page_t page_num, frame_t frame_num, physical_memory_t physical_memory);
#endif