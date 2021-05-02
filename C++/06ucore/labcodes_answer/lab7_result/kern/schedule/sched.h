#ifndef __KERN_SCHEDULE_SCHED_H__
#define __KERN_SCHEDULE_SCHED_H__

#include <defs.h>
#include <list.h>
#include <skew_heap.h>

#define MAX_TIME_SLICE 5

struct proc_struct;

/* 一个timer_t在系统中的存活周期可以被描述如下：
 * 1. timer_t在某个位置被创建和初始化，并通过add_timer加入系统管理列表中；
 * 2. 系统时间被不断累加，直到run_timer_list发现该timer_t到期；
 * 3. run_timer_lsit更改对应的进程状态，并从系统管理列表中移除该timer_t；
 * 
 * 定义了timer_t的基本结构，可以用timer_init函数对其进行初始化
 */
typedef struct
{
    unsigned int expires;
    struct proc_struct *proc;
    list_entry_t timer_link;
} timer_t;

#define le2timer(le, member) \
    to_struct((le), timer_t, member)

//对某计时器进行初始化，让它在expires时间片之后唤醒proc进程
static inline timer_t *timer_init(timer_t *timer, struct proc_struct *proc, int expires)
{
    timer->expires = expires;
    timer->proc = proc;
    list_init(&(timer->timer_link));
    return timer;
}

struct run_queue;

// The introduction of scheduling classes is borrrowed from Linux, and makes the
// core scheduler quite extensible. These classes (the scheduler modules) encapsulate
// the scheduling policies.
struct sched_class
{
    // the name of sched_class
    const char *name;
    // Init the run queue
    void (*init)(struct run_queue *rq);
    // put the proc into runqueue, and this function must be called with rq_lock
    void (*enqueue)(struct run_queue *rq, struct proc_struct *proc, char *const str);
    // get the proc out runqueue, and this function must be called with rq_lock
    void (*dequeue)(struct run_queue *rq, struct proc_struct *proc);
    // choose the next runnable task
    struct proc_struct *(*pick_next)(struct run_queue *rq);
    // dealer of the time-tick
    void (*proc_tick)(struct run_queue *rq, struct proc_struct *proc);
    /* for SMP support in the future
     *  load_balance
     *     void (*load_balance)(struct rq* rq);
     *  get some proc from this rq, used in load_balance,
     *  return value is the num of gotten proc
     *  int (*get_proc)(struct rq* rq, struct proc* procs_moved[]);
     */
};

struct run_queue
{
    list_entry_t run_list; //其运行队列的哨兵结构，可以看作是队列头和尾
    unsigned int proc_num; //表示其内部的进程总数
    int max_time_slice;    //每个进程一轮占用的最多时间片
    // For LAB6 ONLY
    skew_heap_entry_t *lab6_run_pool; //优先队列形式的进程容器，只在lab6中使用
};

void sched_init(void);
void wakeup_proc(struct proc_struct *proc);
void schedule(char *const str);
void add_timer(timer_t *timer);
void del_timer(timer_t *timer, char *const str);
void run_timer_list(void);
void print_timer_list(const char *str);

// syscall define as fellow:
//       #define SYS_exit 1
//       #define SYS_fork 2
//       #define SYS_wait 3
//       #define SYS_exec 4
//       #define SYS_clone 5
//       #define SYS_yield 10
//       #define SYS_sleep 11
//       #define SYS_kill 12
//       #define SYS_gettime 17
//       #define SYS_getpid 18
//       #define SYS_brk 19
//       #define SYS_mmap 20
//       #define SYS_munmap 21
//       #define SYS_shmem 22
//       #define SYS_putc 30
//       #define SYS_pgdir 31

// 打印 schedule函数 调用信息
static int schedule_print_limit = 1000;
static int call_schedule_times = 0;
static int call_schedule_times_successfully = 0;

