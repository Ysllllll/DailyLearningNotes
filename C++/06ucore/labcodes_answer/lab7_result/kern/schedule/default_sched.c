#include <sched_fun_print.h>
#define BIG_STRIDE 0x7FFFFFFF /* ??? */

/*
 * struct run_queue                     //               init:sched_init()
 * {
 *     list_entry_t run_list;                            init:stride_init()
 *     unsigned int proc_num;           //stride_sched   init:stride_init()  modify:stride_enqueue() stride_dequeue()
 *     int max_time_slice;              //stride_sched   init:sched_init()
 *     skew_heap_entry_t *lab6_run_pool;//stride_sched   init:stride_init()  modify:stride_enqueue() stride_dequeue()
 * };
 * 
 * stride_sched调度策略
 * struct proc_struce
 * {
 *     struct run_queue *rq;            //stride_sched   init:alloc_proc()   modify:stride_enqueue() 
 *     list_entry_t run_link;                            init:alloc_proc()
 *     int time_slice;                  //stride_sched   init:alloc_proc()   modify:stride_enqueue() stride_proc_tick()
 *     skew_heap_entry_t lab6_run_pool; //stride_sched   init:alloc_proc()   modify:stride_enqueue() stride_dequeue()
 *     uint32_t lab6_stride;            //stride_sched   init:alloc_proc()   modify:stride_pick_next()
 *     uint32_t lab6_priority;          //               init:alloc_proc()
 * };
 */

/* 
 * 优先队列的比较函数，主要思路就是通过步数相减，然后根据其正负比较大小关系。
 * */
static int proc_stride_comp_f(void *a, void *b)
{
     struct proc_struct *p = le2proc(a, lab6_run_pool); //通过进程控制块指针取得进程a
     struct proc_struct *q = le2proc(b, lab6_run_pool); //通过进程控制块指针取得进程b
     int32_t c = p->lab6_stride - q->lab6_stride;       //步数相减，通过正负比较大小关系
     if (c > 0)
          return 1;
     else if (c == 0)
          return 0;
     else
          return -1;
}

/*
 * 进行调度算法初始化的函数，在本stride调度算法的实现中使用了斜堆来实现优先队列，因此需要对相应的成员变量进行初始化
 */
static void stride_init(struct run_queue *rq)
{
     /* LAB6: YOUR CODE */
     memcpy(sched_fun, stride_sched_fun, sizeof(stride_sched_fun)); //设置调度类函数名称
     list_init(&(rq->run_list));                                    //初始化调度器
     rq->lab6_run_pool = NULL;                                      //对斜堆进行初始化，表示优先队列为空
     rq->proc_num = 0;                                              //设置运行队列为空
}

/*
 * 将指定进程加入就绪队列的时候，需要调用斜堆的插入函数将其插入到斜堆中，然后对时间片等信息进行更新
 */
static void stride_enqueue(struct run_queue *rq, struct proc_struct *proc, char *const str)
{
     /* LAB6: YOUR CODE */
     // 第一次执行：skew_heap_insert(NULL, &(proc->lab6_run_pool), comp);
     // 将新的进程插入到表示就绪队列的斜堆中，该函数返回结果是斜堆新的根
     rq->lab6_run_pool = skew_heap_insert(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);

     if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice)
     {
          proc->time_slice = rq->max_time_slice; //将该进程剩余时间片重新置为最大值
     }
     proc->rq = rq;  //更新进程的就绪队列
     rq->proc_num++; //就绪队列中进程的数量加1

     Print_enqueue(rq, proc, str);
}

/*
 * 将指定进程从就绪队列中删除，只需要将该进程从斜堆中删除掉即可
 */
static void stride_dequeue(struct run_queue *rq, struct proc_struct *proc)
{
     /* LAB6: YOUR CODE */
     assert(proc->rq == rq);
     rq->lab6_run_pool =
         skew_heap_remove(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f); //删除斜堆中的指定进程

     rq->proc_num--; //就绪队列中进程的数量减1

     //Print_dequeue(proc);
}

/*
 * 选择下一个要执行的进程，根据stride算法，只需要选择stride值最小的进程，即斜堆的根节点对应的进程即可
 */
static struct proc_struct *stride_pick_next(struct run_queue *rq)
{
     /* LAB6: YOUR CODE */
     if (rq->lab6_run_pool == NULL)
          return NULL;
     struct proc_struct *p = le2proc(rq->lab6_run_pool, lab6_run_pool); //选择stride值最小的进程

     if (p->lab6_priority == 0)         //优先级为0
          p->lab6_stride += BIG_STRIDE; //步长设置为最大值
     else
          p->lab6_stride += BIG_STRIDE / p->lab6_priority; //步长设置为优先级的倒数，更新该进程的stride值

     //Print_pick_next(p);

     return p;
}

/*
 * 每次时钟中断需要调用的函数，仅在进行时间中断的ISR中调用
 */
static void stride_proc_tick(struct run_queue *rq, struct proc_struct *proc)
{
     if (proc->time_slice > 0)
     {
          proc->time_slice--;
     }
     if (proc->time_slice == 0)
     {
          proc->need_resched = 1;
     }
}

struct sched_class default_sched_class = {
    .name = "stride_scheduler",
    .init = stride_init,
    .enqueue = stride_enqueue,
    .dequeue = stride_dequeue,
    .pick_next = stride_pick_next,
    .proc_tick = stride_proc_tick,
};
