#include <stdio.h>
#include <proc.h>
#include <sem.h>
#include <monitor.h>
#include <assert.h>

#define N 5                  /* 哲学家数目 */
#define LEFT (i - 1 + N) % N /* i的左邻号码 */
#define RIGHT (i + 1) % N    /* i的右邻号码 */
#define THINKING 0           /* 哲学家正在思考 */
#define HUNGRY 1             /* 哲学家想取得叉子 */
#define EATING 2             /* 哲学家正在吃面 */
#define TIMES 4              /* 吃4次饭 */
#define SLEEP_TIME 10

//-----------------philosopher problem using monitor ------------
/*PSEUDO CODE :philosopher problem using semaphore
system DINING_PHILOSOPHERS

VAR
me:    semaphore, initially 1;                    # for mutual exclusion 
s[5]:  semaphore s[5], initially 0;               # for synchronization 
pflag[5]: {THINK, HUNGRY, EAT}, initially THINK;  # philosopher flag 

# As before, each philosopher is an endless cycle of thinking and eating.

procedure philosopher(i)
  {
    while TRUE do
     {
       THINKING;
       take_chopsticks(i);
       EATING;
       drop_chopsticks(i);
     }
  }

# The take_chopsticks procedure involves checking the status of neighboring 
# philosophers and then declaring one's own intention to eat. This is a two-phase 
# protocol; first declaring the status HUNGRY, then going on to EAT.

procedure take_chopsticks(i)
  {
    DOWN(me);               # critical section 
    pflag[i] := HUNGRY;
    test[i];
    UP(me);                 # end critical section 
    DOWN(s[i])              # Eat if enabled 
   }

void test(i)                # Let phil[i] eat, if waiting 
  {
    if ( pflag[i] == HUNGRY
      && pflag[i-1] != EAT
      && pflag[i+1] != EAT)
       then
        {
          pflag[i] := EAT;
          UP(s[i])
         }
    }


# Once a philosopher finishes eating, all that remains is to relinquish the 
# resources---its two chopsticks---and thereby release waiting neighbors.

void drop_chopsticks(int i)
  {
    DOWN(me);                # critical section 
    test(i-1);               # Let phil. on left eat if possible 
    test(i+1);               # Let phil. on rght eat if possible 
    UP(me);                  # up critical section 
   }

*/

//---------- philosophers problem using semaphore ----------------------
int state_sema[N]; /* 记录每个人状态的数组 */
/* 信号量是一个特殊的整型变量 */
semaphore_t mutex; /* 临界区互斥 */
semaphore_t s[N];  /* 每个哲学家一个信号量 */

struct proc_struct *philosopher_proc_sema[N];

void phi_test_sema(i) /* i：哲学家号码从0到N-1 */
{
    //此时，哲学家自己是HUNGRY，而且试图检查旁边两位是否都在吃，如果都不在吃，那么可以获得EATING的状态
    if (state_sema[i] == HUNGRY && state_sema[LEFT] != EATING && state_sema[RIGHT] != EATING)
    {
        state_sema[i] = EATING;
        up(&s[i]);
    }
}

void phi_take_forks_sema(int i) /* i：哲学家号码从0到N-1 */
{
    down(&mutex);           /* 进入临界区 */
    state_sema[i] = HUNGRY; /* 记录下哲学家i饥饿的事实 */
    phi_test_sema(i);       /* 试图得到两只叉子 */
    up(&mutex);             /* 离开临界区 */
    down(&s[i]);            /* 如果得不到叉子就阻塞 */
}

void phi_put_forks_sema(int i) /* i：哲学家号码从0到N-1 */
{
    down(&mutex);             /* 进入临界区 */
    state_sema[i] = THINKING; /* 哲学家进餐结束 */
    phi_test_sema(LEFT);      /* 看一下左邻居现在是否能进餐 */
    phi_test_sema(RIGHT);     /* 看一下右邻居现在是否能进餐 */
    up(&mutex);               /* 离开临界区 */
}

