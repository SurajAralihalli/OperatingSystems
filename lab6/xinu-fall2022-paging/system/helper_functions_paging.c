/* helper_functions_paging.c */

#include <xinu.h>


/*------------------------------------------------------------------------
 * get_empty_frame_from_regionD  -  Return absolute address of empty frame 
                                    from region D
 *------------------------------------------------------------------------
 */
char* get_empty_frame_from_regionD(pid32 pid)
{
    uint32 absolute_addr = 0;
    int32 i;
    for(i = 0; i < NFRAMES_D; i++) {
        if(fHolderListD[i].frame_pres == 0) {
            fHolderListD[i].frame_pres = 1;
            fHolderListD[i].owner_process = pid;
            absolute_addr = (i + FRAME0) * NBPG;
            fHolderListD[i].vaddr = absolute_addr;
            
            return (char*) absolute_addr;  
        }
    }
    return (char*) SYSERR;  
}


/*------------------------------------------------------------------------
 * get_empty_frame_from_regionE1 -  Return absolute address of empty frame from region E1
 *------------------------------------------------------------------------
 */
char* get_empty_frame_from_regionE1(v32addr_t vaddr)
{
    uint32 absolute_addr = 0;
    int32 i;
    for(i = 0; i < NFRAMES_E1; i++) {
        if(fHolderListE1[i].frame_pres == 0) {
            fHolderListE1[i].frame_pres = 1;
            fHolderListE1[i].owner_process = currpid;
            fHolderListE1[i].vaddr = vaddr;
            absolute_addr = (i + FRAME0 + NFRAMES_D) * NBPG; // FRAME0 + NFRAMES_D is starting frame in E1
            fHolderListE1[i].time_counter = frame_counter++;

            return (char*) absolute_addr;  
        }
    }
    return (char*) SYSERR;
}

/*------------------------------------------------------------------------
 * initialize_empty_page_directory -  Initialize page directory entries to default values
 *------------------------------------------------------------------------
 */
void initialize_empty_page_directory(p32addr_t* page_dir_addr)
{
    uint32 i;
	for(i = 0; i < ENTRIES_PER_FRAME; i++) {
        pd_t* page_dir_entry = (pd_t*)&(page_dir_addr[i]);
		page_dir_entry->pd_pres = 0;  	/* page table present?		*/
        page_dir_entry->pd_write =1;			/* page is writable?		*/
        page_dir_entry->pd_user= 0;		/* is use level protection?	*/
        page_dir_entry->pd_pwt	=1;		/* write through cachine for pt?*/
        page_dir_entry->pd_pcd	=0;		/* cache disable for this pt?	*/
        page_dir_entry->pd_acc	=0;		/* page table was accessed?	*/
        page_dir_entry->pd_mbz	=0;		/* must be zero			*/
        page_dir_entry->pd_fmb	=0;		/* four MB pages?		*/
        page_dir_entry->pd_global=0;		/* global (ignored)		*/
        page_dir_entry->pd_avail =0;		/* for programmer's use		*/
        page_dir_entry->pd_base	=0;	    /* location of page table?	*/
	}
    flush_tlb();
}

/*------------------------------------------------------------------------
 * initialize_empty_page_table -  Initialize page table entries to default values
 *------------------------------------------------------------------------
 */
void initialize_empty_page_table(p32addr_t* page_table_addr)
{
    int32 i;
	for(i = 0; i < ENTRIES_PER_FRAME; i++) {
        pt_t* page_table_entry = (pt_t*)&(page_table_addr[i]);
        page_table_entry->pt_pres	= 0;		/* page is present?		*/
        page_table_entry->pt_write = 1;		/* page is writable?		*/
        page_table_entry->pt_user	= 0;		/* is use level protection?	*/
        page_table_entry->pt_pwt	= 1;		/* write through for this page? */
        page_table_entry->pt_pcd	= 0;		/* cache disable for this page? */
        page_table_entry->pt_acc	= 0;		/* page was accessed?		*/
        page_table_entry->pt_dirty = 0;		/* page was written?		*/
        page_table_entry->pt_mbz	= 0;		/* must be zero			*/
        page_table_entry->pt_global= 0;		/* should be zero in 586	*/
        page_table_entry->pt_avail = 0;		/* for programmer's use		*/
        page_table_entry->pt_base	= 0;		/* location of page?		*/
	}
    flush_tlb();
}

