/* helper_functions_swapping.c */

#include <xinu.h>


/*------------------------------------------------------------------------
 * get_index_oldest_frame_regionE1  -  returns the oldest frame in E1 (FIFO)
 *------------------------------------------------------------------------
 */


int get_index_oldest_frame_regionE1()
{
    int32 i;
    int oldest_index;
    uint32 smallest_counter = 0xFFFFFFFF;
    for(i = 0; i < NFRAMES_E1; i++) {
        if(fHolderListE1[i].frame_pres != 0) {
            if(fHolderListE1[i].time_counter < smallest_counter)
            {
                smallest_counter = fHolderListE1[i].time_counter;
                oldest_index = i;
            }
        }
    }

    return oldest_index;
}


/*------------------------------------------------------------------------
 * get_matching_frame_from_regionE2  -  returns the index of the frame in E2 which was swapped from E1 if exists else returns -1
 *------------------------------------------------------------------------
 */
int get_matching_frame_from_regionE2(v32addr_t vaddr)
{
    int index = -1;
    int32 i;
    for(i = 0; i < NFRAMES_E2; i++) {
        if(fHolderListE2[i].frame_pres != 0 
            && fHolderListE2[i].owner_process == currpid 
            && fHolderListE2[i].vaddr == vaddr) 
        {
            index = i;
        }
    }

    return index;
}


/*------------------------------------------------------------------------
 * free_frame_in_regionE2 -  Update fHolderListE2 to reuse frames
 *------------------------------------------------------------------------
 */
void free_frame_in_regionE2(v32addr_t vaddr, pid32 owner_pid)
{
    vaddr = drop_offset_from_addr(vaddr);
    uint32 i;
    for(i = 0; i < NFRAMES_E2; i++) {
        if(fHolderListE2[i].vaddr == vaddr && fHolderListE2[i].owner_process == owner_pid) {
            fHolderListE2[i].frame_pres = 0;
            fHolderListE2[i].owner_process = -1;
            fHolderListE2[i].vaddr = 0x0;
            break;
        }
    }
}


/*------------------------------------------------------------------------
 * get_empty_frame_from_regionE2 -  Return absolute address of empty frame from region E2
 *------------------------------------------------------------------------
 */
char* get_empty_frame_from_regionE2(v32addr_t vaddr, pid32 pid)
{
    uint32 absolute_addr = 0;
    int32 i;
    for(i = 0; i < NFRAMES_E2; i++) {
        if(fHolderListE2[i].frame_pres == 0) {
            fHolderListE2[i].frame_pres = 1;
            fHolderListE2[i].owner_process = pid;
            fHolderListE2[i].vaddr = vaddr;
            absolute_addr = (i + REGIONSTART_E2) * NBPG; // REGIONSTART_E2 is starting frame in E2

            return (char*) absolute_addr;  
        }
    }
    return (char*) SYSERR;
}

/*------------------------------------------------------------------------
 * purge_frames_E2 -  Purge all frames in E2 whose `owner_process` matches pid
 *------------------------------------------------------------------------
 */
uint32 purge_frames_fHolderListE2(pid32 pid)
{
    uint32 i;
    uint32 counter;
    counter = 0;
    for(i = 0; i < NFRAMES_E2; i++) {
        if(fHolderListE2[i].owner_process == pid) {
            fHolderListE2[i].frame_pres = 0;
            fHolderListE2[i].owner_process = -1;
            fHolderListE2[i].vaddr = 0x0;
            counter++;
        }
    }
    return counter;
}

/*------------------------------------------------------------------------
 * ready_framewait_process -  Make framewait processes ready
 *------------------------------------------------------------------------
 */
void ready_framewait_process()
{
    // Check if framewait queue is empty
    if(nonempty(framewait)) {
        ready(dequeue(framewait));
    }
}


/*------------------------------------------------------------------------
 * deallocate_frames_E2 -  Deallocating frames in E2
 *------------------------------------------------------------------------
 */
void deallocate_frames_E2(v32addr_t start_vaddr, uint16 npages, pid32 owner_pid)
{
    uint32 i;
    /* Deallocate frames related to the virtual address */
	for(i = 0; i < npages; i++) {
        v32addr_t vaddr = start_vaddr + (i * NBPG);

		/* Free frame in region E2 */
		free_frame_in_regionE2(vaddr, owner_pid);
	}
}

/*------------------------------------------------------------------------
 * get_number_allocated_frames_E1 -  Get number of allocated frames in E1 (used only in tests_swapping.c)
                                    for process `pid`
 *------------------------------------------------------------------------
 */
uint32 get_number_allocated_frames_E1(pid32 pid)
{
    uint32 counter;
    int32 i;

    counter = 0;
    for(i = 0; i < NFRAMES_E1; i++) {
        if(fHolderListE1[i].frame_pres == 1 && fHolderListE1[i].owner_process == pid) {
            counter++;
        }
    }
    return counter;
}


/*------------------------------------------------------------------------
 * get_number_allocated_frames_E2 -  Get number of allocated frames in E2 (used only in tests_swapping.c)
                                    for process `pid`
 *------------------------------------------------------------------------
 */
uint32 get_number_allocated_frames_E2(pid32 pid)
{
    uint32 counter;
    int32 i;

    counter = 0;
    for(i = 0; i < NFRAMES_E2; i++) {
        if(fHolderListE2[i].frame_pres == 1 && fHolderListE2[i].owner_process == pid) {
            counter++;
        }
    }
    return counter;
}

/*------------------------------------------------------------------------
 * display_E1_oldest_frame_details -  Display fHolderList details of oldest frame in E1 (used only in tests_swapping.c)
 *------------------------------------------------------------------------
 */
void display_E1_oldest_frame_details(uint32 oldest_frame_index_E1, pid32 pid)
{
    kprintf("%d: fHolderListE1[oldest_frame_index_E1].vaddr: %d\n", pid, fHolderListE1[oldest_frame_index_E1].vaddr);
    kprintf("%d: fHolderListE1[oldest_frame_index_E1].owner_process: %d\n", pid, fHolderListE1[oldest_frame_index_E1].owner_process);
    kprintf("%d: fHolderListE1[oldest_frame_index_E1].time_counter: %d\n", pid, fHolderListE1[oldest_frame_index_E1].time_counter);
}


/*------------------------------------------------------------------------
 * proctab status - print the status of proctab
 *------------------------------------------------------------------------
 */
void display_proctab()
{

    intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	
    uint32 i;
    for(i=0;i<NPROC;i++)
    {
        if(proctab[i].prstate != PR_FREE)
        {
            kprintf("\npid: %d, prname:%s, prstate:%d\n", i, proctab[i].prname, proctab[i].prstate);
        }
    }

    restore(mask);
}