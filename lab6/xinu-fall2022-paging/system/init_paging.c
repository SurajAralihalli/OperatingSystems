/* init_paging.c */

#include <xinu.h>

/*------------------------------------------------------------------------
 * init_paging  -  set up kernel data structure to manage per process heaps in virtual memory
                   set up page fault handler for intr 14
                   update context switch to account for richer process context
 *------------------------------------------------------------------------
 */


void init_paging(void)
{
	/* Initialize fHolderList for regions D, E1 */
	initialize_fholderList();
    
	/* Create page directory */
	p32addr_t* page_dir_addr = (p32addr_t*) get_empty_frame_from_regionD(NULLPROC);

	/* Initialize page directory */
	initialize_empty_page_directory((p32addr_t*) page_dir_addr);

	/* Create shared page tables for regions A-E, G*/
	p32addr_t pg_dir_indices[5] = {0, 1, 2, 3, 576};
	uint32 i;
	for(i = 0; i < 5; i++) {
		p32addr_t* page_table_addr = (p32addr_t*) get_empty_frame_from_regionD(NULLPROC);
		
		/* Initialize page table */
		initialize_empty_page_table(page_table_addr);

		uint32 pg_dir_index = pg_dir_indices[i];

		pd_t* page_dir_entry = (pd_t*) &(page_dir_addr[pg_dir_index]);

		set_page_directory_entry(page_dir_entry, (p32addr_t)page_table_addr);

		// p32addr_t base_addr = (p32addr_t) page_dir_entry->pd_base;
		
		build_identity_map_entry(page_table_addr, pg_dir_index);

		/* Save identity mapped page tables  */
		identityMapAddrList[i].page_table_addr = page_table_addr;
		identityMapAddrList[i].page_dir_index = pg_dir_index;
	}

	// set up page directory table for NULL process
	struct	procent	*prptr = &proctab[NULLPROC];
	prptr->page_dir_addr = page_dir_addr;

    // Setup page fault handler
	// set_evec(14, (uint32)pgfdisp);

	/* Update CR3 to setup null process page directory address */
	set_page_dir_addr_cr3((p32addr_t)page_dir_addr); //20 MSB bits


	/* Enable paging - Set PG bit of CR0 to 1 */
	uint32 cr0_val = 0;
	asm("movl %%cr0, %0" 
		: "=r"(cr0_val));
	cr0_val = cr0_val | 1 << 31;
	asm("movl %0, %%cr0" 
		:
		: "r"(cr0_val));

}