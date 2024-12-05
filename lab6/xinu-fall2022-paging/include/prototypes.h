/* in file addargs.c */
extern	status	addargs(pid32, int32, int32[], int32,char *, void *);

/* in file allocate_bs.c */
extern	bsd_t	allocate_bs(unsigned int);

/* in file arp.c */

extern	void	arp_init();
extern	status	arp_resolve(uint32, byte[]);
extern	void	arp_in(struct arppacket *);
extern	int32	arp_alloc();
extern	void	arp_ntoh(struct arppacket *);
extern	void	arp_hton(struct arppacket *);

/* in file ascdate.c */

extern	status	ascdate(uint32, char *);

/* in file bufinit.c */

extern	status	bufinit(void);

/* in file chprio.c */

extern	pri16	chprio(pid32, pri16);

/* in file clkupdate.S */

extern	uint32	clkcount(void);

/* in file clkhandler.c */

extern	interrupt clkhandler(void);

/* in file clkinit.c */

extern	void	clkinit(void);

/* in file clkdisp.S */

extern	void	clkdisp(void);

/* in file close.c */

extern	syscall	close(did32);

/* in file close_bs.c */
extern	bsd_t	close_bs(bsd_t);

/* in file control.c */

extern	syscall	control(did32, int32, int32, int32);

/* in file create.c */

extern	pid32	create(void *, uint32, pri16, char *, uint32, ...);

/* in file ctxsw.S */

extern	void	ctxsw(void *, void *);

/* in file deallocate_bs.c */
extern	bsd_t	deallocate_bs(bsd_t);

/* in file dhcp.c */

extern	uint32	getlocalip(void);

/* in file dot2ip.c */

extern	uint32	dot2ip(char *, uint32 *);

/* in file queue.c */

extern	pid32	enqueue(pid32, qid16);
extern	pid32	dequeue(qid16);

/* in file intutils.S */

extern	intmask	disable(void);

/* in file intutils.S */

extern	void	enable(void);

/* in file ethinit.c */
extern 	int32	ethinit(struct dentry *);

/* in file ethhandler.c */
extern 	interrupt	ethhandler();

/* in file ethcontrol.c */
extern 	devcall ethcontrol(struct dentry *, int32, int32, int32);

/* in file ethdispatch.S */
extern	void	ethdispatch(void);

/* in file ethmcast.c */
extern	int32	ethmcast_add(struct ethcblk *, byte[]);
extern	int32	ethmcast_remove(struct ethcblk *, byte[]);

/* in file ethread.c */
extern  devcall ethread(struct dentry *, char *, int32);

/* in file ethwrite.c */
extern 	devcall ethwrite(struct dentry *, char *buf, int32);

/* in file kprintf.c */
extern int console_init(void);

/* in file early_imr.c */
extern int remove_irm_protections(void);

/* in file evec.c */
extern	int32	initevec(void);
extern	int32	set_evec(uint32, uint32);
extern	void	trap(int32, long *);

/* in file exception.c */
extern  void exception(int32, int32*);

/* in file freebuf.c */
extern	syscall	freebuf(char *);

/* in file freemem.c */
extern	syscall	freemem(char *, uint32);

/* in file get_bs.c */
extern	syscall	get_bs(bsd_t, uint32);

/* in file getbuf.c */
extern	char	*getbuf(bpid32);

/* in file getc.c */
extern	syscall	getc(did32);

/* in file getitem.c */
extern	pid32	getfirst(qid16);
extern	pid32	getlast(qid16);
extern	pid32	getitem(pid32);

/* in file getmem.c */
extern	char	*getmem(uint32);

/* in file getpid.c */
extern	pid32	getpid(void);

/* in file getprio.c */
extern	syscall	getprio(pid32);

/* in file getstk.c */
extern	char	*getstk(uint32);

/* in file gettime.c */
extern	status	gettime(uint32 *);

/* in file getutime.c */
extern	status	getutime(uint32 *);

/* in file halt.S */
extern	void	halt(void);

