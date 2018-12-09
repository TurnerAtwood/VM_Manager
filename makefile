vm_make: address.h page_table.h physical_memory.h system.h tlb.h
	gcc -Wall -o vm_sim vm_sim.c address.c page_table.c physical_memory.c tlb.c