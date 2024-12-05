/* pgfhandler.c - pgfhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * pgfhandler - high level page fault interrupt handler
 *------------------------------------------------------------------------
 */
void	pgfhandler()
{
    unsigned long* ebp = NULL;
    int status;

    struct	procent	*prptr = &proctab[currpid];

    v32addr_t page_faulted_addr = get_page_faulted_addr_cr2();


    /* Check if page fault corresponds to unallocated memory */
    status = is_addr_allocated_by_vmhgetmem(page_faulted_addr);
    if(status == 0) { //status is false
        /* Terminate process */
        kill(currpid);
    }

    page_faulted_addr = drop_offset_from_addr(page_faulted_addr);

    pd_t* page_dir_entry = get_page_directory_entry(page_faulted_addr, (p32addr_t*)prptr->page_dir_addr);

    // check if page table is present
    if(page_dir_entry->pd_pres==0)
    {
        // build a page table
        int32 page_table_addr_raw = (int32) get_empty_frame_from_regionD(currpid);
        if(page_table_addr_raw == SYSERR) {
            return; // TODO: What to do? Process will continue to page fault
        }

        p32addr_t* page_table_addr = (p32addr_t*) page_table_addr_raw;

        // Initialize page table
		initialize_empty_page_table(page_table_addr);

        // Set pres bit to 1 and make page dir entry point to new page table
        set_page_directory_entry(page_dir_entry, (p32addr_t)page_table_addr);

    }

    pt_t* page_table_entry = get_page_table_entry(page_faulted_addr, page_dir_entry);

    p32addr_t* page_table_addr = (p32addr_t*) (page_dir_entry->pd_base << 12);

    //check if page is present
    if(page_table_entry->pt_pres==0)
    {

        int index_fHolderListE2 = get_matching_frame_from_regionE2(page_faulted_addr);
        //page present in E2
        if(index_fHolderListE2 != -1)
        {

            p32addr_t* page_addr = (p32addr_t*) get_empty_frame_from_regionE1(page_faulted_addr);

            // if E1 is free (bring frame from E2 to E1)
            if((int) page_addr != -1)
            {
                p32addr_t absolute_addr = (index_fHolderListE2 + REGIONSTART_E2) * NBPG;

                // copy contents of E2 into E1;
                memcpy((char *)page_addr, (char *)absolute_addr, PAGE_SIZE);

                // Translate page table address into index for fHolderListD
                uint16 index_fHolderListD = (p32addr_t)page_table_addr/NBPG - FRAME0;
                increment_number_entries_allocated(index_fHolderListD);

                //set pres bit to 1 and make page table entry point to new page
                set_page_table_entry(page_table_entry, (p32addr_t)page_addr);

                // remove page from E2
                free_frame_in_regionE2(page_faulted_addr, currpid);
            }

            // if E1 is full (swap frames E2 and E1)
            else
            {
                int oldest_frame_index_E1 = get_index_oldest_frame_regionE1();
    
                v32addr_t oldest_frame_vaddr = fHolderListE1[oldest_frame_index_E1].vaddr;
                pid32 oldest_frame_pid = fHolderListE1[oldest_frame_index_E1].owner_process;

                p32addr_t oldest_frame_paddr_E1 = (oldest_frame_index_E1 + REGIONSTART_E1) * NBPG;
                p32addr_t matching_frame_paddr_E2 = (index_fHolderListE2 + REGIONSTART_E2) * NBPG;


                // swap contents of E1 and E2
                char* temp_area = getmem(PAGE_SIZE);
                memcpy((char *)temp_area, (char *)matching_frame_paddr_E2, PAGE_SIZE);
                memcpy((char *)matching_frame_paddr_E2, (char *)oldest_frame_paddr_E1, PAGE_SIZE);
                memcpy((char *)oldest_frame_paddr_E1, (char *)temp_area, PAGE_SIZE);
                freemem(temp_area, PAGE_SIZE);


                // update contents E1
                fHolderListE1[oldest_frame_index_E1].vaddr = fHolderListE2[index_fHolderListE2].vaddr;
                fHolderListE1[oldest_frame_index_E1].owner_process = fHolderListE2[index_fHolderListE2].owner_process;
                fHolderListE1[oldest_frame_index_E1].time_counter = frame_counter++;

                    // Translate page table address into index for fHolderListD
                    uint16 index_fHolderListD = (p32addr_t)page_table_addr/NBPG - FRAME0;
                    increment_number_entries_allocated(index_fHolderListD);

                    //set pres bit to 1 and make page table entry point to new page
                    set_page_table_entry(page_table_entry, oldest_frame_paddr_E1);


                // update contents E2
                fHolderListE2[index_fHolderListE2].vaddr = oldest_frame_vaddr;
                fHolderListE2[index_fHolderListE2].owner_process = oldest_frame_pid;

                    // get page directory addr and page directory entry
                    struct procent *evicted_prptr = &proctab[fHolderListE2[index_fHolderListE2].owner_process];
                    invalidate_page_table_entries(fHolderListE2[index_fHolderListE2].vaddr, 1, (p32addr_t*)evicted_prptr->page_dir_addr, fHolderListE2[index_fHolderListE2].owner_process);
                    // needs to be investigated removing the page table when its the last page

            }
        }

        // first time access
        else
        {

            p32addr_t* page_addr = (p32addr_t*) get_empty_frame_from_regionE1(page_faulted_addr);
            // if E1 is free (add new frame to E1)
            if((int) page_addr != -1)
            {
                // Translate page table address into index for fHolderListD
                uint16 index_fHolderListD = (p32addr_t)page_table_addr/NBPG - FRAME0;

                increment_number_entries_allocated(index_fHolderListD);

                //set pres bit to 1 and make page table entry point to new page
                set_page_table_entry(page_table_entry, (p32addr_t)page_addr);
            }
            else
            {
                int oldest_frame_index_E1 = get_index_oldest_frame_regionE1();
    
                v32addr_t oldest_frame_vaddr = fHolderListE1[oldest_frame_index_E1].vaddr;
                pid32 oldest_frame_pid = fHolderListE1[oldest_frame_index_E1].owner_process;

                p32addr_t oldest_frame_paddr_E1 = (oldest_frame_index_E1 + REGIONSTART_E1) * NBPG;

                char* empty_frame_addr_E2 = get_empty_frame_from_regionE2(oldest_frame_vaddr, oldest_frame_pid);

                // if E1 is full && E2 is not full (evict page from E1 into E2 and add new frame to E1)
                if((int) empty_frame_addr_E2 != -1)
                {

                    // update E2 (already done by get_empty_frame_from_regionE2)
                    memcpy((char*)empty_frame_addr_E2, (char*)oldest_frame_paddr_E1, PAGE_SIZE);
                    
                    // update E1
                    fHolderListE1[oldest_frame_index_E1].vaddr = page_faulted_addr;
                    fHolderListE1[oldest_frame_index_E1].owner_process = currpid;
                    fHolderListE1[oldest_frame_index_E1].time_counter = frame_counter++;

                    uint16 index_fHolderListD = (p32addr_t)page_table_addr/NBPG - FRAME0;
                    increment_number_entries_allocated(index_fHolderListD);

                    // update page table entry of E1
                    set_page_table_entry(page_table_entry, oldest_frame_paddr_E1);

                    // update page table entry of E2
                    uint16 index_fHolderListE2 = (p32addr_t)empty_frame_addr_E2/NBPG - REGIONSTART_E2;
                    struct procent *evicted_prptr = &proctab[fHolderListE2[index_fHolderListE2].owner_process];
                    invalidate_page_table_entries(fHolderListE2[index_fHolderListE2].vaddr, 1, (p32addr_t*)evicted_prptr->page_dir_addr, fHolderListE2[index_fHolderListE2].owner_process);

                }
                // if E1 is full && E2 is full (add the process to PR_FRAME state)
                else
                {
                    // Insert process into framewait queue
                    insert(currpid, framewait, -1 * frame_counter);

                    // Call resched()
                    prptr->prstate = PR_FRAME;
                    resched();

                    // If it returns, there MAYBE a free frame available either in E1 or E2
                    return;
                }         
            }            
        }
        
    } else {
        
        /* Get error code */
        asm("movl %%ebp, %0;"
            :"=r"(ebp)
            );
        ebp += 10; // (ebp + ret + general purpose registers) + errorcode + return addr to page faulted instruction

        /* Check for access violation */
        status = is_read_write_access_violation(*ebp, page_table_entry);
        if(status == 1) {
            /* Terminate process */
            kill(currpid);
        }
    }

}
