#ifndef __KERN_SYNC_SEM_H__
#define __KERN_SYNC_SEM_H__

#include <defs.h>
#include <atomic.h>
#include <wait.h>

/* semaphore_t是最基本的记录型信号量结构，包含了用于计数的整数值value，
 * 和一个进程等待队列wait_queue，一个等待的进程会挂在此等待队列上。
 */
typedef struct
{
    int value;               //信号量的当前值
    wait_queue_t wait_queue; //信号量对应的等待队列
} semaphore_t;

void sem_init(semaphore_t *sem, int value);
void up(semaphore_t *sem);
void down(semaphore_t *sem);
bool try_down(semaphore_t *sem);

#endif /* !__KERN_SYNC_SEM_H__ */

/* PV操作概念：操作系统中的一种同步机制，实现对于 并发进程 中的 临界区 的管理
 * 并发进程分为两种：
 *      1.无交互的并发进程：每个进程相互独立，谁也不影响谁，基本不会用到PV操作。
 *      2.有交互的并发进程：多个进程共享资源，一个进程的执行，有可能会被外界的原因中断，且断点不固定。
 *                        进程执行的相对速度不能由进程自己控制，于是就会导致并发进程在共享资源的时候出现与时间相关的错误。
 * 临界区：并发进程中与共享变量有关的 程序段 都称为 临界区
 * P操作：申请资源操作
 * V操作：释放资源操作
 * 信号量S：用来记录资源数量，看是否能满足申请资源的操作。是一个全局变量
 *         例如：S=3 表示三个可用空闲资源，S<0 表示无可用空闲资源，进程申请要进入等待队列中
 * 
 * P(S)：S <- S-1
 *      如果S>=0，进程继续执行
 *      如果S<0，进程停止执行，放入信号量等待队列中
 * V(S)：S <- S+1
 *      如果S>0，进程继续执行
 *      如果S<=0，唤醒等待队列中的一个进程
 * 
 */