/*------------------------------------------------------------------------
 * build_identity_map_entry -  Perform identity mapping of entries in page table with
                    start address `page_tab_addr`. Mapping is performed for
                    page directory entry corresponding to `page_dir_index`
                    of the null process
 *------------------------------------------------------------------------
 */
void build_identity_map_entry(p32addr_t* page_table_addr, uint32 page_dir_index)
{
    uint32 i;
    for(i = 0 ; i < ENTRIES_PER_FRAME; i++) {
        pt_t* page_tab_entry = (pt_t*)&(page_table_addr[i]);
        page_tab_entry->pt_pres = 1;
        page_tab_entry->pt_write = 1;		/* page is writable?		*/
        page_tab_entry->pt_user = 0;		/* is use level protection?	*/
        page_tab_entry->pt_pwt	= 1;		/* write through for this page? */
        page_tab_entry->pt_pcd	= 0;		/* cache disable for this page? */
        page_tab_entry->pt_acc	= 0;		/* page was accessed?		*/
        page_tab_entry->pt_dirty = 0;		/* page was written?		*/
        page_tab_entry->pt_mbz	= 0;		/* must be zero			*/
        page_tab_entry->pt_global= 0;		/* should be zero in 586	*/
        page_tab_entry->pt_avail = 0;		/* for programmer's use		*/
        page_tab_entry->pt_base = i | (page_dir_index << 10);		/* location of page?		*/
    }
    flush_tlb();
}

/*------------------------------------------------------------------------
 * set_page_directory_entry -  Set P bit  and page table frame number 
                            of page directory entry
 *------------------------------------------------------------------------
 */
void set_page_directory_entry(pd_t* page_dir_entry, p32addr_t page_table_addr)
{
    
    /* Mark present bit to 1 */
    page_dir_entry->pd_pres = 1;

    /* Assign page table address to page directory entry */
    page_dir_entry->pd_base = (page_table_addr >> 12);

    /* Invalidate TLB */
    flush_tlb();
}


/*------------------------------------------------------------------------
 * set_page_table_entry -  Set P bit  and page frame number 
                            of page table entry
 *------------------------------------------------------------------------
 */
void set_page_table_entry(pt_t* page_table_entry, p32addr_t page_addr)
{
    
    /* Mark present bit to 1 */
    page_table_entry->pt_pres = 1;

    /* Assign page address to page table entry */
    page_table_entry->pt_base = (page_addr >> 12);

    /* Invalidate TLB */
    flush_tlb();
}

/*------------------------------------------------------------------------
 * reset_page_table_entry -  Set P bit to 0 in page table entry
 *------------------------------------------------------------------------
 */
void reset_page_table_entry(pt_t* page_table_entry)
{
    /* Mark present bit to 0 */
    page_table_entry->pt_pres = 0;

    /* Make the base 0 */
    page_table_entry->pt_base = 0;

    /* Invalidate TLB */
    flush_tlb();
}

/*------------------------------------------------------------------------
 * reset_page_directory_entry -  Set P bit to 0 in page directory entry
 *------------------------------------------------------------------------
 */
void reset_page_directory_entry(pd_t* page_directory_entry)
{
    /* Mark present bit to 0 */
    page_directory_entry->pd_pres = 0;

    /* Invalidate TLB */
    flush_tlb();
}


void initialize_fholderList()
{
    uint32 i;
    /* Initialize fHolderListD */
    for(i = 0; i < NFRAMES_D; i++) {
        fHolderListD[i].frame_pres = 0;
        fHolderListD[i].owner_process = -1;
        fHolderListD[i].vaddr = 0x0;
        fHolderListD[i].nentries_allocated = 0;
    }

    /* Initialize fHolderListE1 */
    for(i = 0; i < NFRAMES_E1; i++) {
        fHolderListE1[i].frame_pres = 0;
        fHolderListE1[i].owner_process = -1;
        fHolderListE1[i].vaddr = 0x0;
        fHolderListE1[i].time_counter = 0;
    }

    /* Initialize fHolderListE1 */
    for(i = 0; i < NFRAMES_E2; i++) {
        fHolderListE2[i].frame_pres = 0;
        fHolderListE2[i].owner_process = -1;
        fHolderListE2[i].vaddr = 0x0;
    }

}


