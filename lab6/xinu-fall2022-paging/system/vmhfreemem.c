/* vmhfreemem.c */

#include <xinu.h>

/*------------------------------------------------------------------------
 * vmhfreemem  -  vmhfreemem() is a counterpart of freemem() with corresponding arguments for private heap in virtual memory, 
                  albeit with the second argument in unit of pages (not bytes)
 *------------------------------------------------------------------------
 */


syscall vmhfreemem(char *blockaddr, uint16 msize)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	vmemblk	*next, *prev, *vmemlist_ptr;
    v32addr_t vaddr;
	v32addr_t	top;

	mask = disable();

	uint32 hsize = (&proctab[currpid])->hsize;

	if ((msize == 0) || ((uint32) blockaddr < (uint32) (REGIONSTART_F * NBPG))
			  || ((uint32) blockaddr > (uint32) ((REGIONSTART_F + hsize) * NBPG))) {
		restore(mask);
		return SYSERR;
	}

	vaddr = (v32addr_t)blockaddr;

	/* Ensure that vaddr starts at 4KB boundary */
	vaddr = drop_offset_from_addr(vaddr);

	struct	procent	*prptr =  &proctab[currpid];
	vmemlist_ptr = prptr->vmemlist_ptr;

	prev = vmemlist_ptr;			/* Walk along free list	*/
	next = vmemlist_ptr->mnext;

	while ((next != NULL) && (next->start_addr < vaddr)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == vmemlist_ptr) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = prev->start_addr + (v32addr_t)(prev->npages * NBPG);
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != vmemlist_ptr) && (uint32) vaddr < top)
	    || ((next != NULL)	&& (uint32) (vaddr+msize*NBPG) > (v32addr_t)next->start_addr)) {
		restore(mask);
		return SYSERR;
	}

	vmemlist_ptr->npages += msize;


	struct	vmemblk	*vmemblock;
	/* Either coalesce with previous block or add to free list */

	if (top == vaddr) { 	/* Coalesce with previous block	*/
		prev->npages += msize;
		vmemblock = prev;
	} else {			/* Link into list as new node	*/
		vmemblock = create_vmemblk_node();
		vmemblock->start_addr = vaddr;
		vmemblock->mnext = next;
		vmemblock->npages = msize;
		prev->mnext = vmemblock;
	}

	/* Coalesce with next block if adjacent */

	if ( next!=NULL &&  ((vmemblock->start_addr + vmemblock->npages*NBPG) == next->start_addr)) {
		vmemblock->npages += next->npages;
		vmemblock->mnext = next->mnext;
		/* Free linked list node */
		free_vmemblk_node(next);
	}

	/* Add logic to deallocate frames in E2 */
	deallocate_frames_E2(vaddr, msize, currpid);

	/* Deallocate frames in E1 */
	deallocate_frames_E1(vaddr, msize, currpid);

	/* Invalidate page table entries in VD - Useful when all page table entries have P bit 0 */
	invalidate_page_table_entries(vaddr, msize, (p32addr_t*)prptr->page_dir_addr, currpid);	


	// Check if framewait queue is empty
	ready_framewait_process();

	restore(mask);
	return OK;
}