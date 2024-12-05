/* helper_functions.c */

#include <xinu.h>

/*------------------------------------------------------------------------
 * get_page_faulted_addr_cr2 -  get page faulted addr from cr2
 *------------------------------------------------------------------------
 */
v32addr_t get_page_faulted_addr_cr2()
{
    v32addr_t cr2_val = 0;
	asm("movl %%cr2, %0" 
		: "=r"(cr2_val));
	
    return cr2_val;
}

/*------------------------------------------------------------------------
 * set_page_dir_addr_cr3 -  set page dir addr in cr3 register
 *------------------------------------------------------------------------
 */
void set_page_dir_addr_cr3(p32addr_t page_dir_addr)
{
    p32addr_t cr3_val = 0;
	asm("movl %%cr3, %0" 
		: "=r"(cr3_val));
	cr3_val = (cr3_val & 0x00000FFF) | (page_dir_addr & 0xFFFFF000);
	asm("movl %0, %%cr3" 
		:
		: "r"(cr3_val));
}

/*------------------------------------------------------------------------
 * flush_tlb -  Flush TLB by updating CR3
 *------------------------------------------------------------------------
 */
void flush_tlb()
{
    p32addr_t cr3_val = 0;
	asm("movl %%cr3, %0" 
		: "=r"(cr3_val));
	asm("movl %0, %%cr3" 
		:
		: "r"(cr3_val));
}