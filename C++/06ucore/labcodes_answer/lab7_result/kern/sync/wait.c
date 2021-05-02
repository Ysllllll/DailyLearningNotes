#include <defs.h>
#include <list.h>
#include <sync.h>
#include <wait.h>
#include <proc.h>

/*
 * wait结构和wait queue结构以及相关函数，是实现ucore中的信号量机制和条件变量机制的基础，
 * 进入wait queue的进程会被设为睡眠状态，直到它们被唤醒。
struct wait_queue_t
{
    list_entry_t wait_head; //wait queue的队头
};
struct wait_t
{
    struct proc_struct *proc; //等待进程的指针
    uint32_t wakeup_flags;    //进程被放入等待队列的原因标记
    wait_queue_t *wait_queue; //指向此wait结构所属于的wait_queue
    list_entry_t wait_link;   //用来组织wait_queue中wait节点的连接
} wait_t;
*/

/* wait_init()
 * 初始化wait结构
 */
void wait_init(wait_t *wait, struct proc_struct *proc)
{
    wait->proc = proc;
    wait->wakeup_flags = WT_INTERRUPTED;
    list_init(&(wait->wait_link));
}

/* wait_queue_init()
 * 初始化wait_queue结构
 */
void wait_queue_init(wait_queue_t *queue)
{
    list_init(&(queue->wait_head));
}

/* wait_queue_add()
 * 把wait前插到wait queue中
 */
void wait_queue_add(wait_queue_t *queue, wait_t *wait)
{
    assert(list_empty(&(wait->wait_link)) && wait->proc != NULL);
    wait->wait_queue = queue;
    list_add_before(&(queue->wait_head), &(wait->wait_link));
}

/* wait_queue_del()
 * 从wait queue中删除wait
 */
void wait_queue_del(wait_queue_t *queue, wait_t *wait)
{
    assert(!list_empty(&(wait->wait_link)) && wait->wait_queue == queue);
    list_del_init(&(wait->wait_link));
}

/* wait_queue_next()
 * 取得wait queue的后一个连接指针
 */
wait_t *wait_queue_next(wait_queue_t *queue, wait_t *wait)
{
    assert(!list_empty(&(wait->wait_link)) && wait->wait_queue == queue);
    list_entry_t *le = list_next(&(wait->wait_link));
    if (le != &(queue->wait_head))
    {
        return le2wait(le, wait_link);
    }
    return NULL;
}

/* wait_queue_prev()
 * 取得wait queue的前一个连接指针
 */
wait_t *wait_queue_prev(wait_queue_t *queue, wait_t *wait)
{
    assert(!list_empty(&(wait->wait_link)) && wait->wait_queue == queue);
    list_entry_t *le = list_prev(&(wait->wait_link));
    if (le != &(queue->wait_head))
    {
        return le2wait(le, wait_link);
    }
    return NULL;
}

/* wait_queue_first()
 * 取得wait queue的第一个wait
 */
wait_t *wait_queue_first(wait_queue_t *queue)
{
    list_entry_t *le = list_next(&(queue->wait_head));
    if (le != &(queue->wait_head))
    {
        return le2wait(le, wait_link);
    }
    return NULL;
}

/* wait_queue_last()
 * 取得wait queue的最后一个wait
 */
wait_t *wait_queue_last(wait_queue_t *queue)
{
    list_entry_t *le = list_prev(&(queue->wait_head));
    if (le != &(queue->wait_head))
    {
        return le2wait(le, wait_link);
    }
    return NULL;
}

/* wait_queue_empty()
 * wait queue是否为空
 */
bool wait_queue_empty(wait_queue_t *queue)
{
    return list_empty(&(queue->wait_head));
}

/* wait_in_queue()
 * wait是否在wait queue中
 */
bool wait_in_queue(wait_t *wait)
{
    return !list_empty(&(wait->wait_link));
}

/* wakeup_wait()
 * 唤醒与wait关联的进程
 */
void wakeup_wait(wait_queue_t *queue, wait_t *wait, uint32_t wakeup_flags, bool del)
{
    if (del)
    {
        wait_queue_del(queue, wait);
    }
    wait->wakeup_flags = wakeup_flags;
    wakeup_proc(wait->proc);
}

/* wakeup_first()
 * 唤醒等待队列上挂着的第一个wait所关联的进程
 */
void wakeup_first(wait_queue_t *queue, uint32_t wakeup_flags, bool del)
{
    wait_t *wait;
    if ((wait = wait_queue_first(queue)) != NULL)
    {
        wakeup_wait(queue, wait, wakeup_flags, del);
    }
}

/* wakeup_queue()
 * 唤醒等待队列上所有的等待的进程
 */
void wakeup_queue(wait_queue_t *queue, uint32_t wakeup_flags, bool del)
{
    wait_t *wait;
    if ((wait = wait_queue_first(queue)) != NULL)
    {
        if (del)
        {
            do
            {
                wakeup_wait(queue, wait, wakeup_flags, 1);
            } while ((wait = wait_queue_first(queue)) != NULL);
        }
        else
        {
            do
            {
                wakeup_wait(queue, wait, wakeup_flags, 0);
            } while ((wait = wait_queue_next(queue, wait)) != NULL);
        }
    }
}

/* wait_current_set()
 * 让wait与进程关联，且让当前进程关联的wait进入等待队列wait queue，当前进程睡眠
 */
void wait_current_set(wait_queue_t *queue, wait_t *wait, uint32_t wait_state)
{
    assert(current != NULL);
    wait_init(wait, current);
    current->state = PROC_SLEEPING;
    current->wait_state = wait_state;
    wait_queue_add(queue, wait);
}