/*------------------------------------------------------------------------
 * get_page_table_entry -  Get pointer to page table entry given a virtual address
                            `page_faulted_addr` and page directory entry 
                            `page_dir_entry`
 *------------------------------------------------------------------------
 */
pt_t* get_page_table_entry(v32addr_t page_faulted_addr, pd_t* page_dir_entry)
{
    // find page table entry
    p32addr_t* page_table_addr = (p32addr_t*) ((page_dir_entry->pd_base) << 12);  //base + "000000000000"
    uint32 page_table_index = (page_faulted_addr & 0x003FF000) >> 12; //page table index + page index + offset
    pt_t* page_table_entry =  (pt_t*) &(page_table_addr[page_table_index]);
    return page_table_entry;
}


/*------------------------------------------------------------------------
 * get_page_directory_entry -  Get pointer to page directory entry given a virtual address
                                `page_faulted_addr` and base address of page directory
                                `page_dir_addr`
 *------------------------------------------------------------------------
 */
pd_t* get_page_directory_entry(v32addr_t page_faulted_addr, p32addr_t* page_dir_addr)
{
    // find page directory entry
    uint32 page_dir_index = page_faulted_addr >> 22;
    pd_t* page_dir_entry = (pd_t*) &(page_dir_addr[page_dir_index]);
    return page_dir_entry;
}

/*------------------------------------------------------------------------
 * deallocate_frames_E1 -  Get pointer to page directory entry given a virtual address
                                `page_faulted_addr` and base address of page directory
                                `page_dir_addr`
 *------------------------------------------------------------------------
 */
void deallocate_frames_E1(v32addr_t start_vaddr, uint16 npages, pid32 owner_pid)
{
    uint32 i;
    /* Deallocate frames related to the virtual address */
	for(i = 0; i < npages; i++) {
        v32addr_t vaddr = start_vaddr + (i * NBPG);

		/* Free frame in region E1 */
		free_frame_in_regionE1(vaddr, owner_pid);
	}
}

/*------------------------------------------------------------------------
 * purge_frames_D -  Purge all frames in D whose `owner_process` matches pid
 *------------------------------------------------------------------------
 */
void purge_frames_fHolderListD(pid32 pid)
{
    uint32 i;
    for(i = 0; i < NFRAMES_D; i++) {
        if(fHolderListD[i].owner_process == pid) {
            fHolderListD[i].frame_pres = 0;
            fHolderListD[i].owner_process = -1;
            fHolderListD[i].vaddr = 0x0;
            fHolderListD[i].nentries_allocated = 0;
        }
    }
}

/*------------------------------------------------------------------------
 * purge_frames_E1 -  Purge all frames in E1 whose `owner_process` matches pid
 *------------------------------------------------------------------------
 */
uint32 purge_frames_fHolderListE1(pid32 pid)
{
    uint32 i;
    uint32 counter;
    counter = 0;
    for(i = 0; i < NFRAMES_E1; i++) {
        if(fHolderListE1[i].owner_process == pid) {
            fHolderListE1[i].frame_pres = 0;
            fHolderListE1[i].owner_process = -1;
            fHolderListE1[i].vaddr = 0x0;
            fHolderListE1[i].time_counter = 0;
            counter++;
        }
    }
    return counter;
}


/*------------------------------------------------------------------------
 * invalidate_page_table_entries -  Invalidate page table entries corresponding to 
                                    virtual address `start_vaddr` and going up to `npages`
 *------------------------------------------------------------------------
 */
