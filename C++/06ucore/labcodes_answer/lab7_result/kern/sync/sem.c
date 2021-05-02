#include <defs.h>
#include <wait.h>
#include <atomic.h>
#include <kmalloc.h>
#include <sem.h>
#include <proc.h>
#include <sync.h>
#include <assert.h>

void sem_init(semaphore_t *sem, int value)
{
    sem->value = value;
    wait_queue_init(&(sem->wait_queue));
}

/* __up()
 * 具体实现信号量的V操作，即释放资源。首先关掉中断。
 * 如果信号量对应的wait queue中没有进程在等待，直接把信号量的value++，然后开中断返回。
 * 如果有进程在等待且进程等待的原因是semophore设置的，则调用wakeup_wait函数将wait queue
 * 中等待的第一个wait删除，且把此wait关联的进程唤醒，然后开中断返回
 */
static __noinline void __up(semaphore_t *sem, uint32_t wait_state)
{
    bool intr_flag;
    local_intr_save(intr_flag); //关闭中断
    {
        wait_t *wait;
        if ((wait = wait_queue_first(&(sem->wait_queue))) == NULL) //信号量对应的wait queue中没有进程在等待，直接把信号量value++
        {
            sem->value++; //如果没有进程等待，信号量++
        }
        else // 如果有进程等待，且进程等待的原因是semophore设置的，
             //则调用wakeup_wait函数将wait queue中等待的第一个wait删除，且把此wait关联的进程唤醒
        {
            assert(wait->proc->wait_state == wait_state);
            wakeup_wait(&(sem->wait_queue), wait, wait_state, 1);
        }
    }
    local_intr_restore(intr_flag);
}

/* __down()
 * 具体实现信号量的P操作，即申请资源操作。首先关掉中断，然后判断当前信号量的value是否大于0。
 * 如果是>0，则表明可以获得信号量，故让value减1，并打开中断返回即可；
 * 如果不是>0，则表明无法获得信号量，故需要将当前的进程加入到等待队列中，并打开中断，然后运行调度器选择另外一个进程执行。
 * 如果被V操作唤醒，则把自身关联的wait从等待队列中删除（此过程需要先关中断，完成后打开中断）
 */
static __noinline uint32_t __down(semaphore_t *sem, uint32_t wait_state)
{
    bool intr_flag;
    local_intr_save(intr_flag); //关闭中断
    if (sem->value > 0)         //表明可以获得信号量
    {
        sem->value--;                  //获取一个信号量，value--
        local_intr_restore(intr_flag); //打开中断返回
        return 0;
    }

    // 接下来是无法获得信号量的处理过程
    //  1.将当前进程加入到等待队列中
    wait_t __wait, *wait = &__wait;
    wait_current_set(&(sem->wait_queue), wait, wait_state);
    local_intr_restore(intr_flag);

    // 2.打开中断，运行调度器选择另外一个进程执行
    schedule(call_schedule_fun[__DOWN]);

    // 被V操作唤醒，把自身关联的wait从等待队列中删除
    local_intr_save(intr_flag);
    wait_current_del(&(sem->wait_queue), wait);
    local_intr_restore(intr_flag);

    if (wait->wakeup_flags != wait_state)
    {
        return wait->wakeup_flags;
    }
    return 0;
}

void up(semaphore_t *sem)
{
    __up(sem, WT_KSEM);
}

void down(semaphore_t *sem)
{
    uint32_t flags = __down(sem, WT_KSEM);
    assert(flags == 0);
}

bool try_down(semaphore_t *sem)
{
    bool intr_flag, ret = 0;
    local_intr_save(intr_flag);
    if (sem->value > 0)
    {
        sem->value--, ret = 1;
    }
    local_intr_restore(intr_flag);
    return ret;
}