/* in file icmp.c */

extern	void	icmp_init(void);
extern	void	icmp_in(struct netpacket *);
extern	int32	icmp_register(uint32);
extern	int32	icmp_recv(int32, char *, int32, uint32);
extern	status	icmp_send(uint32, uint16, uint16, uint16, char *, int32);
extern	struct	netpacket *icmp_mkpkt(uint32, uint16, uint16, uint16, char *, int32);
extern	status	icmp_release(int32);
extern	uint16	icmp_cksum(char *, int32);
extern	void	icmp_hton(struct netpacket *);
extern	void	icmp_ntoh(struct netpacket *);

/* in file init.c */
extern	syscall	init(did32);

/* in file initialize.c */
extern	int32	sizmem(void);

/* in file insert.c */
extern	status	insert(pid32, qid16, int32);

/* in file insertd.c */
extern	status	insertd(pid32, qid16, int32);

/* in file intr.S */
extern	uint16	getirmask(void);

/* in file ioerr.c */
extern	devcall	ioerr(void);

/* in file ionull.c */
extern	devcall	ionull(void);

/* in file ip.c */

extern	void	ip_in(struct netpacket *);
extern	status	ip_send(struct netpacket *);
extern	void	ip_local(struct netpacket *);
extern	status	ip_out(struct netpacket *);
extern	int32	ip_route(uint32);
extern	uint16	ipcksum(struct netpacket *);
extern	void	ip_ntoh(struct netpacket *);
extern	void	ip_hton(struct netpacket *);
extern	process	ipout(void);
extern	status	ip_enqueue(struct netpacket *);

/* in file net.c */

extern	void	net_init(void);
extern	process	netin();
extern	process	netout(void);
extern	process	rawin(void);
extern	void	eth_hton(struct netpacket *);
extern	void	eth_ntoh(struct netpacket *);

/* in file netstart.c */

extern	void	netstart(void);

/* in file kill.c */

extern	syscall	kill(pid32);

/* in file lexan.c */

extern	int32	lexan(char *, int32, char *, int32 *, int32 [], int32 []);

/* in file lfibclear.c */

extern	void	lfibclear(struct lfiblk *, int32);

/* in file lfibget.c */

extern	void	lfibget(did32, ibid32, struct lfiblk *);

/* in file lfibput.c */
extern	status	lfibput(did32, ibid32, struct lfiblk *);

/* in file lfdbfree.c */
extern	status	lfdbfree(did32, dbid32);

/* in file lfdballoc.c */
extern	dbid32	lfdballoc(struct lfdbfree *);

/* in file lfflush.c */
extern	status	lfflush(struct lflcblk *);

/* in file lfgetmode.c */
extern	int32	lfgetmode(char *);

/* in file lfiballoc.c */
extern	ibid32	lfiballoc(void);

/* in file lflclose.c */
extern	devcall	lflclose(struct dentry *);

/* in file lflcontrol.c */
extern	devcall	lflcontrol(struct dentry *, int32, int32, int32);

/* in file lflgetc.c */
extern	devcall	lflgetc(struct dentry *);

/* in file lflinit.c */
extern	devcall	lflinit(struct dentry *);

/* in file lflputc.c */
extern	devcall	lflputc(struct dentry *, char);

/* in file lflread.c */
extern	devcall	lflread(struct dentry *, char *, int32);

/* in file lflseek.c */
extern	devcall	lflseek(struct dentry *, uint32);

/* in file lflwrite.c */
extern	devcall	lflwrite(struct dentry *, char *, int32);

/* in file lfscreate.c */
extern  status  lfscreate(did32, ibid32, uint32);

/* in file lfsinit.c */
extern	devcall	lfsinit(struct dentry *);

/* in file lfsopen.c */
extern	devcall	lfsopen(struct dentry *, char *, char *);

/* in file lfsetup.c */
extern	status	lfsetup(struct lflcblk *);

/* in file lftruncate.c */
extern	status	lftruncate(struct lflcblk *);

