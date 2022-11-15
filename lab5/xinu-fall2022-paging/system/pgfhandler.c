/* pgfhandler.c - pgfhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * pgfhandler - high level page fault interrupt handler
 *------------------------------------------------------------------------
 */
void	pgfhandler()
{

    kprintf("Entered page fault handler!\n");

    struct	procent	*prptr = &proctab[currpid];

    v32addr_t page_faulted_addr = get_page_faulted_addr_cr2();

    kprintf("page directory addr: %x\n", (p32addr_t*)prptr->page_dir_addr);

    pd_t* page_dir_entry = get_page_directory_entry(page_faulted_addr, (p32addr_t*)prptr->page_dir_addr);

    // check if page table is present
    if(page_dir_entry->pd_pres==0)
    {
        //build a page table
        p32addr_t* page_table_addr = (p32addr_t*) get_empty_frame_from_regionD();

        //Initialize page table
		initialize_empty_page_table(page_table_addr);

        //set pres bit to 1 and make page dir entry point to new page table
        set_page_directory_entry(page_dir_entry, (p32addr_t)page_table_addr);

    }
    kprintf("Allocated page directory entry: %x!\n", page_dir_entry);

    pt_t* page_table_entry = get_page_table_entry(page_faulted_addr, page_dir_entry);

    p32addr_t* page_table_addr = (p32addr_t*) (page_dir_entry->pd_base << 12);

    kprintf("page table addr: %x\n", page_table_addr);

    // check if page is present
    if(page_table_entry->pt_pres==0)
    {
        // Build a page
        p32addr_t* page_addr = (p32addr_t*) get_empty_frame_from_regionE1(page_faulted_addr);

        // Translate page table address into index for fHolderListD
        uint16 frame_index = (p32addr_t)page_table_addr/NBPG - FRAME0;

        increment_number_pages_allocated(frame_index);

        //set pres bit to 1 and make page table entry point to new page
        set_page_table_entry(page_table_entry, (p32addr_t)page_addr);

        kprintf("page addr: %x\n", page_addr);
    }

    kprintf("Allocated page table entry: %x!\n", page_table_entry);

}
