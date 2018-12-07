// SHOULD THIS ALSO HAVE BACKING STORE STUFF?
#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include "system.h"
#include "address.h"
#include "page_table.h"

// MAKE IT STATIC?
typedef byte physical_memory_t[PAGE_SIZE*NUM_PAGES];
typedef byte value_t; //???

int read_physical_memory(paddress_t physical_address, physical_memory_t physical_memory, value_t* value); //NOPE
int load_frame_to_physical_memory(page_t* page_num, char* backing_store_file_name, 
							paddress_t* physical_address, frame_t* frame_num); //NOPE

#endif