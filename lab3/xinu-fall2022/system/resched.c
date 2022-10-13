/* resched.c - resched, resched_cntl */

#include <xinu.h>


struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			// Update currstop 
			currstop = getticks();
			// set curpid's prtotalcpu
			ptold->prtotalcpu += (uint32)(currstop - currstart) / (double)389;
			// Update currstart
			currstart = getticks();

			// Reset preemptionType flag (if other processes became ready but the process still has highest priority and was not preempted)
			preemptionType =0;
			return;
		}

		// check preemptionType
		if(preemptionType == 1)
		{
			ptold->prpreemptcount1++;
		}
		else if(preemptionType == 2)
		{
			ptold->prpreemptcount2++;
		}

		// Reset preemptionType flag
		preemptionType =0;

		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	// Update currstop for all processes (suspended or curr)
	currstop = getticks();
	// set curpid's prtotalcpu
	ptold->prtotalcpu += (uint32)(currstop - currstart) / (double)389;
		
	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	ptnew->prcurrcount++;  // increment when process context swithes in
	preempt = QUANTUM;		/* Reset time slice for process	*/

	// update currstart
	currstart = getticks();
	// kprintf("\n%s currstart:%d\n",ptnew->prname,currstop);
	// update prtotalresponse in system time
	ptnew->prtotalresponse += (uint32)(currstart - ptnew->prreadystart);
	// update prmaxresponse in millisec
	uint32 newResponseTime = (uint32)(currstart - ptnew->prreadystart) / (double)(389 * 1000);
	ptnew->prmaxresponse = newResponseTime > ptnew->prmaxresponse ? newResponseTime : ptnew->prmaxresponse; 

	// context switch
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