void invalidate_page_table_entries(v32addr_t start_vaddr, uint16 npages, p32addr_t* page_dir_addr, pid32 owner_pid)
{
    uint32 i;
	for(i = 0; i < npages; i++) {
        v32addr_t vaddr = start_vaddr + (i * NBPG);

        /* Get page directory entry corresponding to vaddr */
        pd_t* page_dir_entry = get_page_directory_entry(vaddr, page_dir_addr);

        /* Get corresponding page table entry */
        pt_t* page_table_entry = get_page_table_entry(vaddr, page_dir_entry);

        /* Invalidate page table entry - Sets P bit to 0     */
        reset_page_table_entry(page_table_entry);

        // Translate page table address into index for fHolderListD
        p32addr_t page_table_addr = (page_dir_entry->pd_base) << 12;
        uint16 index_fHolderListD = page_table_addr/NBPG - FRAME0;

        // Decrement number of pages allocated
        decrement_number_entries_allocated(index_fHolderListD);

        // Check if page table has 0 used page table entries
        if(fHolderListD[index_fHolderListD].nentries_allocated == 0) {
            /* Free frame in region D */
            free_frame_in_regionD((v32addr_t) page_table_addr, owner_pid);

            /* Make P bit in page directory entry 0 */
            reset_page_directory_entry(page_dir_entry);
        }

	}

}

/*------------------------------------------------------------------------
 * free_frame_in_regionD -  Update fHolderListD to reuse frames
 *------------------------------------------------------------------------
 */
void free_frame_in_regionD(v32addr_t vaddr, pid32 owner_pid)
{
    uint32 i;
    for(i = 0; i < NFRAMES_D; i++) {
        if(fHolderListD[i].vaddr == vaddr && fHolderListD[i].owner_process == owner_pid) {
            fHolderListD[i].frame_pres = 0;
            fHolderListD[i].owner_process = -1;
            fHolderListD[i].vaddr = 0x0;
            break;
        }
    }
}

/*------------------------------------------------------------------------
 * free_frame_in_regionE1 -  Update fHolderListE1 to reuse frames
 *------------------------------------------------------------------------
 */
void free_frame_in_regionE1(v32addr_t vaddr, pid32 owner_pid)
{
    vaddr = drop_offset_from_addr(vaddr);
    uint32 i;
    for(i = 0; i < NFRAMES_E1; i++) {
        if(fHolderListE1[i].vaddr == vaddr && fHolderListE1[i].owner_process == owner_pid) {
            fHolderListE1[i].frame_pres = 0;
            fHolderListE1[i].owner_process = -1;
            fHolderListE1[i].vaddr = 0x0;
            fHolderListE1[i].time_counter = 0;
            break;
        }
    }
}

/*------------------------------------------------------------------------
 * increment_number_pages_allocated -  Increment number of pages allocated
 *------------------------------------------------------------------------
 */
void increment_number_entries_allocated(uint16 index)
{
    fHolderListD[index].nentries_allocated++;
}

/*------------------------------------------------------------------------
 * decrement_number_pages_allocated -  Decrement number of pages allocated
 *------------------------------------------------------------------------
 */
void decrement_number_entries_allocated(uint16 index)
{
    fHolderListD[index].nentries_allocated--;
}


v32addr_t drop_offset_from_addr(v32addr_t vaddr)
{
    return (vaddr >> 12) << 12;
}

uint16 is_read_write_access_violation(uint32 page_fault_error_code, pt_t* page_table_entry)
{
    uint16 read_write_bit = (page_fault_error_code >> 1) & (0x00000001);
    if(read_write_bit == 1 && page_table_entry->pt_write == 0) {
        return 1;
    }
    return 0;
}

int is_addr_allocated_by_vmhgetmem(v32addr_t addr)
{

    int notFound = 1; //true
    struct	procent* prptr = &proctab[currpid];
    struct vmemblk* curr;

    curr = prptr->vmemlist_ptr;
	curr = curr->mnext;              // first addr node

	while (curr != NULL) {			/* Search free list	*/
        v32addr_t start_addr = curr->start_addr;
        uint32 npages = curr->npages;

        // check if addr lies in the node
        if( addr >= start_addr && addr <= (start_addr + npages*NBPG))
        {
            notFound = 0; //false (addr is found in unassigned address space)
            break;
        }
        curr = curr->mnext;
	}
    return notFound;
}