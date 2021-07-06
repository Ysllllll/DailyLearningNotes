#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <socket.h>
#include <client.h>
#include <proto.h>

/* 
 * cmd -opt(--opt) argv -opt(--opt) argv -opt(--opt) argv...
 * -M	--mgroup	指定多播组
 * -P	--port		指定接收端口
 * -p	--player	指定播放器
 * -H	--help		显示帮助
 * */

struct client_conf_st client_conf =
	{
		.rcvport = DEFAULT_RCVPORT,
		.mgroup = DEFAULT_MGROUP,
		.player_cmd = DEFAULT_PLAYERCMD};

void printhelp(void)
{
	printf("-M --mgroup	 	指定多播组\n");
	printf("-P --port 		指定接收端口\n");
	printf("-p --player 	指定播放器命令行\n");
	printf("-H --help 		显示帮助\n");
}

int main(int argc, char **argv)
{
	int pd[2];
	pid_t pid;
	int c;
	int val;
	int sd;
	int index = 0;
	int len;
	int ret;
	int chosenid;
	struct sockaddr_in laddr, serveraddr;
	socklen_t serveraddr_len;
	struct ip_mreqn mreq;
	struct option *argarr[] = {{"port", 1, NULL, 'P'}, {"mgroup", 1, NULL, 'M'}, {"player", 1, NULL, 'p'}, {"help", 0, NULL, 'H'}, {NULL, 0, NULL, 0}};
	/*
	*	初始化
	*	级别：默认值，配置文件，环境变量，命令行参数
	*/
	while (1)
	{
		c = getop_long(argc, argv, "P:M:p:H", argarr, &index);
		if (c < 0)
			break;
		switch (c)
		{
		case 'P':
			client_conf.rcvport = optarg;
			break;
		case 'M':
			client_conf.mgroup = optarg;
			break;
		case 'p':
			client_conf.player_cmd = optarg;
			break;
		case 'H':
			printhelp();
			exit(0);
		default:
			abort();
			break;
		}
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0)
	{
		perror("socket()");
		exit(0);
	}

	inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
	mreq.imr_ifindex = if_nametoindex("eth0");
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(client_conf.rcvport));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
	if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	if (pipe(pd) < 0)
	{
		perror("pipe()");
		exit(1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	if (pid == 0)
	{
		// 子进程：调用解码器
		close(sd);
		close(pd[1]);
		dup2(pd[0], 0);
		if (pd[0] > 0)
			close(pd[0]);

		execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
		perror("execl()");
		exit(1);
	}
	// 父进程：从网络上收包，发送给子进程
	// 收节目单
	struct msg_list_st *msg_list;
	msg_list = malloc(MSG_LIST_MAX);
	if (msg_list == NULL)
	{
		perror("malloc()");
		exit(1);
	}

	while (1)
	{
		len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&serveraddr, &serveraddr_len);
		if (len < sizeof(struct msg_list_st))
		{
			fprintf(stderr, "message is too small.\n");
			continue;
		}
		if (msg_list->chnid != LISTCHNID)
		{
			fprintf(stderr, "chnid is not match.\n");
			continue;
		}
		break;
	}

	// 打印节目单并选择频道
	struct msg_listentry_st *pos;
	for (pos = msg_list->entry; (char *)pos < (((char *)msg_list) + len); pos = (void *)(((char *)pos) + ntohs(pos->len)))
	{
		printf("channel %d : %s\n", pos->chnid, pos->desc);
	}

	free(msg_list);
	while (1)
	{
		ret = scanf("%d", &chosenid);
		if (ret != 1)
			exit(1);
	}
	// 收频道包并且发送给子进程
	struct msg_channel_st *msg_channel;
	msg_channel = malloc(MSG_CHANNEL_MAX);
	if (msg_channel == NULL)
	{
		perror("malloc()");
		exit(1);
	}

	exit(0);
}