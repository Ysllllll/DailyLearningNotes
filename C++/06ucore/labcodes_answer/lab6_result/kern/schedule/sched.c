#include <list.h>
#include <sync.h>
#include <proc.h>
#include <sched.h>
#include <stdio.h>
#include <assert.h>
#include <RR_sched.h>
#include <default_sched.h>

char *sched_fun[4];

// the list of timer
static list_entry_t timer_list;

static struct sched_class *sched_class;

static struct run_queue *rq;

static inline void
sched_class_enqueue(struct proc_struct *proc, char *const str)
{
    if (proc != idleproc)
    {
        sched_class->enqueue(rq, proc, str);
    }
    else
    {
        cprintf("      %s failed\n", sched_fun[ENQUEUE]);
        cprintf("         proc = idleproc\n");
    }
}

static inline void
sched_class_dequeue(struct proc_struct *proc)
{
    sched_class->dequeue(rq, proc);
}

static inline struct proc_struct *
sched_class_pick_next(void)
{
    return sched_class->pick_next(rq);
}

void sched_class_proc_tick(struct proc_struct *proc)
{
    if (proc != idleproc)
    {
        sched_class->proc_tick(rq, proc);
    }
    else
    {
        proc->need_resched = 1;
    }
}

static struct run_queue __rq;

void sched_init(void)
{
    list_init(&timer_list);

    sched_class = &default_sched_class;

    rq = &__rq;
    rq->max_time_slice = MAX_TIME_SLICE;
    sched_class->init(rq);

    cprintf("sched class: %s\n", sched_class->name);

    // if (!strcmp(sched_class->name, "RR_scheduler"))
    //     memcpy(sched_fun, RR_sched_fun, sizeof(RR_sched_fun));
    // else if (!strcmp(sched_class->name, "stride_scheduler"))
    //     memcpy(sched_fun, stride_sched_fun, sizeof(stride_sched_fun));
}

void wakeup_proc(struct proc_struct *proc)
{
    assert(proc->state != PROC_ZOMBIE);
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (proc->state != PROC_RUNNABLE)
        {
            proc->state = PROC_RUNNABLE;
            proc->wait_state = 0;
            if (proc != current)
            {
                sched_class_enqueue(proc, call_enqueue_fun[WAKEUP_PROC]);
            }
        }
        else
        {
            warn("wakeup runnable process.\n");
        }
    }
    local_intr_restore(intr_flag);
}

void schedule(char *const str)
{
    bool intr_flag;
    struct proc_struct *next;
    local_intr_save(intr_flag);
    {
        call_schedule_times++;
        cprintf("**************************%02d schedule start**************************\n", call_schedule_times);
        current->need_resched = 0;
        if (current->state == PROC_RUNNABLE)
        {
            sched_class_enqueue(current, call_enqueue_fun[SCHEDULE]);
        }
        else
        {
            cprintf("      %s skip\n", sched_fun[ENQUEUE]);
            cprintf("         current->state = %s\n", proc_state_index[current->state]);
            cprintf("         current = 0x%08lx, parent = 0x%08lx, parent->state = %s\n", current, current->parent, proc_state_index[current->parent->state]);
        }

        if ((next = sched_class_pick_next()) != NULL)
        {
            sched_class_dequeue(next);
        }
        if (next == NULL)
        {
            next = idleproc;
        }
        next->runs++;
        if (next != current)
        {
            if ((++call_schedule_times_successfully) <= schedule_print_limit)
            {
                cprintf("      schedule() successfully [No.%d]\n", call_schedule_times_successfully);
                cprintf("         %s call\n", str);
                cprintf("         schedule( 0x%08lx ) -> proc_run( 0x%08lx )\n", current, next);
            }
            cprintf("**************************%02d schedule success**************************\n\n", call_schedule_times);

            proc_run(next);
        }
        else
        {
            if (call_schedule_times_successfully <= schedule_print_limit)
            {
                cprintf("      schedule() failed\n");
                cprintf("         %s call\n", str);
                cprintf("         current.[ 0x%08lx ] == next.[ 0x%08lx ], proc_run() not occur\n", current, next);
            }
            cprintf("**************************%02d schedule failed**************************\n\n", call_schedule_times);
        }
    }
    local_intr_restore(intr_flag);
}