/* in file lpgetc.c */
extern	devcall	lpgetc(struct dentry *);

/* in file lpinit.c */
extern	devcall	lpinit(struct dentry *);

/* in file lpopen.c */
extern	devcall	lpopen(struct dentry *, char *, char *);

/* in file lpputc.c */
extern	devcall	lpputc(struct dentry *, char);

/* in file lpread.c */
extern	devcall	lpread(struct dentry *, char *, int32);

/* in file lpwrite.c */
extern	devcall	lpwrite(struct dentry *, char *, int32);

/* in file mark.c */
extern	void	markinit(void);
extern	status	mark(int32 *);

/* in file memcpy.c */
extern	void	*memcpy(void *, const void *, int32);

/* in file memcpy.c */
extern	int32	*memcmp(void *, const void *, int32);

/* in file memset.c */
extern  void    *memset(void *, const int, int32);

/* in file mkbufpool.c */
extern	bpid32	mkbufpool(int32, int32);

/* in file mount.c */
extern	syscall	mount(char *, char *, did32);
extern	int32	namlen(char *, int32);

/* in file naminit.c */
extern	status	naminit(void);

/* in file nammap.c */
extern	devcall	nammap(char *, char[], did32);
extern	did32	namrepl(char *, char[]);
extern	status	namcpy(char *, char *, int32);

/* in file namopen.c */
extern	devcall	namopen(struct dentry *, char *, char *);

/* in file newqueue.c */
extern	qid16	newqueue(void);

/* in file open.c */
extern	syscall	open(did32, char *, char *);

/* in file open_bs.c */
extern	bsd_t	open_bs(bsd_t);

/* in file panic.c */
extern	void	panic(char *);

/* in file pci.c */
extern	int32	pci_init(void);

/* in file pdump.c */
extern	void	pdump(struct netpacket *);
extern	void	pdumph(struct netpacket *);

/* in file platinit.c */
extern	void	platinit();

/* in file psinit.c */
extern	int32	psinit(void);

/* in file ptclear.c */
extern	void	_ptclear(struct ptentry *, uint16, int32 (*)(int32));

/* in file ptcount.c */
extern	int32	ptcount(int32);

/* in file ptcreate.c */
extern	syscall	ptcreate(int32);

/* in file ptdelete.c */
extern	syscall	ptdelete(int32, int32 (*)(int32));

/* in file ptinit.c */
extern	syscall	ptinit(int32);

/* in file ptrecv.c */
extern	uint32	ptrecv(int32);

/* in file ptreset.c */
extern	syscall	ptreset(int32, int32 (*)(int32));

/* in file ptsend.c */
extern	syscall	ptsend(int32, umsg32);

/* in file putc.c */
extern	syscall	putc(did32, char);

/* in file quark_irq.c */
extern	int32	quark_irq_routing(void);

/* in file ramclose.c */
extern	devcall	ramclose(struct dentry *);

/* in file ramInit.c */
extern	devcall	raminit(struct dentry *);

/* in file ramopen.c */

extern	devcall	ramopen(struct dentry *, char *, char *);

/* in file ramread.c */
extern	devcall	ramread(struct dentry *, char *, int32);

/* in file ramwrite.c */
extern	devcall	ramwrite(struct dentry *, char *, int32);

/* in file rdsclose.c */
extern	devcall	rdsclose(struct dentry *);

/* in file rdscontrol.c */
extern	devcall	rdscontrol(struct dentry *, int32, int32, int32);

/* in file rdsinit.c */
extern	devcall	rdsinit(struct dentry *);

/* in file rdsopen.c */
extern	devcall	rdsopen(struct dentry *, char *, char *);

/* in file rdsread.c */
extern	devcall	rdsread(struct dentry *, char *, int32);

/* in file rdswrite.c */
extern	devcall	rdswrite(struct dentry *, char *, int32);

/* in file rdsbufalloc.c */
extern	struct	rdbuff * rdsbufalloc(struct rdscblk *);

