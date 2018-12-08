make:
gcc -Wall -o vm_sim vm_sim.c address.c backingstore_reader.c page_table.c physical_memory.c tlb.c