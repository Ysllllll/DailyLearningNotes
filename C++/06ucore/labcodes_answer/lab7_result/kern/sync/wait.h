#ifndef __KERN_SYNC_WAIT_H__
#define __KERN_SYNC_WAIT_H__

#include <list.h>

/* 
 * wait结构和wait queue结构以及相关函数，是实现ucore中的信号量机制和条件变量机制的基础，
 * 进入wait queue的进程会被设为睡眠状态，直到它们被唤醒。
 * 
 */
typedef struct
{
    list_entry_t wait_head; //wait queue的队头
} wait_queue_t;

struct proc_struct;

typedef struct
{
    struct proc_struct *proc; //等待进程的指针
    uint32_t wakeup_flags;    //进程被放入等待队列的原因标记
    wait_queue_t *wait_queue; //指向此wait结构所属于的wait_queue
    list_entry_t wait_link;   //用来组织wait_queue中wait节点的连接
} wait_t;

#define le2wait(le, member) \
    to_struct((le), wait_t, member) //实现wait_t中的成员指针向wait_t指针的转化

void wait_init(wait_t *wait, struct proc_struct *proc); //初始化wait结构
void wait_queue_init(wait_queue_t *queue);              //初始化wait_queue结构
void wait_queue_add(wait_queue_t *queue, wait_t *wait); //把wait前插到wait queue中
void wait_queue_del(wait_queue_t *queue, wait_t *wait); //从wait queue中删除wait

wait_t *wait_queue_next(wait_queue_t *queue, wait_t *wait); //取得wait queue的后一个连接指针
wait_t *wait_queue_prev(wait_queue_t *queue, wait_t *wait); //取得wait queue的前一个连接指针
wait_t *wait_queue_first(wait_queue_t *queue);              //取得wait queue的第一个wait
wait_t *wait_queue_last(wait_queue_t *queue);               //取得wait queue的最后一个wait

bool wait_queue_empty(wait_queue_t *queue); //wait queue是否为空
bool wait_in_queue(wait_t *wait);           //wait是否在wait queue中

//唤醒与wait关联的进程
void wakeup_wait(wait_queue_t *queue, wait_t *wait, uint32_t wakeup_flags, bool del);
//唤醒等待队列上挂着的第一个wait所关联的进程
void wakeup_first(wait_queue_t *queue, uint32_t wakeup_flags, bool del);
//唤醒等待队列上所有的等待的进程
void wakeup_queue(wait_queue_t *queue, uint32_t wakeup_flags, bool del);
//让wait与进程关联，且让当前进程关联的wait进入等待队列wait queue，当前进程睡眠
void wait_current_set(wait_queue_t *queue, wait_t *wait, uint32_t wait_state);
//把与当前进程关联的wait从等待队列wait queue中删除
#define wait_current_del(queue, wait)    \
    do                                   \
    {                                    \
        if (wait_in_queue(wait))         \
        {                                \
            wait_queue_del(queue, wait); \
        }                                \
    } while (0)

#endif /* !__KERN_SYNC_WAIT_H__ */