/* in file rdscomm.c */
extern	status	rdscomm(struct rd_msg_hdr *, int32, struct rd_msg_hdr *,
		int32, struct rdscblk *);

/* in file rdsprocess.c */
extern	void	rdsprocess(struct rdscblk *);

/* in file read_bs.c */
extern	syscall	read_bs(char *, bsd_t, int32);

/* in file release_bs.c */
extern	syscall	release_bs(bsd_t);

/* in file sdmcclose.c */
extern	devcall	sdmcclose(struct dentry *);

/* in file sdmcinit.c */
extern	devcall	sdmcinit(struct dentry *);

/* in file sdmcopen.c */
extern	devcall	sdmcopen(struct dentry *, char *, char *);

/* in file sdmcread.c */
extern	devcall	sdmcread(struct dentry *, char *, int32);

/* in file sdmcwrite.c */
extern	devcall	sdmcwrite(struct dentry *, char *, int32);

/* in file sdmcdispatch.S */
extern	interrupt	sdmcdispatch(void);

/* in file read.c */
extern	syscall	read(did32, char *, uint32);

/* in file ready.c */
extern	status	ready(pid32);

/* in file receive.c */
extern	umsg32	receive(void);

/* in file recvclr.c */
extern	umsg32	recvclr(void);

/* in file recvtime.c */
extern	umsg32	recvtime(int32);

/* in file resched.c */
extern	void	resched(void);
extern	status	resched_cntl(int32);

/* in file intutils.S */
extern	void	restore(intmask);

/* in file resume.c */
extern	pri16	resume(pid32);

/* in file rfsgetmode.c */
extern	int32	rfsgetmode(char * );

/* in file rflclose.c */
extern	devcall	rflclose(struct dentry *);

/* in file rfscontrol.c */
extern	devcall	rfscontrol(struct dentry *, int32, int32, int32);

/* in file rflgetc.c */
extern	devcall	rflgetc(struct dentry *);

/* in file rflinit.c */
extern	devcall	rflinit(struct dentry *);

/* in file rflputc.c */
extern	devcall	rflputc(struct dentry *, char );

/* in file rflread.c */
extern	devcall	rflRead(struct dentry *, char *, int32 );

/* in file rflseek.c */
extern	devcall	rflseek(struct dentry *, uint32 );

/* in file rflwrite.c */
extern	devcall	rflwrite(struct dentry *, char *, int32 );

/* in file rfsndmsg.c */
extern	status	rfsndmsg(uint16, char *);

/* in file rfsinit.c */
extern	devcall	rfsinit(struct dentry *);

/* in file rfsopen.c */
extern	devcall	rfsopen(struct dentry  *devptr, char *, char *);

/* in file rfscomm.c */
extern	int32	rfscomm(struct rf_msg_hdr *, int32, struct rf_msg_hdr *, int32);

/* in file rdsClose.c */
extern	devcall	rdsClose(struct dentry *);

/* in file rdsControl.c */
extern	devcall	rdsControl(struct dentry *, int32, int32, int32);

/* in file rdsInit.c */
extern	devcall	rdsInit(struct dentry *);

/* in file rdsOpen.c */
extern	devcall	rdsOpen(struct dentry *, char *, char *);

/* in file rdsRead.c */
extern	devcall	rdsRead(struct dentry *, char *, int32);

/* in file rdsWrite.c */
extern	devcall	rdsWrite(struct dentry *, char *, int32);

/* in file rdsbufalloc.c */
extern	struct	rdbuff * rdsbufalloc(struct rdscblk *);

/* in file rdscomm.c */
extern	status	rdscomm(struct rd_msg_hdr *, int32, struct rd_msg_hdr *, int32, struct rdscblk *);

/* in file rdsprocess.c */
extern	void	rdsprocess(struct rdscblk *);

/* in file seek.c */
extern	syscall	seek(did32, uint32);

/* in file semcount.c */
extern	syscall	semcount(sid32);

/* in file semcreate.c */
extern	sid32	semcreate(int32);