/* 
 * 要理解这个程序，可以设定一种非常简单的运行方式来模拟运行一次
 * 1. 理解down(&mutex)和up(&mutex)
 *    我们来看看phi_take_forks_sema这里的运行情况。
 *    在运行到phi_take_forks_sema之前我们假定程序的运行状况如下：
 *                           NO.1          NO.2          NO.3          NO.4          NO.5。
 *                         do_sleep()                                                            timer_list->NO.1(expires=9) 这里run_list队列里除了NO.1外，其它的均在里面，因为加入了timer_list，进程处于睡眠态，而睡眠态是不enqueue到run_list中的，可以查看下enqueue函数的实现
 *                                       do_sleep()                                              timer_list->NO.1(expires=8)->NO.2(expires=2)
 *                                                     do_sleep()                                timer_list->NO.1(expires=7)->NO.2(expires=2)->NO.3(expires=1)   
 *                                                                   do_sleep()                  timer_list->NO.1(expires=6)->NO.2(expires=2)->NO.3(expires=1)->NO.4(expires=1)
 *                                                                                  do_sleep()   timer_list->NO.1(expires=5)->NO.2(expires=2)->NO.3(expires=1)->NO.4(expires=1)->NO.5(expires=1) 这里run_list队列为空。
 * 
 * 我们假定执行到了NO.5的do_sleep()中schedule()，然后到idleproc进程去了，
 * 期间，我们假定在idleproc中的if (current->need_resched)语句处一个密集
 * 的时钟中断到来，执行了run_timer_list>10次，这样全部进程的expires到期，
 * 全部加入到run_list队列中去。                                           run_list->NO.1->NO.2->NO.3->NO.4->NO.5 注意任意一个进程执行过程中均能产生中断
 * 然后继续执行idleproc中的if (current->need_resched)，满足条件，进行调
 * 度，假定调度了NO.1，执行phi_take_forks_sema(1)->down(&mutex)，此
 * 时获取了这个信号量，再假定在执行的过程中，NO.2哲学家进程进行调度，也进入
 * phi_take_forks_sema(2)->down(&mutex)时，由于mutex->value = 0，NO.2
 * 进程将加入到mutex的wait_queue中。再进行调度，假定调度的是NO.1，那么在结
 * 束phi_take_forks_sema(1)之前执行up(&mutex)时，由于mutex中的队列不为空，
 * 即将NO.2唤醒，NO.1离开临界区，do_sleep()进入睡眠，调度到NO.2，NO.2
 * 继续从挂起的地方开始执行，即down(&mutex)中，将自己从等待队列中删除，离开
 * down(&mutex)，进入到临界区执行。注意此时的mutex->value = 0，当执行完临
 * 界区的代码后，离开临界区执行up(&mutex)，此时会将mutex->value++ = 1。不
 * 影响其它进程的执行。
 *                                                                                                
 * 再考虑刀叉的使用。同样可以用NO.1和NO.2来进行简单举例。NO.1在
 * phi_take_forks_sema中获取了刀叉，然后执行do_sleep进入睡眠状态，这是刀叉
 * 是被NO.1持续占有的。NO.2进入临界区请求刀叉，执行phi_test_sema时，if为假
 * 不执行up(&s[2])，此时s[2]->value = 0，在离开邻接区执行down(&s[2])时，
 * 进程将被挂起到s[2]的等待队列，然后进行调度。假定NO.1已经睡眠完成，并准备
 * 放下刀叉，执行phi_put_forks_sema(1)，在里面他会检查phi_test_sema(2)和
 * phi_test_sema(0)，我们假定只有2满足if条件，那么将刀叉分给他，执行
 * up(&s[2])，将NO.2从s[2]的等待队列中唤醒。NO.1继续执行，然后睡眠，进行
 * 调度，假定调度到NO.2执行，NO.2将从down(&s[2])中的schedule处开始执行，
 * 将自己从等待队列中删除，然后从down(&s[2])处离开临界区时，他已经获得了
 * 刀叉。可以发现如果某个哲学家进程由于没有刀叉进入了阻塞状态，那么他们刀叉
 * 的获取将有其它进程来完成。也就是phi_test_sema(LEFT)、phi_test_sema(RIGHT)
 */
int philosopher_using_semaphore(void *arg) /* i：哲学家号码，从0到N-1 */
{
    int i, iter = 0;
    i = (int)arg; //传入的参数为int型，代表哲学家的编号
    cprintf("I am No.%d philosopher_sema, pid = %d\n", i, current->pid);
    while (iter++ < TIMES) //无限循环，这里取TIMES = 4
    {
        cprintf("Iter %d, No.%d philosopher_sema is thinking, pid = %d\n", iter, i, current->pid); /* 哲学家正在思考 */
        do_sleep(SLEEP_TIME);

        phi_take_forks_sema(i); /* 需要两只叉子，或者阻塞 */

        cprintf("Iter %d, No.%d philosopher_sema is eating, pid = %d\n", iter, i, current->pid); /* 进餐 */
        do_sleep(SLEEP_TIME);

        phi_put_forks_sema(i); /* 把两把叉子同时放回桌子 */
    }
    cprintf("No.%d philosopher_sema quit, pid = %d\n", i, current->pid);
    return 0;
}

//-----------------philosopher problem using monitor ------------
/*PSEUDO CODE :philosopher problem using monitor
 * monitor dp
 * {
 *  enum {thinking, hungry, eating} state[5];
 *  condition self[5];
 *
 *  void pickup(int i) {
 *      state[i] = hungry;
 *      if ((state[(i+4)%5] != eating) && (state[(i+1)%5] != eating)) {
 *        state[i] = eating;
 *      else
 *         self[i].wait();
 *   }
 *
 *   void putdown(int i) {
 *      state[i] = thinking;
 *      if ((state[(i+4)%5] == hungry) && (state[(i+3)%5] != eating)) {
 *          state[(i+4)%5] = eating;
 *          self[(i+4)%5].signal();
 *      }
 *      if ((state[(i+1)%5] == hungry) && (state[(i+2)%5] != eating)) {
 *          state[(i+1)%5] = eating;
 *          self[(i+1)%5].signal();
 *      }
 *   }
 *
 *   void init() {
 *      for (int i = 0; i < 5; i++)
 *         state[i] = thinking;
 *   }
 * }
 */

