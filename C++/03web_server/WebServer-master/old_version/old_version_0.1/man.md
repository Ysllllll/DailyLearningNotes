# 线程池分析

1. threadpool_create()函数先创建一个管理内存池的中心枢纽结构体。然后再创建多个线程。注意线程的特点，在创建的过程中可能就已经有线程接收到任务开始执行了，但是该函数还是会继续按照要求创建相应数量的线程。同时在该函数中只会判断创建的每一个线程是否成功，全部成功则该函数顺利结束，如果有一个不成功则会执行 threadpool_destroy() 函数

2. threadpool_destroy() 它的主体将会通知所有待执行和正在运行的线程结束运行，即调用 pthread_cond_broadcast() 进行通知，同时调用 pthread_join() 等待所有线程执行完毕

3. threadpool_thread() 这个就是线程的主体部分，注意这个线程的返回值并不影响第一点中 判断是否成功创建的线程，这个线程退出情况时：条件变量通知它没有任务可以执行或者线程池被关闭。否则它将一直循环执行任务。支持优雅关闭，如果还有任务没有执行完就会执行完之后在销毁线程

4. threadpool_free() 就是释放内存池管理数据结构所占用的空间，但是这里是使用的 free 释放空间，不知道会不会造成内存泄露

5. threadpool_add() 将一个任务添加到 queue 任务队列中，然后通知所有线程处理该任务，具体是哪一个线程接手该任务不得而知

这里就可以看出，其实这个任务队列没有优先级之分的

```C++
/*
                  ┌──────────┐
                  │ while:   │
                  │   task   │
                  │          │
                  │ quit:    │
                  │  no task │    ┌──────────┐
                  │ pool shutdown │ while:   │
┌────────┐        └───────────    │   task   │
│        │        ▲y?             │          │
│        │        │               │          │
│        │        xxx         y?  │ quit:    │
│  ...   │◄────── xxx  ─────────► │  no task │
│        │y?      xxx ┼           │ pool shutdown(from destroy)
└────────┘      pool center       └───────────
                   │  ──────────┐
                 y?▼          y?└►┌──────────┐
                ┌──────────┐      │          │
                │          │      │          │
                │          │      │          │
                │          │      │          │
                │          │      │    ...   │
                │  ...     │      │          │
                │          │      └──────────┘
                └──────────┘
*/
```

[Linux 中的 EAGAIN 含义](https://www.cnblogs.com/big-xuyue/p/3905597.html)
[关于 errno ==EINTR 的小结](https://www.cnblogs.com/sunzl1987/archive/2012/06/26/2563220.html)
[read()和 write()](https://www.cnblogs.com/lnlin/p/9492144.html)
[linux 信号 signal 和 sigaction 理解](https://blog.csdn.net/beginning1126/article/details/8680757)
