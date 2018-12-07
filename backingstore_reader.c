/*
 * Project 5: backingstore_reader.c
 *
 * This program demonstrates how to read and print the bytes in the backing store.
 *
 * by Xiao Qin.
 */
#include <stdio.h>
#include "system.h"
#define DEBUG

/* There is no byte data type in C */
typedef unsigned char byte_t;
typedef unsigned int uint_t;

void welcome_message(void);

int main ( int argc, char *argv[] ) {
  
    /* three counters initialized to be 0*/
    uint_t lookups = 0;
    uint_t tlb_hits = 0;
    uint_t page_faults = 0;    

    int x;
    FILE *file;
    int i;
    int seek_position;
    fpos_t pos;
    int num_bytes_read;
    byte_t one_byte; 
    
    seek_position= 4;
    num_bytes_read = 4;    // DON'T HARD CODE (4 bytes per index)

    /* argc should be 2 for correct execution */
    if (argc != 2) {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s, <input logic address file>\n", argv[0]);
        return 0;
    }

    welcome_message();

    /* file = fopen(backingstore, "r" );*/
    file = fopen(argv[1], "r" );
        
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 ) {
         printf( "Could not open file: %s.\n", argv[1]);
    }
    else {
        /* SEEK_SET: reference position is the beginning of file */
        fseek(file, seek_position, SEEK_SET);
        fgetpos(file, &pos);
        printf("Reading from position: %lld.\n", pos);

        /* Read and print data from backingstore */
        
        for (i = 0; i < num_bytes_read; i++) {
           fread(&one_byte, 1, 1, file);
           /* printf prints one byte as hex */ 
           // printf("0x%x, %d\n", one_byte, one_byte); // OLD
           printf("0x%x, %d\n", one_byte, one_byte); //PRINT AS ONE 32-bit string
           /* 
            * Note: If one_byte's data type is int, 
            * then we have to use a bit mask: one_byte&0xFF
            */ 
        }
        printf("\n");
  
        fclose( file );
    }
}

void welcome_message(void) {
    printf("Welcome to Xiao Qin's VM Simulator\n");
    printf("Number of logical pages: %d\n", NUM_PAGES);
    printf("Number of physical frames: %d\n", NUM_FRAMES);
    printf("Page size: %d\n", PAGE_SIZE);
    printf("TLB size: %d\n", TLB_SIZE);
}
