
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "tlb.h"
#include "address.h"
#include "page_table.h"
#include "physical_memory.h"

// This is how many logic addresses are requested
typedef laddress_t logic_address_list_t[PAGE_SIZE*NUM_PAGES]; //SIZE INCORRECT (needs to be dynamic)
typedef value_t address_value_list_t[PAGE_SIZE*NUM_PAGES]; //SIZE INCORRECT (see above)
static const char output_file_name[] = "vm_sim_output.txt";

int logic_address_loader(char* logic_address_file_name, logic_address_list_t logic_address_list, 
                            int* logic_address_list_size);
int get_a_logic_address(logic_address_list_t logic_address_list, int index, laddress_t* logic_address); //NOPE
int update_address_value_list(laddress_t logic_address, paddress_t physical_address,
                                value_t value, int index, address_value_list_t address_value_list); //NOPE
int output_address_value_list(char* output_file_name, address_value_list_t address_value_list, int list_size); //NOPE
void welcome_message(void);

int main ( int argc, char *argv[] ) { 
    /* argc should be 2 for correct execution */
    if (argc != 2) {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s, <input logic address file>\n", argv[0]);
        return 1;
    }

    welcome_message();
    /* three counters initialized to be 0 */
    u_int_t lookups = 0;
    u_int_t tlb_hits = 0;
    u_int_t page_faults = 0;   

    /* Variables: page number, frame number and offset */
    page_t page_num;
    frame_t frame_num;
    offset_t offset;

    /* Addresses */
    laddress_t logic_address;
    paddress_t physical_address;

    /* The TLB and page table */
    tlb_t sys_tlb;
    page_table_t page_table;

    /* Simulated main memory */
    physical_memory_t physical_memory;

    /* value and address-value list */
    value_t value;
    address_value_list_t address_value_list;

    /* Boolean for TLB hit and page fault */
    bool is_tlb_hit;
    bool is_page_fault;

    /* Input and output file names */
    char* input_file = argv[1];
    //const char input_file[] = "InputFile.txt";
    

    /* Initialize the system */
    tlb_init(&sys_tlb);
    page_table_init(&page_table);
    init_physical_memory(physical_memory);

    /* Lists for I/O */
    logic_address_list_t logic_address_list;
    int logic_address_list_size;

    /* Create a logical address list from the file */ 
    logic_address_loader(input_file, logic_address_list, &logic_address_list_size); //INCLUDE SIZE

    for (int i = 0; i < logic_address_list_size; i++) { 
        lookups += 1;   
        /* Get a logic address, its page number and offset */    
        get_a_logic_address(logic_address_list, i, &logic_address);   
        /*      
         * The code below demonstrates how to use a pointer to access      
         * page_number updated by the get_page_number() function      
         */    
        get_page_number(logic_address, &page_num);     
        get_offset(logic_address, &offset);    

        //printf("page %d, offset %d\n", page_num, offset);
        /* Search the TLB */    
        search_tlb(page_num, sys_tlb, &is_tlb_hit, &frame_num);  

        /* Hit the TLB: the address translation is done. */    
        if (is_tlb_hit == TRUE) {
            tlb_hits += 1;
            create_physical_address(frame_num, offset, &physical_address);    
        }    

        /* TLB Miss: check page table */    
        else {        
            search_page_table(page_num, page_table, &is_page_fault, &frame_num);

            /* page is found in the page table */        
            if (is_page_fault == FALSE) {            
                create_physical_address(frame_num, offset, &physical_address);

                /* Replace the oldest entry in the TLB with this new entry */            
                tlb_replacement_LRU(page_num, frame_num, &sys_tlb);     //CHANGE TO LRU   
            }        
            /* page fault occurs: call fault_fault_handler */        
            else {
                page_faults += 1;    
                page_fault_handler(page_num, &frame_num, &physical_memory,                                
                                    &page_table, &sys_tlb);            
                create_physical_address(frame_num, offset, &physical_address);        
            }    
        } 
        /* end of else TLB Miss */ 
        // Update the tlb
        tlb_update(page_num, &sys_tlb);  
        /* Read one-byte value from the physical memory */    
        read_physical_memory(physical_address, physical_memory, &value);    \
        /* Update the address-value list */ 
        update_address_value_list(logic_address, physical_address, value,                             
                                    i, address_value_list);
    } 
    /* end of for logic_address_list */

    /* Print results to std out */

    printf("Lookups: %d\n", lookups);
    printf("Page fault rate: %f\n", 1.0*page_faults/lookups);
    printf("TLB hit rate: %f\n", 1.0*tlb_hits/lookups);
    printf("Check the results in the outputfile: %s\n", output_file_name);
    
    /* Output the address-value list into an output file */
    output_address_value_list(output_file_name, address_value_list, logic_address_list_size);
} /* end of main() */


// THESE ARE FUNCTIONS DEALING WITH INPUT/OUTPUT
int logic_address_loader(char* logic_address_file_name, logic_address_list_t logic_address_list, 
                            int* logic_address_list_size) {
    FILE *file;
    int seek_position;
    fpos_t pos;
    seek_position = 0;
    

    /* open the input file */
    file = fopen(logic_address_file_name, "r" );
        
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 ) {
         printf( "Could not open file: %s.\n", logic_address_file_name);
         return 1;
    }

    /* SEEK_SET: reference position is the beginning of file */
    fseek(file, seek_position, SEEK_SET);
    fgetpos(file, &pos);

    /* Read one line at a time until the end of the file */
    
    char line [1000];
    int i = 0;
    while(fgets(line,sizeof line,file)!= NULL) /* read a line from a file */ {
        //fprintf(stdout,"%d\n",atoi(line)); //print the file contents on stdout.
        logic_address_list[i++] = atoi(line);
    }
    fclose( file );
    *logic_address_list_size = i;

    return 0;
}

int get_a_logic_address(logic_address_list_t logic_address_list, int index, laddress_t* logic_address) {
    *logic_address = logic_address_list[index];
    return 0;
}
//CURRENTLY ONLY STORING VALUE
int update_address_value_list(laddress_t logic_address, paddress_t physical_address,
                                value_t value, int index, address_value_list_t address_value_list) {
    address_value_list[index] = value;
    return 0;
}

int output_address_value_list(char* output_file_name, address_value_list_t address_value_list, 
                                int list_size) {
    FILE *file;

    file = fopen(output_file_name, "w" );
    
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 ) {
         printf( "Could not open file: %s.\n", output_file_name);
         return 1;
    }
    /* print integers and floats */
    for (int i = 0; i < list_size; i++) {
        value_t value = address_value_list[i];
        fprintf(file, "%d\n", value);
    }

    fclose( file );

    return 0;
}

void welcome_message(void) {
    printf("Welcome to Turner Atwood's VM Simulator\n");
    printf("Number of logical pages: %d\n", NUM_PAGES);
    printf("Number of physical frames: %d\n", NUM_FRAMES);
    printf("Page size: %d\n", PAGE_SIZE);
    printf("TLB size: %d\n", TLB_SIZE);
}