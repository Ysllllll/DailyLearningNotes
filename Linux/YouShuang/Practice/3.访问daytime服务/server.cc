#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    /*在命令行中输入 ./server 192.168.124.x。
    若是没有反应，则可能服务器的daytime服务没有启动
    第一步：安装xinetd
            sudo apt-get install xinetd
    第二步：将/etc/xinetd.d/daytime文件中的两个 disable 的值 yes 改为 no 。
            chmod 777 / etc / xinetd.d / daytime
            vim /etc/xinetd.d/daytime
    第三步：注销下系统或重新启动下xinetd服务。
            sudo /etc/init.d/xinetd restart

    如果还是不行，则可能服务端没有将该端口设置为可访问：
            firewall-cmd --permanent --add-port=13/tcp
            firewall-cmd --reload
    */
    assert(argc == 2);
    char *host = argv[1];

    //获取目标主机地址信息   hostent{主机名、主机别名列表、地址类型、地址长度、按网络字节序列出的主机IP地址列表}
    struct hostent *hostinfo = gethostbyname(host); //先通过/etc/hosts配置文件查找主机信息，如果没有找到，再去访问DNS服务器
    assert(hostinfo);

    //获取daytime服务信息   servent{服务器名称、服务器名称列表、端口号、服务类型}
    struct servent *servinfo = getservbyname("daytime", "tcp"); //读取/etc/services文件来获取服务的信息
    assert(servinfo);
    printf("daytime port is %d\n", ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;

    //注意下面的代码，因为h_addr_list本身是使用网络字节序的地址列表，所以使用其中的IP地址时，无须对目标IP地址转换字符串
    address.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int res = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    //assert(res != -1);

    char buffer[128];
    res = recv(sockfd, buffer, sizeof(buffer), 0);
    //assert(res > 0);
    buffer[res] = '\0';
    printf("the day time is :%s\n", buffer);
    close(sockfd);
    return 0;
}