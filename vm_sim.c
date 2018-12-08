
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "tlb.h"
#include "address.h"
#include "page_table.h"
#include "physical_memory.h"
#define DEBUG

// This is how many logic addresses are requested
typedef laddress_t logic_address_list_t[PAGE_SIZE*NUM_PAGES]; //SIZE INCORRECT (needs to be dynamic)
typedef value_t address_value_list_t[PAGE_SIZE*NUM_PAGES]; //SIZE INCORRECT (see above)
int logic_address_loader(char* logic_address_file_name, logic_address_list_t logic_address_list, 
                            int* logic_address_list_size);
int get_a_logic_address(logic_address_list_t logic_address_list, int index, laddress_t* logic_address); //NOPE
int update_address_value_list(laddress_t logic_address, paddress_t physical_address,
                                value_t value, int index, address_value_list_t address_value_list); //NOPE
int output_address_value_list(char* output_file_name, address_value_list_t address_value_list, int list_size); //NOPE

int main () { 
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
    const char input_file[] = "testinput.txt";
    const char output_file[] = "output.txt";

    /* Initialize the system */
    tlb_init(&sys_tlb);
    page_table_init(&page_table);
    init_physical_memory(physical_memory);

    /* Lists for I/O */
    logic_address_list_t logic_address_list;
    int logic_address_list_size;

    /* Create a logical address list from the file */ 
    logic_address_loader(input_file, logic_address_list, &logic_address_list_size); //INCLUDE SIZE

    // TEST MEM ACCESS
    /*
    value_t val = 100;
    physical_address = 0xff07;
    frame_num= 0xff;
    load_frame_to_physical_memory(frame_num, physical_memory);
    read_physical_memory(physical_address, physical_memory, &val);
    printf("%d\n",val);
    */

    for (int i = 0; i < logic_address_list_size; i++) {    
        /* Get a logic address, its page number and offset */    
        get_a_logic_address(logic_address_list, i, &logic_address);   
        printf("logic address: %s (%d)\n", itob16(logic_address), logic_address); 
        /*      
         * The code below demonstrates how to use a pointer to access      
         * page_number updated by the get_page_number() function      
         */    
        get_page_number(logic_address, &page_num);     
        get_offset(logic_address, &offset);    

        printf("page %d, offset %d\n", page_num, offset);
        /* Search the TLB */    
        search_tlb(page_num, sys_tlb, &is_tlb_hit, &frame_num);  

        /* Hit the TLB: the address translation is done. */    
        if (is_tlb_hit == TRUE) {        
            create_physical_address(frame_num, offset, &physical_address);    
        }    

        /* TLB Miss: check page table */    
        else {        
            search_page_table(page_num, page_table, &is_page_fault, &frame_num);

            /* page is found in the page table */        
            if (is_page_fault == FALSE) {            
                create_physical_address(frame_num, offset, &physical_address);

                /* Replace the oldest entry in the TLB with this new entry */            
                tlb_replacement_FIFO(page_num, frame_num, &sys_tlb);     //CHANGE TO LRU   
            }        
            /* page fault occurs: call fault_fault_handler */        
            else {             
                page_fault_handler(page_num, &physical_memory,                                
                                    &page_table, &sys_tlb);            
                create_physical_address(frame_num, offset, &physical_address);        
            }    
        } 
        /* end of else TLB Miss */   
        /* Read one-byte value from the physical memory */    
        read_physical_memory(physical_address, physical_memory, &value);    \

        printf("value: %d\n", value);
        /* Update the address-value list */ 
        update_address_value_list(logic_address, physical_address, value,                             
                                    i, address_value_list);
    } 
    /* end of for logic_address_list */
    /* Output the address-value list into an output file */
    output_address_value_list(output_file, address_value_list, logic_address_list_size);
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