/* paging.h */

//typedef unsigned int	 bsd_t;

/* Structure for a page directory entry */

typedef struct {

  unsigned int pd_pres	: 1;		/* page table present?		*/
  unsigned int pd_write : 1;		/* page is writable?		*/
  unsigned int pd_user	: 1;		/* is use level protection?	*/
  unsigned int pd_pwt	: 1;		/* write through cachine for pt?*/
  unsigned int pd_pcd	: 1;		/* cache disable for this pt?	*/
  unsigned int pd_acc	: 1;		/* page table was accessed?	*/
  unsigned int pd_mbz	: 1;		/* must be zero			*/
  unsigned int pd_fmb	: 1;		/* four MB pages?		*/
  unsigned int pd_global: 1;		/* global (ignored)		*/
  unsigned int pd_avail : 3;		/* for programmer's use		*/
  unsigned int pd_base	: 20;		/* location of page table?	*/
} pd_t;

/* Structure for a page table entry */

typedef struct {

  unsigned int pt_pres	: 1;		/* page is present?		*/
  unsigned int pt_write : 1;		/* page is writable?		*/
  unsigned int pt_user	: 1;		/* is use level protection?	*/
  unsigned int pt_pwt	: 1;		/* write through for this page? */
  unsigned int pt_pcd	: 1;		/* cache disable for this page? */
  unsigned int pt_acc	: 1;		/* page was accessed?		*/
  unsigned int pt_dirty : 1;		/* page was written?		*/
  unsigned int pt_mbz	: 1;		/* must be zero			*/
  unsigned int pt_global: 1;		/* should be zero in 586	*/
  unsigned int pt_avail : 3;		/* for programmer's use		*/
  unsigned int pt_base	: 20;		/* location of page?		*/
} pt_t;


/* Prototypes for required API calls */
//SYSCALL xmmap(int, bsd_t, int);
//SYSCALL xmunmap(int);

#define NBPG		4096	/* number of bytes per page	*/
#define FRAME0		1024	/* zero-th frame		*/
#define NFRAMES		3072	/* number of frames		*/

#define ENTRIES_PER_FRAME 1024

#define MAP_SHARED 1
#define MAP_PRIVATE 2

#define FIFO 3
#define MYPOLICY 4

#define MAX_ID		7		/* You get 8 mappings, 0 - 7 */
#define MIN_ID          0


// NEW DATASTRUCTURES // 

#define NFRAMES_D		1000	/* number of frames		*/
#define NFRAMES_E1		1024
#define NFRAMES_E2		1048

#define REGIONSTART_D		1024	/* number of frames		*/
#define REGIONSTART_E1		2024
#define REGIONSTART_E2		3048
#define REGIONSTART_F		4096

// struct to indicate which frames are free
struct fholderD{
    unsigned int frame_pres	: 1;		/* frame is present? */
    pid32  owner_process;           /* Owner process ID */
    v32addr_t vaddr;                /* Virtual address */
    uint16 nentries_allocated;        /* Number of pages allocated*/  
};

struct fholderE1{
    unsigned int frame_pres	: 1;		/* frame is present? */
    pid32  owner_process;           /* Owner process ID */
    v32addr_t vaddr;                /* Virtual address */
    uint32 time_counter;
};

struct fholderE2{
    unsigned int frame_pres	: 1;		/* frame is present? */
    pid32  owner_process;           /* Owner process ID */
    v32addr_t vaddr;                /* Virtual address */
};


struct identityMapAddr {
  p32addr_t* page_table_addr;
  uint32   page_dir_index;
};

// extern	struct	fholder *fHolderListD;
// extern	struct	fholder *fHolderListE1;
// extern struct identityMapAddr *identityMapAddrList;


extern	struct	fholderD fHolderListD[];
extern	struct	fholderE1 fHolderListE1[];
extern	struct	fholderE2 fHolderListE2[];
extern  struct  identityMapAddr identityMapAddrList[];

/* Queue for blocked processes waiting on free frames in E1 when E2 is also full */
extern qid16 framewait;