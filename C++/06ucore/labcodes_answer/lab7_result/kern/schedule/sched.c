#include <list.h>
#include <sync.h>
#include <proc.h>
#include <sched.h>
#include <stdio.h>
#include <assert.h>
#include <default_sched.h>
#include <sched_fun_print.h>

char *sched_fun[4];

// the list of timer
list_entry_t timer_list;

static struct sched_class *sched_class;

static struct run_queue *rq;

static inline void sched_class_enqueue(struct proc_struct *proc, char *const str)
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

static inline void sched_class_dequeue(struct proc_struct *proc)
{
    sched_class->dequeue(rq, proc);
}

static inline struct proc_struct *sched_class_pick_next(void)
{
    return sched_class->pick_next(rq);
}

static void sched_class_proc_tick(struct proc_struct *proc)
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
        cprintf("\n*****************************%02d schedule start*****************************\n", call_schedule_times);
        current->need_resched = 0;
        if (current->state == PROC_RUNNABLE)
        {
            sched_class_enqueue(current, call_enqueue_fun[SCHEDULE]);
        }
        else
        {
            cprintf("      %s skip\n", sched_fun[ENQUEUE]);
            //cprintf("         current->state = %s\n", proc_state_index[current->state]);
            //cprintf("         current = 0x%08lx, parent = 0x%08lx, parent->state = %s\n", current, current->parent, proc_state_index[current->parent->state]);

            cprintf("         skew_heap[%2d proc]: [rq->lab6_run_pool] ->\n", rq->proc_num);
            skew_heap_DFS(rq->lab6_run_pool);
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
                cprintf("         %s call", str);
                cprintf("         schedule( pid = %d) -> proc_run( pid = %d)\n", current->pid, next->pid);
            }
            cprintf("*****************************%02d schedule success*****************************\n\n", call_schedule_times);

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
            cprintf("*****************************%02d schedule failed*****************************\n\n", call_schedule_times);
        }
    }
    local_intr_restore(intr_flag);
}

/* add_timer()
 * 向系统添加某个初始化过的timer_t，该计时器在指定时间后被激活，
 * 并将对应的进程唤醒至runnable（如果当前进程处于等待状态）
 */
void add_timer(timer_t *timer)
{
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        cprintf("         add_timer(): pid = %d, timer proc = 0x%08lx, set expires = %d\n", timer->proc->pid, timer->proc, timer->expires);

        assert(timer->expires > 0 && timer->proc != NULL);
        assert(list_empty(&(timer->timer_link)));
        list_entry_t *le = list_next(&timer_list);
        while (le != &timer_list)
        {
            timer_t *next = le2timer(le, timer_link);
            if (timer->expires < next->expires)
            {
                next->expires -= timer->expires;
                break;
            }
            timer->expires -= next->expires;
            le = list_next(le);
        }
        list_add_before(le, &(timer->timer_link));

        print_timer_list("      ");
    }
    local_intr_restore(intr_flag);
}

/* del_timer()
 * 向系统删除或者说取消某一个计时器，该计时器在取消后不会被系统激活并唤醒进程
 */
void del_timer(timer_t *timer, char *const str)
{
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (!list_empty(&(timer->timer_link)))
        {
            if (timer->expires != 0)
            {
                list_entry_t *le = list_next(&(timer->timer_link));
                if (le != &timer_list)
                {
                    timer_t *next = le2timer(le, timer_link);
                    next->expires += timer->expires;
                }
            }
            list_del_init(&(timer->timer_link));
            cprintf("      del_timer()\n");
            cprintf("         %s\n", str);
            cprintf("         pid = %d, timer proc = 0x%08lx, set expires = %d\n", timer->proc->pid, timer->proc, timer->expires);
        }
        else
        {
            cprintf("      del_timer(),  Timer does not exist\n");
        }
    }
    local_intr_restore(intr_flag);
}

/* run_timer_list()
 * 更新当前系统时间点，遍历当前所有处在系统管理内的计时器，找出所有应该激活的计时器，并激活他们，
 * 该过程在且只在每次计时器中断时被调用，在ucore中，其还会调用调度器事件处理程序。
 */
void run_timer_list(void)
{
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        list_entry_t *le = list_next(&timer_list);
        if (le != &timer_list)
        {
            timer_t *timer = le2timer(le, timer_link);
            assert(timer->expires != 0);
            timer->expires--;
            while (timer->expires == 0)
            {
                le = list_next(le);
                struct proc_struct *proc = timer->proc;
                if (proc->wait_state != 0)
                {
                    assert(proc->wait_state & WT_INTERRUPTED);
                }
                else
                {
                    warn("process %d's wait_state == 0.\n", proc->pid);
                }
                wakeup_proc(proc);
                del_timer(timer, call_del_timer_fun[del_timer_RUN_TIMER_LIST]);
                if (le == &timer_list)
                {
                    break;
                }
                timer = le2timer(le, timer_link);
            }
        }
        sched_class_proc_tick(current);

        if (!(current == idleproc || current == initproc))
            print_timer_list(int_L_indentation);
    }
    local_intr_restore(intr_flag);
}

void print_timer_list(const char *str)
{
    cprintf("%s   timer_list -> ", str);
    list_entry_t *le = &timer_list;
    int i = 1;
    if (list_next(le) == &timer_list)
    {
        cprintf("NULL\n");
        return;
    }
    else
        while ((le = list_next(le)) != &timer_list)
        {
            timer_t *timer = le2timer(le, timer_link);
            if (i == 1)
                cprintf("( proc = 0x%08lx, pid = %d, proc_expires = %d.[%d] )\n", timer->proc, timer->proc->pid, timer->expires, i++);
            else
                cprintf("%s                 ( proc = 0x%08lx, pid = %d, proc_expires = %d.[%d] )\n", str, timer->proc, timer->proc->pid, timer->expires, i++);
        }
}