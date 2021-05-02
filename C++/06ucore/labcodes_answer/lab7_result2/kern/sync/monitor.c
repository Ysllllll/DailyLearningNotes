#include <stdio.h>
#include <monitor.h>
#include <kmalloc.h>
#include <assert.h>

// Initialize monitor.
void monitor_init(monitor_t *mtp, size_t num_cv)
{
   /*
 *      |—— next | mtp->next_count = 0  
 *      |
 *      |        | sem_init(&(mtp->mutex), 1)
 *      |—— init |
 *      |        | sem_init(&(mtp->next), 0)
 * mtp->|
 *      |
 *      |——  cv  |     ——count | mtp->cv[0].count = 0
 *               |0 —— ——sem   | sem_init(&(mtp->cv[0].sem), 0);
 *               |     ——owner | mtp->cv[0].owner = mtp;
 *               |
 *               |     ——count | mtp->cv[1].count = 0
 *               |1 —— ——sem   | sem_init(&(mtp->cv[1].sem), 0);
 *               |     ——owner | mtp->cv[1].owner = mtp;
 *               |
 *               |     ——count | mtp->cv[2].count = 0
 *               |2 —— ——sem   | sem_init(&(mtp->cv[2].sem), 0);
 *               |     ——owner | mtp->cv[2].owner = mtp;
 *               |
 *               |     ——count | mtp->cv[3].count = 0
 *               |3 —— ——sem   | sem_init(&(mtp->cv[3].sem), 0);
 *               |     ——owner | mtp->cv[3].owner = mtp;
 *               |
 *               |     ——count | mtp->cv[4].count = 0
 *               |4 —— ——sem   | sem_init(&(mtp->cv[4].sem), 0);
 *               |     ——owner | mtp->cv[4].owner = mtp;
*/
   int i;
   assert(num_cv > 0);
   mtp->next_count = 0;
   mtp->cv = NULL;
   sem_init(&(mtp->mutex), 1); //unlocked
   sem_init(&(mtp->next), 0);
   mtp->cv = (condvar_t *)kmalloc(sizeof(condvar_t) * num_cv);
   assert(mtp->cv != NULL);
   for (i = 0; i < num_cv; i++)
   {
      mtp->cv[i].count = 0;
      sem_init(&(mtp->cv[i].sem), 0);
      mtp->cv[i].owner = mtp;
   }
}

// Unlock one of threads waiting on the condition variable.
//将等待队列中的一个线程唤醒，如果等待队列为空，则等同空操作
/*
首先进程B判断cv.count，如果不大于0，则表示当前没有执行cond_wait而睡眠的进程，因此就没有被唤醒的对象了，直接函数返回即可；
如果大于0，则表示当前有执行cond_wait而睡眠的进程A，因此需要唤醒等待在cv.sem上睡眠的进程A。由于只允许一个进程在管程中执行，
所以一旦进程B唤醒了别人(进程A)，那么自己就需要睡眠。故让monitor.next_count加1，且让自己(进程B)睡在信号量monitor.next上，
如果睡醒了，则让monitor.next_count减1。
*/
void cond_signal(condvar_t *cvp)
{
   cprintf("cond_signal begin: cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);

   if (cvp->count > 0)
   {
      cvp->owner->next_count++;
      up(&(cvp->sem)); //up会wakeup进程，而不会进行调度。只有down才会进行调度
      down(&(cvp->owner->next));
      cvp->owner->next_count--;
   }
   cprintf("cond_signal end: cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);
}

// Suspend calling thread on a condition variable waiting for condition Atomically unlocks
// mutex and suspends calling thread on conditional variable after waking up locks mutex. Notice: mp is mutex semaphore for monitor's procedures
// 将自己阻塞在等待队列中，唤醒一个等待者或释放管程的互斥访问
/*
进程A执行了cond_wait函数，表示此进程等待的某个条件C不为真，需要睡眠。因此表示等待此条件的睡眠进程个数cv.count要加1，加下来会有如下两种情况：
1.如果monitor.next_count如果大于0，表示有大于等于1个进程执行cond_signal函数且睡着了，就睡在了monitor.next信号量上。假定这些进程形成S进程链表，
因此需要唤醒S进程链表中的一个进行B。然后进程A睡在cv.sem上，如果睡醒了，则让cv.count减1，表示等待此条件的睡眠进程个数少了一个，可继续执行了，
这里隐含了这一个现象：即某进程A在时间顺序上先执行了signal_cv，而另一个进程B后执行了wait_cv，这会导致进程A没有起到唤醒进程B的作用。这里还隐藏着
一个问题，在cond_wait有sem_signal(mutex)，但是没有看到哪里有sem_wait(mutex)，这好像没有成对出现，是否错误呢？其实在管程中的每一个函数的入口处
会有wait(mutex)，这样二者就配对好了。
2.如果monitor.next_count小于等于0，表示目前没有进程执行cond_signal函数且睡着了，那需要唤醒的是由于互斥条件限制而无法进入管程的进程，所以要唤醒
睡在monitor.mutex上的进程，然后进程A睡在cv.sem上，如果睡醒了，则让cv.count减1，表示等待此条件的睡眠进程个数少了一个，可继续执行了。
*/

void cond_wait(condvar_t *cvp)
{
   cprintf("cond_wait begin:  cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);

   cvp->count++;
   if (cvp->owner->next_count > 0)
      up(&(cvp->owner->next));
   else
      up(&(cvp->owner->mutex));
   down(&(cvp->sem));
   cvp->count--;
   cprintf("cond_wait end:  cvp %x, cvp->count %d, cvp->owner->next_count %d\n", cvp, cvp->count, cvp->owner->next_count);
}
