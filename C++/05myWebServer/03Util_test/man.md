关于 size_t 和 ssize_t 的区别，参考文章[size_t 和 ssize_t](https://blog.csdn.net/qq_35231095/article/details/107468547)。

简单来说使用 size_t 是为了代码的移植性，size_t 可用于计数那些不会是负数的量，而 ssize_t 可用于那些值可能是负数的量。

[【Networkk】一篇文章完全搞清楚 scoket read/write 返回码、阻塞与非阻塞、异常处理 等让你头疼已久的问题](https://www.cnblogs.com/junneyang/p/6126635.html)
