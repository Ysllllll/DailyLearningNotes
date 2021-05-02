#ifndef __KERN_SYNC_MONITOR_CONDVAR_H__
#define __KERN_SYNC_MOINTOR_CONDVAR_H__

#include <sem.h>
/* In [OS CONCEPT] 7.7 section, the accurate define and approximate implementation of MONITOR was introduced.
 * INTRODUCTION:
 *  Monitors were invented by C. A. R. Hoare and Per Brinch Hansen, and were first implemented in Brinch Hansen's
 *  Concurrent Pascal language. Generally, a monitor is a language construct and the compiler usually enforces mutual exclusion. Compare this with semaphores, which are usually an OS construct.
 * DEFNIE & CHARACTERISTIC:
 *  A monitor is a collection of procedures, variables, and data structures grouped together.
 *  Processes can call the monitor procedures but cannot access the internal data structures.
 *  Only one process at a time may be be active in a monitor.
 *  Condition variables allow for blocking and unblocking.
 *     cv.wait() blocks a process.
 *        The process is said to be waiting for (or waiting on) the condition variable cv.
 *     cv.signal() (also called cv.notify) unblocks a process waiting for the condition variable cv.
 *        When this occurs, we need to still require that only one process is active in the monitor. This can be done in several ways:
 *            on some systems the old process (the one executing the signal) leaves the monitor and the new one enters
 *            on some systems the signal must be the last statement executed inside the monitor.
 *            on some systems the old process will block until the monitor is available again.
 *            on some systems the new process (the one unblocked by the signal) will remain blocked until the monitor is available again.
 *   If a condition variable is signaled with nobody waiting, the signal is lost. Compare this with semaphores, in which a signal will allow a process that executes a wait in the future to no block.
 *   You should not think of a condition variable as a variable in the traditional sense.
 *     It does not have a value.
 *     Think of it as an object in the OOP sense.
 *     It has two methods, wait and signal that manipulate the calling process.
 * IMPLEMENTATION:
 *   monitor mt {
 *     ----------------variable------------------
 *     semaphore mutex;
 *     semaphore next;
 *     int next_count;
 *     condvar {int count, sempahore sem}  cv[N];
 *     other variables in mt;
 *     --------condvar wait/signal---------------
 *     cond_wait (cv) {
 *         cv.count ++;
 *         if(mt.next_count>0)
 *            signal(mt.next)
 *         else
 *            signal(mt.mutex);
 *         wait(cv.sem);
 *         cv.count --;
 *      }
 *
 *      cond_signal(cv) {
 *          if(cv.count>0) {
 *             mt.next_count ++;
 *             signal(cv.sem);
 *             wait(mt.next);
 *             mt.next_count--;
 *          }
 *       }
 *     --------routines in monitor---------------
 *     routineA_in_mt () {
 *        wait(mt.mutex);
 *        ...
 *        real body of routineA
 *        ...
 *        if(next_count>0)
 *            signal(mt.next);
 *        else
 *            signal(mt.mutex);
 *     }
 */

typedef struct monitor monitor_t;

//一个条件变量可以理解为一个进程的等待队列
typedef struct condvar
{
    semaphore_t sem;  // 用于让发出wait_cv操作的等待某个条件C为真的进程睡眠，而让发出signal_cv操作的进程通过这个sem来唤醒睡眠的进程
    int count;        // 表示等在这个条件变量上的睡眠进程的个数
    monitor_t *owner; // 表示此条件变量的宿主是哪个管程
} condvar_t;

typedef struct monitor
{
    semaphore_t mutex; // 是一个二值信号量，是实现每次只允许一个进程进入管程的关键元素，确保了互斥访问性质
    semaphore_t next;  // 配合进程对条件变量cv的操作而设置的，这是由于发出signal_cv的进程A会唤醒睡眠进程B，进程B执行会导致进程A睡眠，
                       // 直到进程B离开管程，进程A才能继续执行，这个同步过程是通过信号量next完成的。
    int next_count;    // 配合进程对条件变量cv的操作而设置的，表示了由于发出signal_cv而睡眠的进程个数。
    condvar_t *cv;     // cv通过执行wait_cv，会使等待某个条件C为真的进程能够离开管程并睡眠，且让其它进程进入管程继续执行；
                       // 而进入管程的某进程设置条件C为真并执行signal_cv时，能够让等待某个条件C为真的睡眠进程被唤醒，从而继续进入管程中执行
} monitor_t;

// Initialize variables in monitor.
void monitor_init(monitor_t *cvp, size_t num_cv);
// Unlock one of threads waiting on the condition variable.
void cond_signal(condvar_t *cvp);
// Suspend calling thread on a condition variable waiting for condition atomically unlock mutex in monitor,
// and suspends calling thread on conditional variable after waking up locks mutex.
void cond_wait(condvar_t *cvp);

static char *const cond_indent[] = {"", "\t\t", "\t\t\t\t", "\t\t\t\t\t\t", "\t\t\t\t\t\t\t\t"};

#endif /* !__KERN_SYNC_MONITOR_CONDVAR_H__ */
