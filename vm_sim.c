
#include <stdio.h>
#include "system.h"
#include "tlb.h"
#include "address.h"
#include "page_table.h"
#include "physical_memory.h"
#define DEBUG

logic_address_loader(char* logic_address_file_name, laddress_t* logic_address_list); //NOPE

int main () { /* Variables: page number, frame number and offset */
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

    

    /* Create a logical address list from the file */ 
    logic_address_loader(input_file, logic_address_list);

    for (each logical address in logic_address_list) {    
    /* Get a logic address, its page number and offset */    
    get_a_logic_address(logic_address_list, logic_address);    
    /*      
     * The code below demonstrates how to use a pointer to access      
     * page_number updated by the get_page_number() function      
     */    
    get_page_number(logic_address, &page_num);     
    get_offset(logic_address, &offset);    

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
        page_fault_handler(page_num, &physical_memory,                                
                            &page_table, &sys_tlb);            
        create_physical_address(frame_num, offset, &physical_address);        
        }    
    } 
    /* end of else TLB Miss */   
    /* Read one-byte value from the physical memory */    
    read_physical_memory(physical_address, &value);    
    /* Update the address-value list */ 
    update_address_value_list(logic_address, physical_address, value                             
                                &address_value_list)} 
    /* end of for logic_address_list */
    /* Output the address-value list into an output file */
    output_address_value_list(output_file, address_value_list);
} /* end of main() */


// THESE ARE FUNCTIONS DEALING WITH INPUT/OUTPUT