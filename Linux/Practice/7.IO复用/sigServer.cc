#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define LISTEM_NUM 5
#define MAX_EVENT_NUM 1024

int setnonblocking(int fd);

void add_epoll_fd(int epoll_fd, int fd);

void sig_handler(int sigalno);

void add_sig(int sigalno);

static int pipe_fd[2];

int main(int argc, char *argv[])
{
    //./sigServer 192.168.124.7 54321
    // telnet 192.168.124.7 54321
    // ps aux 找到./sigServer对应进程的PID
    // kill -n 1 10382(./sigServer的PID)
    if (argc != 3)
    {
        printf("usage:./%s [server ip] [server port]\n", basename(argv[1]));
        exit(EXIT_FAILURE);
    }

    int ser_fd, server_port;
    const char *server_ip;

    //创建套接字
    if ((ser_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //初始化服务端地址
    struct sockaddr_in server_address;

    server_ip = argv[1];
    server_port = atoi(argv[2]);
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr.s_addr) == -1)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    //绑定服务端地址
    if (bind(ser_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    //开启监听
    if (listen(ser_fd, LISTEM_NUM) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    int epoll_fd;
    //创建epoll事件表句柄
    if ((epoll_fd = epoll_create(5)) == -1)
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    //将服务端套接字加入到事件表中
    add_epoll_fd(epoll_fd, ser_fd);

    //创建管道
    if (socketpair(PF_UNIX, SOCK_STREAM, 0, pipe_fd) == -1)
    {
        perror("socketpair");
        exit(EXIT_FAILURE);
    }
    /*sockpair函数创建的管道是全双工的，不区分读写端
        此处我们假设pipe_fd[1]为写端，非阻塞
        pipe_fd[0]为读端
    */
    setnonblocking(pipe_fd[1]);
    add_epoll_fd(epoll_fd, pipe_fd[0]);

    //为一些信号绑定信号处理函数
    add_sig(SIGHUP);  //终端接口检测到一个连接断开，发送此信号
    add_sig(SIGCHLD); //子进程终止或停止时，子进程发送此信号
    add_sig(SIGTERM); //接收到kill命令
    add_sig(SIGINT);  //用户按下中断键(Delete或Ctrl+C)

    int server_running = 1;
    int epoll_wait_ret_value;

    struct epoll_event events[MAX_EVENT_NUM];

    while (server_running)
    {
        bzero(events, sizeof(events));
        epoll_wait_ret_value = epoll_wait(epoll_fd, events, MAX_EVENT_NUM, -1);

        //epoll_wait函数出错
        if ((epoll_wait_ret_value == -1) && (errno != EINTR))
        {
            close(ser_fd);
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        //遍历就绪的事件
        for (int i = 0; i < epoll_wait_ret_value; ++i)
        {
            int sock_fd = events[i].data.fd; //获取文件描述符
            //如果是服务端套接字,接收客户端的连接
            if (sock_fd == ser_fd)
            {
                int client_fd;
                char client_address_ip[24];
                struct sockaddr_in client_address;
                socklen_t address_len = sizeof(client_address);
                bzero(&client_address, sizeof(client_address));

                if ((client_fd = accept(ser_fd, (struct sockaddr *)&client_address, &address_len)) == -1)
                {
                    perror("accept");
                    continue;
                }

                //将新的客户端套接字放入到事件集中
                add_epoll_fd(epoll_fd, client_fd);
                //打印客户端地址信息
                inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_address_ip, sizeof(client_address_ip));
                printf("get a new client,ip:%s,port:%d\n", client_address_ip, ntohs(client_address.sin_port));
            }
            //如果是管道的一端有数据可读，那么处理信号
            else if ((sock_fd == pipe_fd[0]) && (events[i].events & EPOLLIN))
            {
                char signals[1024];
                int sig;
                int recv_ret_value;
                recv_ret_value = recv(pipe_fd[0], signals, sizeof(signals), 0);
                if (recv_ret_value <= 0)
                    continue;
                else
                {
                    //每个信号值占1字节，所以按字节来逐个接收信号
                    for (int i = 0; i < recv_ret_value; ++i)
                    {
                        printf("server:I caugh the signal %d\n", signals[i]);
                        switch (signals[i])
                        {
                        case SIGCHLD:
                        case SIGHUP:
                            continue;
                        //接收到下面这两个信号，终止程序
                        case SIGTERM: //kill
                        case SIGINT:  //ctrl +c
                            server_running = 0;
                        }
                    }
                }
            }
            //如果是客户端
            else
            {
            }
        }
    }

    printf("service is down\n");
    close(ser_fd);
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    exit(EXIT_SUCCESS);
}

int setnonblocking(int fd)
{
    int old_options, new_options;
    //获取原先的描述符标志
    if ((old_options = fcntl(fd, F_GETFL)) == -1)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    //设置非阻塞
    new_options = old_options | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, new_options) == -1)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    return old_options;
}

void add_epoll_fd(int epoll_fd, int fd)
{
    struct epoll_event new_event;
    bzero(&new_event, sizeof(new_event));

    new_event.events = EPOLLIN | EPOLLET;
    new_event.data.fd = fd;

    //将新事件加入到事件表中
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &new_event) == -1)
    {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    //设置为非阻塞
    setnonblocking(fd);
}

void add_sig(int sigalno)
{
    struct sigaction act;
    bzero(&act, sizeof(act));
    act.sa_handler = sig_handler; //设置信号处理函数
    sigfillset(&act.sa_mask);     //初始化信号屏蔽集
    act.sa_flags |= SA_RESTART;   //由此信号中断的系统调用自动重启动

    //初始化信号处理函数
    if (sigaction(sigalno, &act, NULL) == -1)
    {
        printf("capture signal,but to deal with failure\n");
        return;
    }
}

void sig_handler(int sigalno)
{
    printf("capture signal,signal num is %d", sigalno);
    //保留原来的errno，在函数最后回复，以保证函数的可重入性
    int save_errno = errno;
    int msg = sigalno;
    //将信号值写入管代，通知主循环
    if (send(pipe_fd[1], (char *)&msg, 1, 0) <= 0)
    {
        printf("The message sent to the server failed\n");
    }
    printf(",signal is send to server\n");
    errno = save_errno;
}