## 参考文献

- [线程 pthread_t 与 pid_t 的区别](https://blog.csdn.net/zxc120389574/article/details/105201003)
- [linux 中 pid，tid， 以及 真实 pid 的关系](https://blog.csdn.net/u012398613/article/details/52183708/?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-0&spm=1001.2101.3001.4242)
- [pthread_mutex_t 和 pthread_cond_t 配合使用的简要分析](https://blog.csdn.net/chengonghao/article/details/51779279)

这里面的生产者消费者模型可以用一个房间有两扇门，同时这两扇门用的是同一把钥匙。有两个线程先生他们都会取用该钥匙从属于自己的门进去，开始做事情。有一个消费先生和一个生产先生。

我们来看下：消费先生拿到了这个钥匙 `pthread_mutex_lock(&lock)`，然后打开房门进去了，但是发现房间里面没得东西，然后就瓜起。先留了个电话纸条（`cond`）在房间里面，然后离开房间了，**同时**把钥匙放出来了 `pthread_cond_wait()（内含pthread_mutex_unlock(&lock)）`。ok，生产先生带着东西准备放到房间里面，拿到了消费先生放着的钥匙`pthread_mutex_lock(&lock)`，进入房间，把东西放下，然后看到有个电话纸条（`cond`），于是一个电话打过去 `pthread_cond_signal()`，告诉消费先生房间里面有东西了，但是这个时候因为生产先生拿到钥匙的，消费先生就算知道了而且站在门外也进不去，就瓜起。但是它确实知道里面有东西了，然后生产先生就出来了把钥匙放回的原位`pthread_mutex_unlock(&lock)`，离开。最后消费先生不管是马上看见钥匙还是后面回来才看到钥匙，都可以拿着钥匙`pthread_mutex_lock(&lock)`进去消费东西了，最后离开房间的时候把钥匙又放回去`pthread_mutex_unlock(&lock)`。

这里会有一个讨论，就是说生产先生进入房间然后通知消费先生里面有东西了呢？还是不进入房间，生产先生就在外面把东西甩进去，然后就通知消费先生里面有东西？第一种就是加 lock，第二种就是不加 lock。如果是第二种，消费先生这时恰好在门外的话就可以马上拿着钥匙进去消费了，第一种的话这个时候还是要在外面 gu 到等起。

```bash
g++ main.cc threadPool.cc  -lpthread -o main
./main
```