// 打印 调用schedule()函数 的 函数名
// lab7中调用schedule()函数的有："do_exit()","do_wait()","init_main()",
//                              "cpu_idle()","lock()","trap()","do_sleep()","__down()"
#define DO_EXIT 0
#define DO_WAIT 1
#define INIT_MAIN 2
#define CPU_IDLE 3
#define LOCK 4
#define TRAP 5
#define DO_SLEEP 6
#define __DOWN 7
static char *const call_schedule_fun[] = {"do_exit()",
                                          "do_wait()",
                                          "init_main()",
                                          "cpu_idle()",
                                          "lock()",
                                          "trap()",
                                          "do_sleep()",
                                          "__down()"};

// 打印 调用sched_class_enqueue()函数 的 函数名
// lab7中wakeup_proc()和schedule()函数会调用sched_class_enqueue()
#define WAKEUP_PROC 0
#define SCHEDULE 1
static char *const call_enqueue_fun[] = {"wakeup_proc()", "schedule()"};

// 打印 调用del_timer()函数 的 函数名
// lab7中do_sleep()和run_timer_list()函数会调用del_timer()
#define del_timer_DO_SLEEP 0
#define del_timer_RUN_TIMER_LIST 1
static char *const call_del_timer_fun[] = {"do_sleep()", "run_timer_list()"};
extern list_entry_t timer_list;

// 打印 sched_class_fun函数调用 的 函数名
#define ENQUEUE 0
#define DEQUEUE 1
#define PICK_NEXT 2
#define PROC_TICK 3
static char *const RR_sched_fun[4] = {"RR_enqueue()", "RR_dequeue()", "RR_pick_next()", "RR_proc_tick()"};
static char *const stride_sched_fun[4] = {"stride_enqueue()", "stride_dequeue()", "stride_pick_next()", "stride_proc_tick()"};

/* 需求(*.c文件均包含sched.h头文件)：
 *                                       sched.h(声明int i)
 * sched.c(定义int i = 5)            RR_sched.c(使用int a = i;)          default_sched.c(使用int b = i;)
 * 
 * 
 * 以下×并不会报错，而是无法使用定义的值，√代表能够使用定义的值
 * static声明方式：
 *                                      sched.h(static int i)
 *    sched.c(i = 5)                RR_sched.c(i = 6;)×              default_sched.c(i = 7;)× 仅有这种能够运行，但是逻辑错误
 *    不可使用(int i;)进行定义，只能使用(i = ...;)，在定义的xx.c中能够正确使用i变量，在其它**.c文件中使用i变量没法得到它在xx.c文件的定义。
 * 
 * const声明方式：
 *                                      sched.h(const int i)
 *    sched.c(i = 5)                RR_sched.c(i = 10;)√             default_sched.c(i = 12;)√
 *    不可使用(int i;)进行定义，只能使用(i = ...;)，在定义的xx.c中能够正确使用i变量，在其它**.c文件中使用i变量[能够]得到它在xx.c文件的定义。
 * 
 * extern声明方式：
 *                                       sched.h(extern int i)
 *    sched.c(i = 10;)              RR_sched.c(i = 11;)×               default_sched.c(i = 12;)× 仅有这一种会产生错误
 *    sched.c(int i = 10;)          RR_sched.c(i = 11;)√               default_sched.c(i = 12;)√ <——|——等价
 *    sched.c(int i;)               RR_sched.c(i = 11;)√               default_sched.c(i = 12;)√ <——|
 *    sched.c(i = 10;)              RR_sched.c(int i;)√                default_sched.c(i = 12;)√
 *    sched.c(i = 10;)              RR_sched.c(int i = 11;)√           default_sched.c(i = 12;)√
 *    sched.c(i = 10;)              RR_sched.c(i = 11;)√               default_sched.c(int i;)√
 *    sched.c(i = 10;)              RR_sched.c(i = 11;)√               default_sched.c(int i = 12;)√
 *    sched.c(int i;)               RR_sched.c(int i;)√                default_sched.c(int i;)√
 *                                      ...任意组合，但是必须保证有一个.c文件定义了i：(int i)或者(int i = 10)
 */
extern char *sched_fun[4]; //关于该语句的使用见上

#endif /* !__KERN_SCHEDULE_SCHED_H__ */