/* in file semdelete.c */
extern	syscall	semdelete(sid32);

/* in file semreset.c */
extern	syscall	semreset(sid32, int32);

/* in file send.c */
extern	syscall	send(pid32, umsg32);

/* in file shell.c */
extern 	process shell(did32);

/* in file signal.c */
extern	syscall	signal(sid32);

/* in file signaln.c */
extern	syscall	signaln(sid32, int32);

/* in file sleep.c */
extern	syscall	sleepms(uint32);
extern	syscall	sleep(uint32);

/* in file start.S */
extern	int32	inb(int32);
extern	int32	inw(int32);
extern	int32	inl(int32);
extern	int32	outb(int32, int32);
extern	int32	outw(int32, int32);
extern	int32	outl(int32, int32);
extern	int32	outsw(int32, int32, int32);
extern	int32	insw(int32, int32 ,int32);

/* in file suspend.c */
extern	syscall	suspend(pid32);

/* in file ttycontrol.c */
extern	devcall	ttycontrol(struct dentry *, int32, int32, int32);

/* in file ttydispatch.c */
extern	interrupt	ttydispatch(void);

/* in file ttygetc.c */
extern	devcall	ttygetc(struct dentry *);

/* in file ttyhandle_in.c */
extern	void	ttyhandle_in(struct ttycblk *, struct uart_csreg *);

/* in file ttyhandle_out.c */
extern	void	ttyhandle_out(struct ttycblk *, struct uart_csreg *);

/* in file ttyhandler.c */
extern	void	ttyhandler(void);

/* in file ttyinit.c */
extern	devcall	ttyinit(struct dentry *);

/* in file ttykickout.c */
extern	void	ttykickout(struct uart_csreg *);

/* in file ttyputc.c */
extern	devcall	ttyputc(struct dentry *, char);

/* in file ttyread.c */
extern	devcall	ttyread(struct dentry *, char *, int32);

/* in file ttywrite.c */
extern	devcall	ttywrite(struct dentry *, char *, int32);

/* in file udp.c */

extern	void	udp_init(void);
extern	void	udp_in(struct netpacket *);
extern	uid32	udp_register(uint32, uint16, uint16);
extern	int32	udp_recv(uid32, char *, int32, uint32);
extern	int32	udp_recvaddr(uid32, uint32 *, uint16 *, char *, int32, uint32);
extern	status	udp_send(uid32, char *, int32);
extern	status	udp_sendto(uid32, uint32, uint16, char *, int32);
extern	status	udp_release(uid32);
extern	void	udp_ntoh(struct netpacket *);
extern	void	udp_hton(struct netpacket *);


/* in file unsleep.c */
extern	syscall	unsleep(pid32);

/* in file userret.c */
extern	void	userret(void);

/* in file wait.c */
extern	syscall	wait(sid32);

/* in file wakeup.c */
extern	void	wakeup(void);

/* in file write_bs.c */
extern	syscall	write_bs(char *, bsd_t, int32);

/* in file write.c */
extern	syscall	write(did32, char *, uint32);

/* in file xdone.c */
extern	void	xdone(void);

/* in file yield.c */
extern	syscall	yield(void);

/* NETWORK BYTE ORDER CONVERSION NOT NEEDED ON A BIG-ENDIAN COMPUTER */
#define	htons(x)   ( ( 0xff & ((x)>>8) ) | ( (0xff & (x)) << 8 ) )
#define	htonl(x)   (  (((x)>>24) & 0x000000ff) | (((x)>> 8) & 0x0000ff00) | \
		      (((x)<< 8) & 0x00ff0000) | (((x)<<24) & 0xff000000) )
#define	ntohs(x)   ( ( 0xff & ((x)>>8) ) | ( (0xff & (x)) << 8 ) )
#define	ntohl(x)   (  (((x)>>24) & 0x000000ff) | (((x)>> 8) & 0x0000ff00) | \
		      (((x)<< 8) & 0x00ff0000) | (((x)<<24) & 0xff000000) )



