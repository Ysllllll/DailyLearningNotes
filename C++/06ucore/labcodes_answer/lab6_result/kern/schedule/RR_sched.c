#include <sched_fun_print.h>

/*
 * struct run_queue                                  init:sched_init()
 * {
 *     list_entry_t run_list;           //RR_sched   init:RR_init()      modify:RR_enqueue() 
 *     unsigned int proc_num;           //RR_sched   init:RR_init()      modify:RR_enqueue() RR_dequeue()
 *     int max_time_slice;              //RR_sched   init:sched_init()
 *     skew_heap_entry_t *lab6_run_pool;
 * };
 * 
 * RR_sched调度策略
 * struct proc_struce
 * {
 *     struct run_queue *rq;            //RR_shced   init:alloc_proc()   modify:RR_enqueue()
 *     list_entry_t run_link;           //RR_shced   init:alloc_proc()   modify:RR_enqueue() RR_dequeue()
 *     int time_slice;                  //RR_shced   init:alloc_proc()   modify:RR_enqueue() RR_proc_tick()
 *     skew_heap_entry_t lab6_run_pool;              init:alloc_proc()
 *     uint32_t lab6_stride;                         init:alloc_proc()        
 *     uint32_t lab6_priority;                       init:alloc_proc()          
 * };
 */

static void RR_init(struct run_queue *rq)
{
    memcpy(sched_fun, RR_sched_fun, sizeof(RR_sched_fun));
    list_init(&(rq->run_list));
    rq->proc_num = 0;
}

/* 
 * 把某进程的进程控制块指针放入到rq队列末尾，且如果进程控制块的时间片为0，则需要把它重置为rq成员变量max_time_slice。
 * 这表示如果进程在当前的执行时间片已经用完，需要等到下一次有机会运行时，才能再执行一段时间。
 */
static void RR_enqueue(struct run_queue *rq, struct proc_struct *proc, char *const str)
{
    assert(list_empty(&(proc->run_link)));
    list_add_before(&(rq->run_list), &(proc->run_link));
    if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice)
    {
        proc->time_slice = rq->max_time_slice;
    }
    proc->rq = rq;
    rq->proc_num++;

    Print_enqueue(rq, proc, str, RR);
}

/* 
 * 把就绪进程队列rq的进程控制块指针的队列元素删除，并把表示就绪进程个数的proc_num减1
 */
static void RR_dequeue(struct run_queue *rq, struct proc_struct *proc)
{
    assert(!list_empty(&(proc->run_link)) && proc->rq == rq);
    list_del_init(&(proc->run_link));
    rq->proc_num--;

    Print_dequeue(proc, RR);
}

/* 
 * 选取就绪进程队列rq中的队头队列元素，并把队列元素转换成进程控制块指针。
 */
static struct proc_struct *RR_pick_next(struct run_queue *rq)
{
    list_entry_t *le = list_next(&(rq->run_list));
    if (le != &(rq->run_list))
    {
        struct proc_struct *proc = le2proc(le, run_link);
        Print_pick_next(proc, RR);
        return proc;
    }
    return NULL;
}

/* 
 * 每次timer到时后，trap函数将会间接调用此函数RR_proc_tick()来把当前执行进程的时间片time_slice减1。
 * 若time_slice=0，则设置此进程的成员变量need_resched = 1，在下一次中断来后，执行trap函数时，
 * 会由于当前进程的成员变量need_resched = 1而执行schedule函数，从而把当前执行进程放回就绪队列末尾，
 * 而从就绪队列头取出在就绪队列上等待时间最久的那个就行进程执行。
 */
static void RR_proc_tick(struct run_queue *rq, struct proc_struct *proc)
{
    if (proc->time_slice > 0)
    {
        proc->time_slice--;
    }
    if (proc->time_slice == 0)
    {
        proc->need_resched = 1;
    }
}

// struct sched_class default_sched_class = {
//     .name = "RR_scheduler",
//     .init = RR_init,
//     .enqueue = RR_enqueue,
//     .dequeue = RR_dequeue,
//     .pick_next = RR_pick_next,
//     .proc_tick = RR_proc_tick,
// };
