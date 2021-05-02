#ifndef __KERN_SCHEDULE_SCHED_PRINT__
#define __KERN_SCHEDULE_SCHED_PRINT__

#include <defs.h>
#include <list.h>
#include <proc.h>
#include <assert.h>
#include <default_sched.h>
/*
 * 此文件用于打印调用enqueue、dequeue、pick_next、proc_time等函数时等待队列(run_queue)或斜堆(skew_heap)的信息
 */

static int queue_printf_time = 0;
static int queue_printf_time_limit = 1000;

static void skew_heap_DFS(skew_heap_entry_t *a)
{
    if (a == NULL)
    {
        cprintf(" NULL\n");
        return;
    }

    struct proc_struct *proc = le2proc(a, lab6_run_pool);

    cprintf("                             [ 0x%08lx, pid = %d, stride = 0x%08lx, priority = %u ]\n", proc, proc->pid, proc->lab6_stride, proc->lab6_priority);
    if (a->left)
        skew_heap_DFS(a->left);
    if (a->right)
        skew_heap_DFS(a->right);
}

static void Print_enqueue(struct run_queue *rq, struct proc_struct *proc, char *const str)
{
    if ((++queue_printf_time) >= queue_printf_time_limit)
        return;
    cprintf("      %s successfully\n", sched_fun[ENQUEUE]);
    cprintf("         %s call\n", str);
    cprintf("         enqueue proc: ( 0x%08lx ), pid = %d\n", proc, proc->pid);
    // if (proc->time_slice == rq->max_time_slice)
    //  cprintf("         reset this proc time_slice: %u\n", proc->time_slice);
    /****************************************************/

    cprintf("         skew_heap[%2d proc]: [rq->lab6_run_pool] -> \n", rq->proc_num);
    skew_heap_DFS(rq->lab6_run_pool);
}

static void Print_dequeue(struct proc_struct *proc)
{
    if ((queue_printf_time) >= queue_printf_time_limit)
        return;
    cprintf("      %s\n", sched_fun[DEQUEUE]);
    /****************************************************/

    cprintf("         dequeue proc: [ 0x%08lx, pid = %d, stride = 0x%08lx, priority = %u, ts = %d]\n", proc, proc->pid, proc->lab6_stride, proc->lab6_priority, proc->time_slice);
}

static void Print_pick_next(struct proc_struct *proc)
{
    if ((queue_printf_time) >= queue_printf_time_limit)
        return;
    cprintf("      %s\n", sched_fun[PICK_NEXT]);
    /****************************************************/

    cprintf("         pick    proc: [ 0x%08lx, pid = %d, stride = 0x%08lx, priority = %u, ts = %d]\n", proc, proc->pid, proc->lab6_stride, proc->lab6_priority, proc->time_slice);
}

#endif
