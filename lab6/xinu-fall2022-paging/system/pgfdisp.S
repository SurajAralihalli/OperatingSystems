/* pgfdisp.s - pgfdisp (x86) */

/*------------------------------------------------------------------------
 * pgfdisp  -  Interrupt dispatcher for page faults
 *------------------------------------------------------------------------
 */


#include <icu.s>
    	   .text
           .globl  pgfdisp
pgfdisp:
	pushal			# Save registers
	cli			# Disable further interrupts

	call	pgfhandler	# Call high level handler

	sti			# Restore interrupt status
	popal			# Restore registers
	addl $4, %esp
	iret			# Return from interrupt