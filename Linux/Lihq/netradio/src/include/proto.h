#ifndef PROTO_H__
#define PROTO_H__
#include <site_type.h>

#define DEFAULT_MGROUP "224.2.2.2"
#define DEFAULT_RCVPORT "1989"

#define CHNNR 100

#define LISTCHNID 0

#define MINCHNID 1
#define MAXCHNID (MINCHNIT + CHNNR - 1)

#define MSG_CHANNEL_MAX (65536 - 20 - 8) //-IP报的报头 - udp报的报头
#define MSG_DATA (MSG_CHANNEL_MAX - sizeof(chnid_t))

#define MSG_LIST_MAX (65535 - 20 - 8)
#define MAX_ENTRY (MSG_LIST_MAX - sizeof(chnid_t))

struct msg_channel_st
{
	chnid_t chnid; /*must between [MINCHNID, MAXCHNID]*/
	uint8_t data[1];
} __attribute__((packed)); //告诉gcc udp的包不考虑对齐方式

struct msg_listentry_st
{
	chnid_t chnid; /*must between LISTCHNID*/
	uint16_t len;
	uint8_t desc[1];
} __attribute__((packed));

struct msg_list_st
{
	chnid_t chnid; /*must between LISTCHNID*/
	struct msg_listentry_st entry[1];
} __attribute__((packed));

#endif