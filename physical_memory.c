#include <stdio.h>
#include "system.h"
#include "tlb.h"
#include "address.h"
#include "page_table.h"
#include "physical_memory.h"
#define DEBUG

int init_physical_memory(physical_memory_t physical_memory) {
	for (int i = 0;  i < PAGE_SIZE*NUM_PAGES; i++) {
		physical_memory[i] = 0;
	}
	return 0;
}

// NEEDS TESTING
int read_physical_memory(paddress_t physical_address, physical_memory_t physical_memory, 
							value_t* value) {
	
	printf("%d\n",physical_address);
	*value = physical_memory[physical_address];
 
	return 0;
}

int load_frame_to_physical_memory(frame_t frame_num, physical_memory_t physical_memory) {
    FILE *file;
    int i;
    paddress_t seek_position;
    fpos_t pos;
    int num_bytes_read;
    byte one_byte; 
    
    // Start your read at the beginning of the relevant frame

    seek_position = frame_num << OFFSET_BITS;
    num_bytes_read = PAGE_SIZE;

    /* open the backingstore file */
    file = fopen(backing_store, "r" );
        
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 ) {
         printf( "Could not open file: %s.\n", backing_store);
         return 1;
    }

    /* SEEK_SET: reference position is the beginning of file */
    fseek(file, seek_position, SEEK_SET);
    fgetpos(file, &pos);
    printf("Reading from position: %lld.\n", pos);

    /* Read from backingstore and write to physical_memory */
    
    for (i = 0; i < num_bytes_read; i++) {
       fread(&one_byte, 1, 1, file);
       printf("0x%x, %d\n", one_byte, one_byte); //PRINT AS ONE 32-bit string
       physical_memory[seek_position+i] = one_byte;
       /* 
        * Note: If one_byte's data type is int, 
        * then we have to use a bit mask: one_byte&0xFF
        */ 
    }
    printf("\n");

    fclose( file );

	return 0;
}