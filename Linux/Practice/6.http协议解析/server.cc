#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

//程序运行
// 1.bash 1.sh
//服务器端： ./server 192.168.124.7 54321
//客户端：   ./client 192.168.124.7 54321

//"GET http://www.baidu.com/index.html HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n"

#define BUFFER_SIZE 4096

/* 主状态机的两种状态*/
enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0, // 当前正在分析请求行
    CHECK_STATE_HEADER           // 当前正在分析头部字段
};

/* 从状态机的三种状态*/
enum LINE_STATUS
{
    LINE_OK = 0, // 读取到完整行
    LINE_BAD,    // 行出错
    LINE_OPEN    // 行不完整需要再次读取
};

/* 处理HTTP的结果 */
enum HTTP_CODE
{
    NO_REQUEST,        //请求不完整，需要继续读取客户数据
    GET_REQUEST,       //表示获得了一个完整的客户请求
    BAD_REQUEST,       //表示客户请求有语法错误
    FORBIDDEN_REQUEST, //表示客户对资源没有足够的访问权限
    INTERNAL_ERROR,    //表示服务器内部错误
    CLOSED_CONNECTION  //表示客户端已经关闭连接了
};

/* 为了简化问题，我们不给客户端发送一个完整的HTTP应答报文，而只是根据服务器的处理结果发送：成功 或 失败 的信息 */
static const char *szret[] = {"I get a correct result\n", "Something wrong\n"};

