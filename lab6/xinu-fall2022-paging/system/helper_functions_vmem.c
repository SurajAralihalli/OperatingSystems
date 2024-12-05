/* helper_functions_vmem.c */
#include <xinu.h>
/*------------------------------------------------------------------------
 * create_vmemblk_node -  Creates a new linked list node of type vmemblk
 *------------------------------------------------------------------------
 */
struct vmemblk* create_vmemblk_node()
{
    uint32 size = sizeof( struct vmemblk );
    struct vmemblk* vnode  =  (struct vmemblk*)getmem(size);

    vnode->start_addr = 0x0;
    vnode->mnext = NULL;
    vnode->npages = MAXHSIZE;

    return vnode;
}

/*------------------------------------------------------------------------
 * free_vmemblk_node -  Free a new linked list node of type vmemblk
 *------------------------------------------------------------------------
 */
void free_vmemblk_node(struct vmemblk* nodeptr)
{
    freemem((char* )nodeptr, sizeof(struct vmemblk));
}

/*------------------------------------------------------------------------
 * setup_vmemlist -  setup vmemlist in process table
 *------------------------------------------------------------------------
 */
void setup_vmemlist(pid32 pid)
{
    struct	procent	*prptr = &proctab[pid];

    //initialize the vmemlist
	prptr->vmemlist_ptr = create_vmemblk_node();

    prptr->vmemlist_ptr->npages = prptr->hsize;
    prptr->vmemlist_ptr->start_addr = 0x0;
    prptr->vmemlist_ptr->mnext = create_vmemblk_node();

    prptr->vmemlist_ptr->mnext->npages = prptr->hsize;
    prptr->vmemlist_ptr->mnext->start_addr = REGIONSTART_F * NBPG; // (4096 * 4096)
    prptr->vmemlist_ptr->mnext->mnext = NULL;
}

/*------------------------------------------------------------------------
 * purge_vmemlist -  purge vmemlist in process table
 *------------------------------------------------------------------------
 */
void purge_vmemlist()
{
    struct	procent* prptr = &proctab[currpid];
    struct vmemblk* curr, *next;


    curr = prptr->vmemlist_ptr;
	
	while (curr != NULL) {			/* Search free list	*/
        next = curr->mnext;

        /* Free linked list node */
        free_vmemblk_node(curr);
        curr = next;
	}

}
