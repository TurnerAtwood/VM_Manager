/*
 * Project 6: physical_memory.c
 * Turner Atwood
 */

#include <stdio.h>
#include "system.h"
#include "tlb.h"
#include "address.h"
#include "page_table.h"
#include "physical_memory.h"

int init_physical_memory(physical_memory_t physical_memory) {
	for (int i = 0;  i < PAGE_SIZE*NUM_PAGES; i++) {
		physical_memory[i] = 0;
	}
	return 0;
}

int read_physical_memory(paddress_t physical_address, physical_memory_t physical_memory, 
							value_t* value) {
	*value = physical_memory[physical_address];
	return 0;
}

// CLEAN
int load_frame_to_physical_memory(page_t page_num, frame_t frame_num, 
									physical_memory_t physical_memory) {
    /* Initialize all needed variable */
    FILE *file;
    int i;
    laddress_t seek_position;
    paddress_t place_position;
    fpos_t pos;
    int num_bytes_read;
    byte one_byte; 
    num_bytes_read = PAGE_SIZE;
    
    /* Where data will be read to in physical memory */    
    place_position = frame_num << OFFSET_BITS;

    /* open the backingstore file */
    file = fopen(backing_store, "r" );
        
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 ) {
         printf( "Could not open file: %s.\n", backing_store);
         return 1;
    }

    /* Start your read at the beginning of the relevant frame */
    seek_position = page_num << OFFSET_BITS;
    fseek(file, seek_position, SEEK_SET);
    fgetpos(file, &pos);

    /* Read from backingstore and write to physical_memory */    
    for (i = 0; i < num_bytes_read; i++) {
       fread(&one_byte, 1, 1, file);
       physical_memory[place_position+i] = one_byte;

    }
    fclose( file );

	return 0;
}