关于 size_t 和 ssize_t 的区别，参考文章[size_t 和 ssize_t](https://blog.csdn.net/qq_35231095/article/details/107468547)。

简单来说使用 size_t 是为了代码的移植性，size_t 可用于计数那些不会是负数的量，而 ssize_t 可用于那些值可能是负数的量。

[【Networkk】一篇文章完全搞清楚 scoket read/write 返回码、阻塞与非阻塞、异常处理 等让你头疼已久的问题](https://www.cnblogs.com/junneyang/p/6126635.html)

---

针对 setsockopt 函数，当中如果在已经处于 ESTABLISHED 状态下的 socket(一般由端口号和标志符区分）调用 close(socket)（一般不会立即关闭而经历 TIME_WAIT 的过程）后想继续重用该 socket：

```C++
int reuse=1;
setsockopt(s,SOL_SOCKET ,SO_REUSEADDR,(const char\*)& reuse,sizeof(int));
```

注意：必须在调用 bind 函数之前设置 SO_REUSEADDR 选项。

参考：[linux 网络编程系列（六）--setsockopt 的常用选项](https://www.jianshu.com/p/cb157208eb24)

---

考虑为什么需要 readn 和 writen

首先需要弄明白[应用程序写数据到一个 TCP 套接字中发生的事情](https://blog.csdn.net/tiankong_/article/details/74251061)，可以看到，我们的应用程序与其通过 tcp 连接的对端之间，发送和接收数据的实质是应用程序与内核中的 tcp 发送和接收缓冲区的交互。

在[我们已经有了系统提供的接口 read 和 write，为什么还需要 readn writen 和 readline 呢？](https://blog.csdn.net/tiankong_/article/details/74516422)

因为字节流套接字上调用 read 或 write 输入和输出的字节数可能比请求的数量少，然而这不是出错状态，为了预防万一，不让实现返回一个不足的字节计数值

---

接下来，我们需要分析 readn 和 writen 这两个操作如何使用

在 main 程序中我们给出了一个服务器端的 listen_fd 套接字，将其设置为非阻塞，所以在后续 accept 一个客户端连接的时候必须是用 while，使得在执行 readn 之前已经获取了一个客户端，当然你可以使用其它手段来 accept 一个客户端。

在 accept 一个客户端之后，我们的服务端将调用 readn 和 writen 对这个 conn 客户端进行读写。在另外一个端口开一个 telnet 192.168.123.84 54321 客户端来访问这个服务端。我们可以发现传进 readn 和 writen 中的套接字不能为非阻塞的，换而言之，readn 和 writen 都是阻塞函数调用，这表明我们必须从内核的缓冲区读到我们需要数量的字节才返回，否则将一直阻塞下去。

这里考虑的一个问题是为什么 conn 客户端套接字要设置为阻塞的，可以看下 readn 函数，函数中，如果 read 为非阻塞，那么在没有信息的时候 read 会立即返回-1，然后跳出 readn，这样就没有达到我们的要读取指定字节数的功能。所以 read 必须为阻塞函数，从而 readn 也未阻塞函数

[UNIX 网络编程卷１：套接字联网-第 3 章:套接字编程-readn,writen 函数](https://blog.csdn.net/tiankong_/article/details/74516422)

关于同步、异步、阻塞、非阻塞等概念，参考[linux 异步非异步阻塞非阻塞(转载 )](https://www.jianshu.com/p/0aca498f50ca)
