#ifndef __KERN_SCHEDULE_SCHED_PRINT__
#define __KERN_SCHEDULE_SCHED_PRINT__

#include <defs.h>
#include <list.h>
#include <proc.h>
#include <assert.h>
#include <default_sched.h>
#include <RR_sched.h>
/*
 * 此文件用于打印调用enqueue、dequeue、pick_next、proc_time等函数时等待队列(run_queue)或斜堆()skew_heap的信息
 */
#define RR 0
#define STRIDE 1
static int queue_printf_time = 0;
static int queue_printf_time_limit = 1000;

static void skew_heap_DFS(skew_heap_entry_t *a)
{
    struct proc_struct *proc = le2proc(a, lab6_run_pool);
    cprintf("                             [ 0x%08lx, stride = 0x%08lx, priority = %u, ts = %u ]\n", proc, proc->lab6_stride, proc->lab6_priority, proc->time_slice);
    if (a->left)
        skew_heap_DFS(a->left);
    if (a->right)
        skew_heap_DFS(a->right);
}

static void Print_enqueue(struct run_queue *rq, struct proc_struct *proc, char *const str, int flag)
{
    if ((++queue_printf_time) >= queue_printf_time_limit)
        return;
    cprintf("      %s successfully\n", sched_fun[ENQUEUE]);
    cprintf("         %s call\n", str);
    cprintf("         enqueue proc: ( 0x%08lx ), parent: ( 0x%08lx )\n", proc, proc->parent);
    if (proc->time_slice == rq->max_time_slice)
        cprintf("         reset this proc time_slice: %u\n", proc->time_slice);
    /****************************************************/

    if (flag == RR)
    {
        cprintf("         run_queue[%2d proc]: [rq] <-", rq->proc_num);

        list_entry_t *le = &(rq->run_list);
        int i = 1;
        while ((le = list_next(le)) != &(rq->run_list))
            if (i == 1)
                cprintf(" 0x%08lx.[%d] \n", le2proc(le, run_link), i++);
            else
                cprintf("                                     0x%08lx.[%d] \n", le2proc(le, run_link), i++);
    }
    else if (flag == STRIDE)
    {
        cprintf("         skew_heap[%2d proc]: [rq->lab6_run_pool] -> \n", rq->proc_num);
        skew_heap_DFS(rq->lab6_run_pool);
    }
}

static void Print_dequeue(struct proc_struct *proc, int flag)
{
    if ((queue_printf_time) >= queue_printf_time_limit)
        return;
    cprintf("      %s\n", sched_fun[DEQUEUE]);
    /****************************************************/

    if (flag == RR)
        cprintf("         dequeue proc: 0x%08lx\n", proc);
    else if (flag == STRIDE)
        cprintf("         dequeue proc: [ 0x%08lx, stride = 0x%08lx, priority = %u ]\n", proc, proc->lab6_stride, proc->lab6_priority);
}

static void Print_pick_next(struct proc_struct *proc, int flag)
{
    if ((queue_printf_time) >= queue_printf_time_limit)
        return;
    cprintf("      %s\n", sched_fun[PICK_NEXT]);
    /****************************************************/

    if (flag == RR)
        cprintf("         pick    proc: 0x%08lx\n", proc);
    else if (flag == STRIDE)
        cprintf("         pick    proc: [ 0x%08lx, stride = 0x%08lx, priority = %u ]\n", proc, proc->lab6_stride, proc->lab6_priority);
}

#endif
