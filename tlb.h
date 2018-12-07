/*
 * COMP3500: Project 5
 * Sample header file for TLB 
 * Xiao Qin
 */
#ifndef TLB_H
#define TLB_H

#include "system.h"
#include "address.h"

typedef struct {
    page_t  page_num;
    frame_t frame_num;
    bool    valid;      /* indicate if this entry is valid */
}tlb_entry_t;

typedef struct {
    tlb_entry_t tlb_entry[TLB_SIZE]; 
    u_int_t       next_tlb_ptr;  /* next candidate entry to be replaced/used */
}tlb_t;

int TLB_init(tlb_t* tlb);
int TLB_replacement_FIFO(page_t page_num, frame_t frame_num, tlb_t* tlb);
int TLB_display(tlb_t tlb);
#endif
