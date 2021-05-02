// the control struct for a set of vma using the same PDT
struct mm_struct
{
    list_entry_t mmap_list;        // linear list link which sorted by start addr of vma
    struct vma_struct *mmap_cache; // current accessed vma, used for speed purpose
    pde_t *pgdir;                  // the PDT of these vma
    int map_count;                 // the count of these vma
    void *sm_priv;                 // the private data for swap manager
    int mm_count;                  // the number ofprocess which shared the mm
    lock_t mm_lock;                // mutex for using dup_mmap fun to duplicat the mm
};
struct vma_struct
{
    struct mm_struct *vm_mm; // the set of vma using the same PDT
    uintptr_t vm_start;      // start addr of vma
    uintptr_t vm_end;        // end addr of vma
    uint32_t vm_flags;       // flags of vma
    list_entry_t list_link;  // linear list link which sorted by start addr of vma
};

/*****************************************************************************************************/
/* registers as pushed by pushal */
struct pushregs
{
    uint32_t reg_edi;
    uint32_t reg_esi;
    uint32_t reg_ebp;
    uint32_t reg_oesp; /* Useless */
    uint32_t reg_ebx;
    uint32_t reg_edx;
    uint32_t reg_ecx;
    uint32_t reg_eax;
};

struct trapframe
{
    struct pushregs tf_regs;
    uint16_t tf_gs;
    uint16_t tf_padding0;
    uint16_t tf_fs;
    uint16_t tf_padding1;
    uint16_t tf_es;
    uint16_t tf_padding2;
    uint16_t tf_ds;
    uint16_t tf_padding3;
    uint32_t tf_trapno;
    /* below here defined by x86 hardware */
    uint32_t tf_err;
    uintptr_t tf_eip;
    uint16_t tf_cs;
    uint16_t tf_padding4;
    uint32_t tf_eflags;
    /* below here only when crossing rings, such as from user to kernel */
    uintptr_t tf_esp;
    uint16_t tf_ss;
    uint16_t tf_padding5;
} __attribute__((packed));

/*****************************************************************************************************/
extern list_entry_t proc_list; //将进程控制块串联起来的双向链表头结点，在proc_init()中进行初始化，
// 在do_fork->set_links函数中会对该参数进行修改，显然是创建一个进程就将其加入到进程控制块队列中。
// list_add(&proc_list, &(proc->list_link));

// 这里对do_fork参数列表中的tf进行一个整理：
// 首先需要明白这个tf帧格式在进程切换中的作用：从A进程切换到B进程后能够跳转到B进程所属的tf帧中指定的位置执行，这个指定的位置，可以是主动指定也可以是被动指定。
// 然后内核线程和用户进程创建子进程时使用do_fork的方式不一样，内核线程在kernel_thread中使用do_fork；用户进程在中断trap->syscall中调用do_fork。
// 将do_fork和tf结合起来，在do_fork的参数列表中会有一个tf变量，两类线程在针对这个传进do_fork中的tf的构造不一样
// 内核线程创建子进程时将子进程的跳转接口进行了统一，也就是将tf的重建和do_fork的调用包装在了kernel_thread中。
// 用户进程重建子进程时，子进程的第一条执行指令没有进行统一，所以只需要使用中断时使用的这个tf帧
static int nr_process = 0; //操作系统中的进程总数

struct proc_struct
{
    enum proc_state state;                  // Process state
    int pid;                                // Process ID
    int runs;                               // the running times of Proces
    uintptr_t kstack;                       // Process kernel stack
    volatile bool need_resched;             // bool value: need to be rescheduled to release CPU?
    struct proc_struct *parent;             // the parent process
    struct mm_struct *mm;                   // Process's memory management field
    struct context context;                 // Switch here to run process
    struct trapframe *tf;                   // Trap frame for current interrupt
    uintptr_t cr3;                          // CR3 register: the base addr of Page Directroy Table(PDT)
    uint32_t flags;                         // Process flag
    char name[PROC_NAME_LEN + 1];           // Process name
    list_entry_t list_link;                 // Process link list
    list_entry_t hash_link;                 // Process hash list 在find_proc(pid)中根据pid快速找到该进程控制块
    int exit_code;                          // exit code (be sent to parent proc)
    uint32_t wait_state;                    // waiting state
    struct proc_struct *cptr, *yptr, *optr; // relations between processes
    struct run_queue *rq;                   // running queue contains Process 指向运行队列头结点proc_rq = rq
    list_entry_t run_link;                  // the entry linked in run queue 连接进运行队列中run_link->run_list
    int time_slice;                         // time slice for occupying the CPU
    skew_heap_entry_t lab6_run_pool;        // FOR LAB6 ONLY: the entry in the run pool
    uint32_t lab6_stride;                   // FOR LAB6 ONLY: the current stride of the process
    uint32_t lab6_priority;                 // FOR LAB6 ONLY: the priority of process, set by lab6_set_priority(uint32_t)
};

/*****************************************************************************************************/
static struct run_queue *rq;
static struct run_queue __rq; // 在sched_init(void)中会令rq = &__rq;即存在一个全局静态变量实体
                              // list_add_before(&(rq->run_list), &(proc->run_link));
                              // procA.rq->rq
                              // idle.rq->proc_num++
struct run_queue
{
    list_entry_t run_list; //
    unsigned int proc_num;
    int max_time_slice;
    // For LAB6 ONLY
    skew_heap_entry_t *lab6_run_pool;
};