/* NEW FUNCTION PROTOTYPES START HERE */

/* in file vmhgetmem.c */
extern char *vmhgetmem(uint16 msize);

/* in file vmhfreemem.c */
extern syscall vmhfreemem(char *blockaddr, uint16 msize);

/* in file init_paging.c */
extern void init_paging(void);

/* in file pgfhandler.c */
extern void pgfhandler();

/* in file pgfdisp.S */
extern	void	pgfdisp(void);

/* in file helperFunctions.c */
extern char* get_empty_frame_from_regionD(pid32 pid);

extern char* get_empty_frame_from_regionE1();

extern void initialize_empty_page_directory(p32addr_t* page_dir_addr);

extern void initialize_empty_page_table(p32addr_t* page_tab_addr);

extern void build_identity_map_entry(p32addr_t* page_tab_addr, uint32 page_dir_index);

extern void set_page_directory_entry(pd_t* page_dir_entry, p32addr_t page_table_addr);

extern void set_page_table_entry(pt_t* page_table_entry, p32addr_t page_addr);

extern void reset_page_table_entry(pt_t* page_table_entry);

extern void reset_page_directory_entry(pd_t* page_directory_entry);

extern void initialize_fholderList();

extern pt_t* get_page_table_entry(v32addr_t page_faulted_addr, pd_t* page_dir_entry);

extern pd_t* get_page_directory_entry(v32addr_t page_faulted_addr, p32addr_t* page_dir_addr);

extern void deallocate_frames_E1(v32addr_t start_vaddr, uint16 npages, pid32 owner_pid);

extern void invalidate_page_table_entries(v32addr_t start_vaddr, uint16 npages, p32addr_t* page_dir_addr, pid32 owner_pid);

extern void free_frame_in_regionD(v32addr_t vaddr, pid32 owner_pid);

extern void free_frame_in_regionE1(v32addr_t vaddr, pid32 owner_pid);

extern uint32 purge_frames_fHolderListE1(pid32 pid);

extern void purge_frames_fHolderListD(pid32 pid);

extern void increment_number_entries_allocated(uint16 frame_index);

extern void decrement_number_entries_allocated(uint16 frame_index);

extern v32addr_t drop_offset_from_addr(v32addr_t vaddr);

extern uint16 is_read_write_access_violation(uint32 page_fault_error_code, pt_t* page_table_entry);

extern int is_addr_allocated_by_vmhgetmem(v32addr_t addr);

extern struct vmemblk* create_vmemblk_node();

extern void free_vmemblk_node(struct vmemblk* nodeptr);

extern void purge_vmemlist();

extern char* vmhgetstk(uint16 msize);

extern syscall	vmhalloc(uint32 hsize);

extern void setup_vmemlist(pid32 pid);

extern void set_page_dir_addr_cr3(p32addr_t page_dir_addr);

extern v32addr_t get_page_faulted_addr_cr2();

extern void flush_tlb();

/* in file helper_functions_swapping.c */
extern int get_index_oldest_frame_regionE1();

extern int get_matching_frame_from_regionE2(v32addr_t vaddr);

extern void free_frame_in_regionE2(v32addr_t vaddr, pid32 owner_pid);

extern char* get_empty_frame_from_regionE2(v32addr_t vaddr, pid32 pid);

extern void ready_framewait_process();

extern uint32 purge_frames_fHolderListE2(pid32 pid);

extern void deallocate_frames_E2(v32addr_t start_vaddr, uint16 npages, pid32 owner_pid);


/* Used only for testing */
extern uint32 get_number_allocated_frames_E1(pid32 pid);

extern uint32 get_number_allocated_frames_E2(pid32 pid);

extern void display_E1_oldest_frame_details(uint32 oldest_frame_index_E1, pid32 pid);

extern void display_proctab();

/* in tests.c */
extern void test_vmhgetmem(int test_num);

/* in tests_swapping.c */
extern void test_swapping(int test_num);