## 相关文档

[select、poll、epoll 总结及 ET、LT 区别](https://www.jianshu.com/p/12652dfac760)
主要关注三种方式在用户空间和内核空间之间的数据传输。另外需要关注 ET、LT 两种模式的实际含义

[epoll 事件通知机制详解，水平触发和边沿触发的区别](https://www.jianshu.com/p/7835726dc78b)
man 文档的翻译

[epoll 内核源码分析](https://www.jianshu.com/p/aa486512e989)

## 基础知识

signal（SIGPIPE，SIG_IGN）；

Linux 网络编程第 12 讲 tcp11 种状态中
如果客户端关闭套接字 close
而服务器调用了次 write，服务器会接收一个 RST segment（TCP 传输层）
如果服务器端再次调用了 write，这个时候就会产生 SIGPIPE 信号。

7\*24

TIME_WAIT 状态对大并发服务器的影响

应尽可能在服务器端避免出现 TIMEWAIT 状态
如果服务器端主动断开连接（先与 client 调用 close），服务端就会进入 TIME_WAIT

协议设计上，应该让客户端主动断开连接，这样就把 TIME_WAIT 状态分散到大量的客户端。如果客户端不活跃了，一些客户端不断开连援，这样子就会占用服务器端的连接资源。服务器端也有要个机制来踢掉不活跃的连 close

nonblocking socket +I/0 复用
