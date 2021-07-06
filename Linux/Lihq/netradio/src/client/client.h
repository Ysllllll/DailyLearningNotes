#ifndef CLIENT_H__
#define CLIENT_H__

#define DEFAULT_PLAYERCMD "/usr/bin/mpg123 > /dev/null"
struct client_conf_st
{
	char *rcvport;
	char *mgroup;
	char *player_cmd;
};

// 全局变量如果还有机会在别的.c文件中使用，就要做成全局变量，并且在.h文件中进行声明。
// 这样别的.c文件在包了当前.h文件之后，就相当于也能够看到这个全局变量。
// extern扩展了当前全局变量的作用域
extern struct client_conf_st client_conf;

#endif