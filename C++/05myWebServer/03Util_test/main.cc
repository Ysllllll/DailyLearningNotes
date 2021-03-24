#include "Util.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc <= 2)
		return -1;
	printf("%s\n", argv[1]);
	printf("%s\n", argv[2]);
	int listen_fd = socket_bind_listen(atoi(argv[2]), argv[1]); //获取服务器端的监听端口
	setSocketNonBlocking(listen_fd);							//将该端口设定为非阻塞

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);

	int conn;
	while ((conn = accept(listen_fd, (struct sockaddr *)&client, &client_addrlength)) == -1) //由于监听端口为非阻塞，所以需要循环accept客户端的连接
		;

	printf("get client: %d\n", conn); //表明有客户端连接
	//setSocketNonBlocking(conn);		  //这里不能将conn设置为非阻塞
	char buf[5];
	memset(buf, '\0', 5);
	while (1)
	{
		int ret = readn(conn, buf, 5); //读取客户端发送来的信息，注意这个函数返回时，ret一定为5
		if (ret < 0)
			continue;
		std::cout << "ret: " << ret << std::endl;
		std::cout << buf << std::endl;
		ret = writen(conn, buf, 5);
		memset(buf, '\0', 5);
	}
	return 0;
}