/* 从状态机, 解析出一行内容*/
LINE_STATUS parse_line(char *buffer, int &checked_index, int &read_index)
{
    char temp;

    // 分析buffer中是否有\r字符, 只有\r字符说明不完整, \r\n都存在说明完整, 都不存在说明不完整
    // checked_index指向buffer（应用程序的读缓冲区）中当前正在分析的字节
    // read_index指向buffer中客户数据的尾部的下一字节
    // buffer中第0~checked_index字节都已经分析完毕，第checked_index~(read_index-1)字节由下面的循环挨个分析
    for (; checked_index < read_index; checked_index++)
    {
        temp = buffer[checked_index]; //获取当前要分析的字节
        if (temp == '\r')             //如果当前的字节时“\r”，即回车符，则说明可能读取到一个完整的行
        {
            if ((checked_index + 1) == read_index) //如果“\r”字符碰巧是目前buffer中的最后一个已经被读入的客户数据
            {                                      //那么这次分析没有读取到一个完整的行，返回LINE_OPEN以表示还需要继续读取客户数据才能进一步分析
                return LINE_OPEN;
            }
            else if (buffer[checked_index + 1] == '\n') //如果下一个字符是“\n”，则说明我们成功读取到一个完整的行
            {

                buffer[checked_index++] = '\0'; //？？？？
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            else //否则的话，说明客户发送的HTTP请求存在语法问题
                return LINE_BAD;
        }
        else if (temp == '\n') //如果当前的字节是“\n”，即换行符，则也说明可能读取到一个完整的行
        {
            if ((checked_index > 1) && buffer[checked_index - 1] == '\r')
            {

                buffer[checked_index - 1] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }

    //如果所有的内容都分析完毕也没有遇到“\r”字符，则返回LINE_OPEN，表示还需要继续读取客户数据才能进一步分析
    return LINE_OPEN;
}

/* 分析请求行 */
HTTP_CODE parse_requestline(char *temp, CHECK_STATE &checkstate)
{
    /*小测试
	char temp[20];
    strcpy(temp, "abc \tcde");
    char *url = strpbrk(temp, " \t"); //在源字符串（s1：temp）中找出最先含有搜索字符串（s2：“ \t”）中任一字符的位置并返回，若找不到则返回空指针
    if (!url)
    {
        printf("失败\n");
    }
    *url++ = '\0';
    
    printf("%s\n", temp);
    printf("%s\n",url);
    return 0;
	*/
    // Http请求必定带有空白字符 或者 \t字符
    char *url = strpbrk(temp, " \t"); //在源字符串（s1：temp）中找出最先含有搜索字符串（s2：“ \t”）中任一字符的位置并返回，若找不到则返回空指针
    if (!url)
    {
        return BAD_REQUEST;
    }
    *url++ = '\0';

    char *method = temp;
    // 仅支持GET方法
    if (strcasecmp(method, "GET") == 0) //忽略大小写比较字符串
    {
        printf("The request method is GET\n");
    }
    else
    {
        return BAD_REQUEST;
    }

    url += strspn(url, " \t");
    char *version = strpbrk(url, " \t");
    if (!version)
    {
        return BAD_REQUEST;
    }

    *version++ = '\0';
    version += strspn(version, " \t");
    // 仅支持HTTP1.1
    if (strcasecmp(version, "HTTP/1.1") != 0)
    {
        return BAD_REQUEST;
    }
    // 检查URL完整性
    if (strncasecmp(url, "http://", 7) == 0)
    {
        url += 7;
        url = strchr(url, '/');
    }
    if (!url || url[0] != '/')
    {
        return BAD_REQUEST;
    }
    printf("The request URL is: %s\n", url);

    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

/* 分析头部字段 */
HTTP_CODE parse_headers(char *temp)
{
    if (temp[0] == '\0') //遇到一个空行，说明我们得到了一个正确的HTTP请求
    {
        return GET_REQUEST;
    }
    else if (strncasecmp(temp, "Host:", 5) == 0) //处理“HOST”头部字段
    {
        temp += 5;
        temp += strspn(temp, " \t");
        printf("The request host is: %s\n", temp);
    }
    else //其它头部字段都不处理
    {
        printf("I can not handle this header\n");
    }
    return NO_REQUEST;
}

/* 分析HTTP请求的入口函数 */
HTTP_CODE parse_content(char *buffer, int &checked_index, CHECK_STATE &checkstate, int &read_index, int &start_line)
{
    LINE_STATUS linestatus = LINE_OK; //记录当前行的读取状态
    HTTP_CODE retcode = NO_REQUEST;   //记录HTTP请求的处理结果

    //主状态机，用于从buffer中取得所有完整的行
    //示例1："GET \thttp://www.baidu.com/index.html \tHTTP/1.0\r\nHost: \twww.baidu.com\r\n" 两个完整的行
    //示例2："GET \thttp://www.baidu.com/index.html \tHTTP/1.0\r\nHost: \twww" 第一个行完整，第二个行不完整
    while ((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK)
    {

        char *temp = buffer + start_line; //start_line是行在buffer中的起始位置
        start_line = checked_index;       //记录下一行的起始位置

        switch (checkstate) //checkstate记录主状态机当前状态
        {
        case CHECK_STATE_REQUESTLINE: //第一个状态，分析请求行
            retcode = parse_requestline(temp, checkstate);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
        case CHECK_STATE_HEADER: //第二个状态，分析头部字段
            retcode = parse_headers(temp);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if (retcode == GET_REQUEST)
            {
                return GET_REQUEST;
            }
            break;
        default:
            return INTERNAL_ERROR;
        }
    }

    //若没有读取到一个完整的行，则表示还需要继续读取客户数据才能进一步分析
    //示例："GET \thttp://www.baidu.com/index.html" 非完整信息，缺少版本号
    if (linestatus == LINE_OPEN)
    {
        return NO_REQUEST;
    }
    else
    {
        return BAD_REQUEST;
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("Wrong number of parameters\n");
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    while (ret == -1)
    {
        address.sin_port = htons(++port);
        ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
        printf("the port is %d\n", port);
    }
    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int fd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);

    if (fd < 0)
    {
        printf("errno is: %d\n", errno);
    }
    else
    {
        char buffer[BUFFER_SIZE];          //读缓冲区
        memset(buffer, '\0', BUFFER_SIZE); //清空缓存区
        int date_read = 0;
        int read_index = 0;    //当前已经读取了多少字节的客户数据
        int checked_index = 0; //当前已经分析完了多少字节的客户数据
        int start_line = 0;    //行在buffer中的起始位置

        CHECK_STATE checkstatus = CHECK_STATE_REQUESTLINE; //设置主状态机的初始状态
        while (1)                                          //循环读取客户数据并分析之
        {
            date_read = recv(fd, buffer + read_index, BUFFER_SIZE - read_index, 0);
            if (date_read == -1)
            {
                printf("reading failed\n;");
                break;
            }
            else if (date_read == 0)
            {
                printf("remote client had closed the connection\n");
                break;
            }
            read_index += date_read;

            HTTP_CODE result = parse_content(buffer, checked_index, checkstatus, read_index, start_line);
            if (result == NO_REQUEST) //尚未得到一个完整的HTTP请求
            {
                continue;
            }
            else if (result == GET_REQUEST) //得到一个完整的、正确的HTTP请求
            {
                send(fd, szret[0], strlen(szret[0]), 0);
                break;
            }
            else //其它情况表示发生错误
            {
                send(fd, szret[1], strlen(szret[1]), 0);
                break;
            }
        }
        close(fd);
    }
    close(listenfd);
    return 0;
}
