#include "tw_timer.h"

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024

static int pipefd[2];
static time_wheel timer_wheel;
int time_slot = time_wheel::get_tick_SI();
int connection_time_out = time_slot * 5;
static int epollfd = 0;

int setNonBlocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setNonBlocking(fd);
}

void sig_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

void addsig(int sig)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    int ret = sigaction(sig, &sa, NULL);
    assert(ret != -1);
}

void cb_func(client_data *user_data)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    printf("close fd %d\n", user_data->sockfd);
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("argument error");
        return 1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    int ret = bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address));
    assert(ret != -1);
    ret = listen(listenfd, 5);
    assert(ret != -1);

    int epollfd = epoll_create(5);
    assert(epollfd >= 0);
    addfd(epollfd, listenfd);
    epoll_event events[MAX_EVENT_NUMBER];

    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret != -1);
    setNonBlocking(pipefd[1]);
    addfd(epollfd, pipefd[0]);

    addsig(SIGALRM);
    addsig(SIGTERM);

    client_data *users = new client_data[FD_LIMIT];

    bool stop_server = false;
    alarm(time_slot);

    while (!stop_server)
    {
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((num < 0) && errno != EINTR)
        {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < num; i++)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_address_len = sizeof(client_address);
                int connfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_len);
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
                printf("accept a client, ip: %s, port: %d\n", client_ip, ntohs(client_address.sin_port));
                addfd(epollfd, connfd);

                tw_timer *timer = timer_wheel.add_timer(connection_time_out);
                timer->user_data = &users[connfd];
                timer->cb_func = cb_func;

                users[connfd].timer = timer;
                users[connfd].address = client_address;
                users[connfd].sockfd = connfd;
            }
            else if (sockfd == pipefd[0] && events[i].events & EPOLLIN)
            {
                char signals[1024];
                int ret = recv(pipefd[0], signals, 1024, 0);
                if (ret == -1 || !ret)
                    continue;
                else
                    for (int i = 0; i < ret; i++)
                        switch (signals[i])
                        {
                        case SIGALRM:
                            timer_wheel.tick();
                            break;
                        case SIGTERM:
                            stop_server = true;
                        }
            }
            else if (events[i].events & EPOLLIN)
            {
                memset(&users[sockfd].buf, 0, BUFFER_SIZE);
                ret = recv(sockfd, &users[sockfd].buf, BUFFER_SIZE, 0);
                printf("get %d byte of from client %d : %s", ret, sockfd, users[sockfd].buf);

                tw_timer *timer = users[sockfd].timer;
                if (ret <= 0)
                {
                    cb_func(&users[sockfd]);
                    timer_wheel.del_timer(timer);
                    users[sockfd].timer = NULL;
                }
                else
                {
                    timer_wheel.del_timer(timer);
                    tw_timer *timer = timer_wheel.add_timer(connection_time_out);
                    timer->user_data = &users[sockfd];
                    timer->cb_func = cb_func;
                    users[sockfd].timer = timer;
                }
            }
            else
            {
            }
        }
    }
    close(listenfd);
    close(pipefd[0]);
    close(pipefd[1]);
    delete[] users;
    return 0;
}