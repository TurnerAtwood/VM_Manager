/*
 * Project 6: address.c
 * Turner Atwood with parts from Xiao Qin.
 */
#include <stdio.h>
#include <limits.h>
#include "address.h"

/*
 * convert machine number to human-readable binary string.
 * Returns: pointer to static string overwritten with each call.
 * Reference: 
 * https://bytes.com/topic/c/answers/621985-print-binary-representation
 * */
char *itob(int x)
{
    static char buff[sizeof(int) * CHAR_BIT + 1];
    int i;
    int j = sizeof(int) * CHAR_BIT - 1;

    buff[j] = 0;
    for(i=0;i<sizeof(int) * CHAR_BIT; i++) {
        if(x & (1 << i))
            buff[j] = '1';
        else
            buff[j] = '0';
        j--;
    } 
    return buff;
}

/*
 * Xiao Qin:
 * This is a mofified version of the above itob()
 * Only print 16 bits
 */
char *itob16(int x)
{
    static char buff[17];
    int i;
    int j = 15;

    buff[j] = 0;
    for(i=0;i<16; i++) {
        if(x & (1 << i))
            buff[j] = '1';
        else
            buff[j] = '0';
        j--;
    } 
    return buff;
}

/*
 * Xiao Qin:
 * This is a mofified version of the above itob()
 * Only print 8 bits
 */
char *itob8(int x)
{
    static char buff[9];
    int i;
    int j = 7;

    buff[j] = 0;
    for(i=0;i<8; i++) {
        if(x & (1 << i))
            buff[j] = '1';
        else
            buff[j] = '0';
        j--;
    } 
    return buff;
}

/*
 *  Returns the offset of the given address
 */
int get_offset(laddress_t logic_address, offset_t* offset) {
    *offset = logic_address & OFFSET_MASK;
    return 0;
}

/*
 *  Returns the page number of a logical address
 */
int get_page_number(laddress_t logic_address, page_t* page_num) {
    *page_num = logic_address >> OFFSET_BITS;
    return 0;
}

/*
 *  Return a physical address from a frame number and an offset
 */

int create_physical_address(frame_t frame_num, offset_t offset, paddress_t* physical_address) {
    *physical_address = frame_num << OFFSET_BITS | offset;
    return 0;
}