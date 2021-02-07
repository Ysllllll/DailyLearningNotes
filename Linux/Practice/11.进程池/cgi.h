#include "processpool.h"

class cgi_conn
{
public:
    cgi_conn() {}
    ~cgi_conn() {}
    void init(int epollfd, int sockfd, const sockaddr_in &client_addr)
    {
        m_epollfd = epollfd;
        m_sockfd = sockfd;
        m_address = client_addr;
        memset(m_buf, 0, BUFFER_SIZE);
        m_read_idx = 0;
    }

    void process()
    {
        int idx = 0;
        int ret = -1;
        while (true)
        {
            idx = m_read_idx;
            ret = recv(m_sockfd, m_buf + idx, BUFFER_SIZE - 1 - idx, 0);
            if (ret < 0)
            {
                if (errno != EAGAIN)
                    removefd(m_epollfd, m_sockfd);
                break;
            }
            else if (ret == 0)
            {
                removefd(m_epollfd, m_sockfd);
                break;
            }
            else
            {
                m_read_idx += ret;
                printf("user content is:%s\n", m_buf);
                for (; idx < m_read_idx; idx++)
                    if ((idx >= 1) && (m_buf[idx - 1] == '\r') && (m_buf[idx] == '\n'))
                        break;

                if (idx == m_read_idx)
                    continue;
                m_buf[idx - 1] = '\0';

                char *file_name = m_buf;
                if (access(file_name, F_OK) == -1)
                {
                    removefd(m_epollfd, m_sockfd);
                    break;
                }
                ret = fork();
                if (ret == -1)
                {
                    removefd(m_epollfd, m_sockfd);
                    break;
                }
                else if (ret > 0)
                {
                    removefd(m_epollfd, m_sockfd);
                    break;
                }
                else
                {
                    close(STDOUT_FILENO);
                    dup(m_sockfd);
                    execl(m_buf, m_buf, NULL);
                    exit(0);
                }
            }
        }
    }

private:
    static const int BUFFER_SIZE = 1024;
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFER_SIZE];
    int m_read_idx;
};
int cgi_conn::m_epollfd = -1;