struct proc_struct *philosopher_proc_condvar[N]; // N philosopher
int state_condvar[N];                            // the philosopher's state: EATING, HUNGARY, THINKING
monitor_t mt, *mtp = &mt;                        // monitor

void phi_test_condvar(i)
{
    if (state_condvar[i] == HUNGRY && state_condvar[LEFT] != EATING && state_condvar[RIGHT] != EATING)
    {
        cprintf("phi_test_condvar: state_condvar[%d] will eating\n", i);
        state_condvar[i] = EATING;
        cprintf("phi_test_condvar: signal self_cv[%d] \n", i);
        cond_signal(&mtp->cv[i]);
    }
}

void phi_take_forks_condvar(int i)
{
    down(&(mtp->mutex));
    //--------into routine in monitor--------------
    // LAB7 EXERCISE1: YOUR CODE
    // I am hungry
    // try to get fork
    // I am hungry
    state_condvar[i] = HUNGRY;
    // try to get fork
    phi_test_condvar(i);
    if (state_condvar[i] != EATING)
    {
        cprintf("phi_take_forks_condvar: %d didn't get fork and will wait\n", i);
        cond_wait(&mtp->cv[i]);
    }
    //--------leave routine in monitor--------------
    if (mtp->next_count > 0)
        up(&(mtp->next));
    else
        up(&(mtp->mutex));
}

void phi_put_forks_condvar(int i)
{
    down(&(mtp->mutex));

    //--------into routine in monitor--------------
    // LAB7 EXERCISE1: YOUR CODE
    // I ate over
    // test left and right neighbors
    // I ate over
    state_condvar[i] = THINKING;
    // test left and right neighbors
    phi_test_condvar(LEFT);
    phi_test_condvar(RIGHT);
    //--------leave routine in monitor--------------
    if (mtp->next_count > 0)
        up(&(mtp->next));
    else
        up(&(mtp->mutex));
}

//---------- philosophers using monitor (condition variable) ----------------------
int philosopher_using_condvar(void *arg)
{ /* arg is the No. of philosopher 0~N-1*/

    int i, iter = 0;
    i = (int)arg;
    cprintf("I am No.%d philosopher_condvar\n", i);
    while (iter++ < TIMES)
    {                                                                         /* iterate*/
        cprintf("Iter %d, No.%d philosopher_condvar is thinking\n", iter, i); /* thinking*/
        do_sleep(SLEEP_TIME);
        phi_take_forks_condvar(i);
        /* need two forks, maybe blocked */
        cprintf("Iter %d, No.%d philosopher_condvar is eating\n", iter, i); /* eating*/
        do_sleep(SLEEP_TIME);
        phi_put_forks_condvar(i);
        /* return two forks back*/
    }
    cprintf("No.%d philosopher_condvar quit\n", i);
    return 0;
}

void check_sync(void)
{

    int i;

    cprintf("   check_sync() run: \n");
    cprintf("      /*Initialize semaphores and create 5 processes representing 5 philosophers*/\n");
    //check semaphore 第一部分：基于信号量的哲学家问题
    sem_init(&mutex, 1);
    cprintf("      init mutex semaphore: value = 1, wait_queue = 0x%08lx\n", &(mutex.wait_queue));

    for (i = 0; i < N; i++) //N是哲学家的数量
    {
        sem_init(&s[i], 0); //初始化信号量
        cprintf("      init s[%d] semaphore: value = 0, wait_queue = 0x%08lx\n", i, &(s[i].wait_queue));
        int pid = kernel_thread(philosopher_using_semaphore, (void *)i, 0); //线程需要执行的函数名、哲学家编号、0表示共享内存，创建哲学家就餐问题的内核线程
        if (pid <= 0)
        {
            panic("create No.%d philosopher_using_semaphore failed.\n");
        }
        philosopher_proc_sema[i] = find_proc(pid);
        set_proc_name(philosopher_proc_sema[i], "philosopher_sema_proc");
    }

    cprintf("\n\n***********************************************All 5 philosopher processes have been created and have been added to the runnable queue***********************************************\n\n");

    //check condition variable 第二部分基于管程的哲学家问题
    // monitor_init(&mt, N);
    // for (i = 0; i < N; i++)
    // {
    //     state_condvar[i] = THINKING;
    //     int pid = kernel_thread(philosopher_using_condvar, (void *)i, 0);
    //     if (pid <= 0)
    //     {
    //         panic("create No.%d philosopher_using_condvar failed.\n");
    //     }
    //     philosopher_proc_condvar[i] = find_proc(pid);
    //     set_proc_name(philosopher_proc_condvar[i], "philosopher_condvar_proc");
    // }
}
